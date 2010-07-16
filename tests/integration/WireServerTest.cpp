#include <cukebins/cukebins.hpp>
#include <cukebins/internal/connectors/wire/WireServer.hpp>

#include <gtest/gtest.h>

#include "../utils/StepManagerTestDouble.hpp"

#include <boost/thread.hpp>
#include <boost/timer.hpp>
#include <boost/interprocess/detail/atomic.hpp>

#include <limits>
#include <sstream>

using namespace cukebins::internal;
using namespace boost::posix_time;
using namespace std;

class InputConsumerProtocol : public NetworkProtocol {
public:
    void processStream(iostream &stream) {
        while (stream) {
            stream.ignore(std::numeric_limits<std::streamsize>::max());
        }
    }
};

class EchoProtocol : public NetworkProtocol {
public:
    void processStream(iostream &stream) {
        string request;
        while (stream) {
            stream >> request;
            stream << request << std::endl << std::flush;
        }
    }
};

class SocketServerTest : public ::testing::Test {

protected:
    static const unsigned short port = 54321;
    static const long longTimeout = 4000;
    static const long shortTimeout = 500;

    void waitForOutput(ip::tcp::iostream &stream, string &output, long millis) {
        double seconds = millis / 1000.0;
        boost::timer timer;
        while (timer.elapsed() < seconds) {
            if (stream.rdbuf()->available() > 0) {
                stream >> output;
                break;
            }
            boost::this_thread::yield();
        }
    }
};

TEST_F(SocketServerTest, exitsOnFirstConnectionClosed) {
    SocketServer<InputConsumerProtocol> server(port);
    boost::thread serverThread(boost::bind(&SocketServer<InputConsumerProtocol>::accept, &server));

    ip::tcp::iostream client(ip::tcp::endpoint(ip::tcp::v4(), port));
    client.close();

    serverThread.timed_join(milliseconds(longTimeout));
    EXPECT_FALSE(serverThread.joinable());
}

TEST_F(SocketServerTest, receiveAndSendsSingleLineMassages) {
    string request("Echo"), response;

    SocketServer<EchoProtocol> server(port);
    boost::thread serverThread(boost::bind(&SocketServer<EchoProtocol>::accept, &server));

    ip::tcp::iostream client(ip::tcp::endpoint(ip::tcp::v4(), port));
    client << request << std::endl << std::flush;
    waitForOutput(client, response, shortTimeout);
    EXPECT_EQ(request, response);
    client.close();

    serverThread.timed_join(milliseconds(longTimeout));
    EXPECT_FALSE(serverThread.joinable());
}

class JSONCommandsTest : public ::testing::Test {
    WireProtocol wireProtocol;
protected:
    static string success;
    static string fail;
    static string empty;

    StepManagerTestDouble stepManager;

    string processCommand(string input) {
        stringstream stream;
        string output;
        stream << input << endl;
        wireProtocol.processOneRequest(stream);
        stream >> output;
        return output;
    }
    void TearDown() {
        stepManager.clearSteps();
    }
};
string JSONCommandsTest::success = "[\"success\"]";
string JSONCommandsTest::fail = "[\"fail\"]";
string JSONCommandsTest::empty = "";

TEST_F(JSONCommandsTest, handlesBeginScenario) {
    EXPECT_EQ(success, processCommand("[\"begin_scenario\"]"));
    EXPECT_EQ(success, processCommand("[\"begin_scenario\",null]"));

    EXPECT_EQ(fail, processCommand("[\"begin_scenario\",\"something else\"]"));
}

TEST_F(JSONCommandsTest, handlesEndScenario) {
    EXPECT_EQ(success, processCommand("[\"end_scenario\"]"));
    EXPECT_EQ(success, processCommand("[\"end_scenario\",null]"));

    EXPECT_EQ(fail, processCommand("[\"end_scenario\",\"something else\"]"));
}

TEST_F(JSONCommandsTest, handlesStepMatches) {
    stepManager.addStepDefinitionWithId(100, "match the number (\\d+)");
    EXPECT_EQ("[\"success\",[{\"args\":[{\"pos\":17,\"val\":\"1\"}],\"id\":\"100\"}]]", processCommand("[\"step_matches\",{\"name_to_match\":\"match the number 1\"}]"));
    EXPECT_EQ("[\"success\",[]]", processCommand("[\"step_matches\",{\"name_to_match\":\"match the number one\"}]"));

    EXPECT_EQ(fail, processCommand("[\"step_matches\"]"));
    EXPECT_EQ(fail, processCommand("[\"step_matches\",null]"));
    EXPECT_EQ(fail, processCommand("[\"step_matches\",{}]"));
    EXPECT_EQ(fail, processCommand("[\"step_matches\",42]"));
}

TEST_F(JSONCommandsTest, handlesInvoke) {
    stepManager.addStepDefinitionWithId(101, "match the numbers (\\d+) and (\\d+)");
    EXPECT_EQ(success, processCommand("[\"invoke\",{\"args\":[\"27\",\"72\"],\"id\":\"101\"}]"));

    EXPECT_EQ(fail, processCommand("[\"invoke\"]"));
    EXPECT_EQ(fail, processCommand("[\"invoke\",null]"));
    EXPECT_EQ(fail, processCommand("[\"invoke\",{\"id\":\"42\"}]"));
    EXPECT_EQ(fail, processCommand("[\"invoke\",{\"id\":\"not_a_number\"}]"));
}

TEST_F(JSONCommandsTest, doesNotHandleSnippetText) {
    EXPECT_EQ(fail, processCommand("[\"snippet_text\",{\"step_keyword\":\"Given\",\"multiline_arg_class\":\"\",\"step_name\":\"name\"}]"));

    EXPECT_EQ(fail, processCommand("[\"snippet_text\"]"));
    EXPECT_EQ(fail, processCommand("[\"snippet_text\",null]"));
}

TEST_F(JSONCommandsTest, failsGracefullyOnUnknownOrMalformed) {
    EXPECT_EQ(fail, processCommand("[\"unknown_command\"]"));

    EXPECT_EQ(empty, processCommand(""));
    EXPECT_EQ(empty, processCommand("{\"malformed_command\"}"));
    EXPECT_EQ(fail, processCommand("[42]"));
}
