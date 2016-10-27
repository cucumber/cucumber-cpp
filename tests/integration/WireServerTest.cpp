#include <cucumber-cpp/internal/connectors/wire/WireServer.hpp>

#include <gmock/gmock.h>

#include <boost/filesystem/operations.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/timer.hpp>

#include <stdlib.h>
#include <sstream>

using namespace cucumber::internal;
using namespace boost::posix_time;
using namespace boost::asio::ip;
#if defined(BOOST_ASIO_HAS_LOCAL_SOCKETS)
using namespace boost::asio::local;
#endif
using namespace std;
using namespace testing;
using boost::bind;
using boost::thread;
namespace fs = boost::filesystem;

static const time_duration THREAD_TEST_TIMEOUT = milliseconds(4000);

MATCHER(IsConnected, string(negation ? "is not" : "is") +
        " connected") { return arg.good(); }

MATCHER(HasTerminated, "") {
    return !arg.joinable();
}

MATCHER(EventuallyTerminates, "") {
    return arg->timed_join(THREAD_TEST_TIMEOUT);
}

MATCHER_P(EventuallyReceives, value, "") {
    std::basic_iostream<char> *stream = const_cast<std::basic_iostream<char> *>(
            static_cast<const std::basic_iostream<char> *>(&arg));
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
    MOCK_CONST_METHOD1(handle, string(const string &request));
};

class SocketServerTest : public Test {

protected:
    StrictMock<MockProtocolHandler> protocolHandler;
    boost::scoped_ptr<thread> serverThread;

    virtual void SetUp() {
        SocketServer* server = createListeningServer();
        serverThread.reset(new thread(&SocketServer::acceptOnce, server));
    }

    virtual void TearDown() {
        if (serverThread) {
            serverThread->timed_join(THREAD_TEST_TIMEOUT);
            serverThread.reset();
        }
        destroyListeningServer();
    }

    virtual SocketServer* createListeningServer() = 0;
    virtual void destroyListeningServer() = 0;
};

class TCPSocketServerTest : public SocketServerTest {
protected:
    boost::scoped_ptr<TCPSocketServer> server;

    virtual SocketServer* createListeningServer() {
        server.reset(new TCPSocketServer(&protocolHandler));
        server->listen(0);
        return server.get();
    }

    virtual void destroyListeningServer() {
        server.reset();
    }
};

TEST_F(TCPSocketServerTest, exitsOnFirstConnectionClosed) {
    // given
    tcp::iostream client(server->listenEndpoint());
    ASSERT_THAT(client, IsConnected());

    // when
    client.close();

    // then
    EXPECT_THAT(serverThread, EventuallyTerminates());
}

TEST_F(TCPSocketServerTest, moreThanOneClientCanConnect) {
    // given
    tcp::iostream client1(server->listenEndpoint());
    ASSERT_THAT(client1, IsConnected());

    // when
    tcp::iostream client2(server->listenEndpoint());

    //then
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
    tcp::iostream client(server->listenEndpoint());
    ASSERT_THAT(client, IsConnected());

    // when
    client << "1" << flush << "2" << endl << flush;
    client << "3" << endl << "4" << endl << flush;

    // then
    EXPECT_THAT(client, EventuallyReceives("A"));
    EXPECT_THAT(client, EventuallyReceives("B"));
    EXPECT_THAT(client, EventuallyReceives("C"));
}

#if defined(BOOST_ASIO_HAS_LOCAL_SOCKETS)
class UnixSocketServerTest : public SocketServerTest {
protected:
    boost::scoped_ptr<UnixSocketServer> server;

    virtual SocketServer* createListeningServer() {
        fs::path socket = fs::temp_directory_path() / fs::unique_path();
        server.reset(new UnixSocketServer(&protocolHandler));
        server->listen(socket.string());
        return server.get();
    }

    virtual void destroyListeningServer() {
        server.reset();
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
    stream_protocol::endpoint socketName = server->listenEndpoint();
    EXPECT_CALL(protocolHandler, handle("X")).WillRepeatedly(Return("Y"));

    // socket created at startup
    ASSERT_TRUE(fs::exists(socketName.path()));

    // traffic flows
    stream_protocol::iostream client(socketName);
    client << "X" << endl << flush;
    EXPECT_THAT(client, EventuallyReceives("Y"));

    // client disconnection terminates server
    client.close();
    EXPECT_THAT(serverThread, EventuallyTerminates());

    // socket removed by destructor
    TearDown();
    EXPECT_FALSE(fs::exists(socketName.path()));
}
#endif
