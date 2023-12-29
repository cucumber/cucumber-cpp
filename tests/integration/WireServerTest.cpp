#include <cucumber-cpp/internal/connectors/wire/WireServer.hpp>

#include <gmock/gmock.h>

#include <filesystem>
#include <memory>
#include <random>
#include <thread>
#include <chrono>

#include <stdlib.h>
#include <sstream>

using namespace cucumber::internal;
using namespace testing;

static const auto THREAD_TEST_TIMEOUT = std::chrono::milliseconds(4000);

MATCHER(IsConnected, std::string(negation ? "is not" : "is") + " connected") {
    return arg.good();
}

MATCHER(HasTerminated, "") {
    return !arg.joinable();
}

MATCHER(EventuallyTerminates, "") {
    const std::future_status status = arg.wait_for(THREAD_TEST_TIMEOUT);
    return status == std::future_status::ready;
}

MATCHER_P(EventuallyReceives, value, "") {
    std::basic_iostream<char>* stream
        = const_cast<std::basic_iostream<char>*>(static_cast<const std::basic_iostream<char>*>(&arg)
        );
    std::string output;
    // FIXME It should not block
    (*stream) >> output;
    //    boost::timer timer;
    //    double timeout = THREAD_TEST_TIMEOUT.total_milliseconds() / 1000.0;
    //    while (timer.elapsed() < timeout) {
    //        if (stream->rdbuf()->available() > 0) { // it is zero even if it doesn't block!
    //            (*stream) >> output;
    //            break;
    //        }
    //        boost::this_thread::yield();
    //    }
    return (output == value);
}

class MockProtocolHandler : public ProtocolHandler {
public:
    MOCK_METHOD(std::string, handle, (const std::string& request), (const, override));
};

class SocketServerTest : public Test {

protected:
    StrictMock<MockProtocolHandler> protocolHandler;
    std::future<void> serverThread{};

    void SetUp() override {
        SocketServer* server = createListeningServer();
        serverThread = std::async(std::launch::async, &SocketServer::acceptOnce, server);
    }

    void TearDown() override {
        serverThread.wait_for(THREAD_TEST_TIMEOUT);
        destroyListeningServer();
    }

    virtual SocketServer* createListeningServer() = 0;
    virtual void destroyListeningServer() = 0;
};

class TCPSocketServerTest : public SocketServerTest {
protected:
    std::unique_ptr<TCPSocketServer> server;

    SocketServer* createListeningServer() override {
        server.reset(new TCPSocketServer(&protocolHandler));
        server->listen(0);
        return server.get();
    }

    void destroyListeningServer() override {
        server.reset();
    }
};

TEST_F(TCPSocketServerTest, exitsOnFirstConnectionClosed) {
    // given
    asio::ip::tcp::iostream client(server->listenEndpoint());
    ASSERT_THAT(client, IsConnected());
    ASSERT_THAT(server->listenEndpoint().address().to_string(), std::string("0.0.0.0"));

    // when
    client.close();

    // then
    EXPECT_THAT(serverThread, EventuallyTerminates());
}

TEST_F(TCPSocketServerTest, moreThanOneClientCanConnect) {
    // given
    asio::ip::tcp::iostream client1(server->listenEndpoint());
    ASSERT_THAT(client1, IsConnected());

    // when
    asio::ip::tcp::iostream client2(server->listenEndpoint());

    // then
    ASSERT_THAT(client2, IsConnected());
}

TEST_F(TCPSocketServerTest, receiveAndSendsSingleLineMassages) {
    {
        InSequence s;
        EXPECT_CALL(protocolHandler, handle("12")).WillRepeatedly(Return("A"));
        EXPECT_CALL(protocolHandler, handle("3")).WillRepeatedly(Return("B"));
        EXPECT_CALL(protocolHandler, handle("4")).WillRepeatedly(Return("C"));
    }

    // given
    asio::ip::tcp::iostream client(server->listenEndpoint());
    ASSERT_THAT(client, IsConnected());

    // when
    client << "1" << std::flush << "2" << std::endl << std::flush;
    client << "3" << std::endl << "4" << std::endl << std::flush;

    // then
    EXPECT_THAT(client, EventuallyReceives("A"));
    EXPECT_THAT(client, EventuallyReceives("B"));
    EXPECT_THAT(client, EventuallyReceives("C"));
}

class TCPSocketServerLocalhostTest : public SocketServerTest {
protected:
    std::unique_ptr<TCPSocketServer> server;

    SocketServer* createListeningServer() override {
        server.reset(new TCPSocketServer(&protocolHandler));
        server->listen(asio::ip::tcp::endpoint(asio::ip::address::from_string("127.0.0.1"), 0));
        return server.get();
    }

    void destroyListeningServer() override {
        server.reset();
    }
};

TEST_F(TCPSocketServerLocalhostTest, listensOnLocalhost) {
    // given
    asio::ip::tcp::iostream client(server->listenEndpoint());
    ASSERT_THAT(client, IsConnected());
    ASSERT_THAT(server->listenEndpoint().address().to_string(), std::string("127.0.0.1"));

    // when
    client.close();

    // then
    EXPECT_THAT(serverThread, EventuallyTerminates());
}

#if defined(ASIO_HAS_LOCAL_SOCKETS)
class UnixSocketServerTest : public SocketServerTest {
protected:
    std::unique_ptr<UnixSocketServer> server;

    SocketServer* createListeningServer() override {
        const std::string filename = std::filesystem::temp_directory_path() / randomString();
        server.reset(new UnixSocketServer(&protocolHandler));
        server->listen(filename);
        return server.get();
    }

    void destroyListeningServer() override {
        server.reset();
    }

private:
    std::random_device rd{};
    std::mt19937 gen{rd()};
    std::uniform_int_distribution<> distrib{0, 15};

    std::string randomString() {
        std::stringstream out{};
        for (std::size_t i = 0; i < 16; i++)
            out << std::hex << distrib(gen);
        return out.str();
    }
};

/*
 * Tests are flickering on OSX when testing without traffic flowing.
 *
 * This full lifecycle test is not optimal but it should be enough
 * given that the main difference between Unix and TCP is the socket
 * created at startup and removed on shutdown.
 */
TEST_F(UnixSocketServerTest, fullLifecycle) {
    asio::local::stream_protocol::endpoint socketName = server->listenEndpoint();
    EXPECT_CALL(protocolHandler, handle("X")).WillRepeatedly(Return("Y"));

    // socket created at startup
    ASSERT_TRUE(std::filesystem::exists(socketName.path()));

    // traffic flows
    asio::local::stream_protocol::iostream client(socketName);
    client << "X" << std::endl << std::flush;
    EXPECT_THAT(client, EventuallyReceives("Y"));

    // client disconnection terminates server
    client.close();
    EXPECT_THAT(serverThread, EventuallyTerminates());

    // socket removed by destructor
    TearDown();
    EXPECT_FALSE(std::filesystem::exists(socketName.path()));
}
#endif
