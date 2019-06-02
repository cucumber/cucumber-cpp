#include <cucumber-cpp/internal/connectors/wire/WireProtocol.hpp>
#include <cucumber-cpp/internal/connectors/wire/WireProtocolCommands.hpp>

#include <gmock/gmock.h>
#include <boost/assign/list_of.hpp>
#include <boost/shared_ptr.hpp>

#include <typeinfo>

using namespace cucumber::internal;
using namespace std;
using namespace testing;

using boost::assign::list_of;

namespace cucumber {
namespace internal {

void PrintTo(const Embedding& embedding, std::ostream* os) {
    *os << "{src: \"" << embedding.src << "\"; mime_type: \"" << embedding.mime << "\"; label: \""
        << embedding.label << "\"}";
}

bool operator==(const Embedding& lhs, const Embedding& rhs) {
    return lhs.src == rhs.src && lhs.mime == rhs.mime && lhs.label == rhs.label;
}

} // namespace internal
} // namespace cucumber

class MockCukeEngine : public CukeEngine {
public:
    MOCK_CONST_METHOD1(stepMatches, std::vector<StepMatch>(const std::string & name));
    MOCK_METHOD1(endScenario, void(const tags_type & tags));
    MOCK_METHOD3(invokeStep,
                 InvokeResult(const std::string& id,
                              const invoke_args_type& args,
                              const invoke_table_type& tableArg));
    MOCK_METHOD1(beginScenario, void(const tags_type & tags));
    MOCK_CONST_METHOD3(snippetText, std::string(const std::string & keyword, const std::string & name, const std::string & multilineArgClass));
};

#define EXPECT_PTRTYPE(classname, expression) \
    EXPECT_NE(dynamic_cast<const classname*>(expression), (void*)NULL)

class WireMessageCodecTest : public Test {
public:
    WireMessageCodecTest() {};

protected:
    boost::shared_ptr<WireCommand> commandAutoPtr;

    WireCommand& decode(const char *jsonStr) {
        commandAutoPtr = codec.decode(jsonStr);
        return *commandAutoPtr;
    }

    std::string encode(const WireResponse& response) const {
        return codec.encode(response);
    }

protected:
    const JsonSpiritWireMessageCodec codec;
};


/*
 * Request decoding
 */

TEST_F(WireMessageCodecTest, decodesUnknownOrMalformedMessage) {
    MockCukeEngine engine;

    EXPECT_EQ(encode(*decode("[\"unknown_command\"]").run(engine)), "[\"fail\"]");
    EXPECT_EQ(encode(*decode("").run(engine)), "[\"fail\"]");
    EXPECT_EQ(encode(*decode("rubbish").run(engine)), "[\"fail\"]");
    EXPECT_EQ(encode(*decode("{\"malformed_command\"}").run(engine)), "[\"fail\"]");
    EXPECT_EQ(encode(*decode("[42]").run(engine)), "[\"fail\"]");
}

TEST_F(WireMessageCodecTest, handlesStepMatchesMessage) {
    MockCukeEngine engine;
    EXPECT_CALL(engine, stepMatches("name to match"))
            .Times(1)
            .WillRepeatedly(Return(std::vector<StepMatch>(0)));

    decode("[\"step_matches\","
           "{\"name_to_match\":\"name to match\"}]")
            .run(engine);
}

TEST_F(WireMessageCodecTest, handlesBeginScenarioMessageWithoutArgument) {
    MockCukeEngine engine;
    EXPECT_CALL(engine, beginScenario(ElementsAre())).Times(1);

    decode("[\"begin_scenario\"]").run(engine);
}

TEST_F(WireMessageCodecTest, handlesBeginScenarioMessageWithTagsArgument) {
    MockCukeEngine engine;
    EXPECT_CALL(engine, beginScenario(ElementsAre("bar","baz","foo"))).Times(1);

    decode("[\"begin_scenario\","
           "{\"tags\":["
                "\"bar\","
                "\"baz\","
                "\"foo\""
           "]}]").run(engine);
}

TEST_F(WireMessageCodecTest, handlesBeginScenarioMessageWithNullArgument) {
    MockCukeEngine engine;
    EXPECT_CALL(engine, beginScenario(ElementsAre())).Times(1);

    decode("[\"begin_scenario\",null]").run(engine);
}

TEST_F(WireMessageCodecTest, handlesInvokeMessageWithNoArgs) {
    MockCukeEngine engine;
    EXPECT_CALL(engine, invokeStep("42", ElementsAre(), ElementsAre())).Times(1);

    decode("[\"invoke\",{\"id\":\"42\",\"args\":[]}]").run(engine);
}

TEST_F(WireMessageCodecTest, handlesInvokeMessageWithoutTableArgs) {
    MockCukeEngine engine;
    EXPECT_CALL(engine, invokeStep("42", ElementsAre("p1","p2","p3"), ElementsAre())).Times(1);

    decode("[\"invoke\",{"
           "\"id\":\"42\","
           "\"args\":["
                "\"p1\","
                "\"p2\","
                "\"p3\""
           "}]").run(engine);
}

TEST_F(WireMessageCodecTest, handlesInvokeMessageWithTableArgs) {
    MockCukeEngine engine;
    EXPECT_CALL(engine, invokeStep(
            "42",
            ElementsAre("p1"),
            ElementsAre(
                ElementsAre("col1","col2"),
                ElementsAre("r1c1","r1c2"),
                ElementsAre("r2c1","r2c2"),
                ElementsAre("r3c1","r3c2")
            )
        )).Times(1);

    decode("[\"invoke\",{"
           "\"id\":\"42\","
           "\"args\":["
                "\"p1\","
                "["
                    "[\"col1\",\"col2\"],"
                    "[\"r1c1\",\"r1c2\"],"
                    "[\"r2c1\",\"r2c2\"],"
                    "[\"r3c1\",\"r3c2\"]"
                "]"
           "}]").run(engine);
}

TEST_F(WireMessageCodecTest, handlesInvokeMessageWithNullArg) {
    MockCukeEngine engine;
    EXPECT_CALL(engine, invokeStep("42", ElementsAre(), ElementsAre())).Times(1);

    decode("[\"invoke\",{\"id\":\"42\",\"args\":[null]}]").run(engine);
}

TEST_F(WireMessageCodecTest, handlesEndScenarioMessageWithoutArgument) {
    MockCukeEngine engine;
    EXPECT_CALL(engine, endScenario(ElementsAre())).Times(1);

    decode("[\"end_scenario\"]").run(engine);
}

TEST_F(WireMessageCodecTest, handlesEndScenarioMessageWithNullArgument) {
    MockCukeEngine engine;
    EXPECT_CALL(engine, endScenario(ElementsAre())).Times(1);

    decode("[\"end_scenario\",null]").run(engine);
}

TEST_F(WireMessageCodecTest, handlesEndScenarioMessageWithTagsArgument) {
    MockCukeEngine engine;
    EXPECT_CALL(engine, endScenario(ElementsAre("cu","cum","ber"))).Times(1);

    decode("[\"end_scenario\","
           "{\"tags\":["
                "\"cu\","
                "\"cum\","
                "\"ber\""
           "]}]").run(engine);
}

TEST_F(WireMessageCodecTest, handlesSnippetTextMessage) {
    MockCukeEngine engine;
    EXPECT_CALL(engine, snippetText("Keyword", "step description","Some::Class")).Times(1);

    decode("[\"snippet_text\","
           "{\"step_keyword\":\"Keyword\","
             "\"multiline_arg_class\":\"Some::Class\","
             "\"step_name\":\"step description\"}]")
            .run(engine);
}

/*
 * Response encoding
 */

TEST_F(WireMessageCodecTest, handlesSimpleSuccessResponse) {
    SuccessResponse response;
    EXPECT_THAT(codec.encode(response), StrEq("[\"success\"]"));
}

TEST_F(WireMessageCodecTest, handlesDetailedSuccessResponse) {
    Embedding embedding1("Some text", "text/plain", "Embedded text");
    Embedding embedding2("iVBORw0KGgoAAAANSUhEUgAAAAEAAAABCAYAAAAfFcSJAAAADUlEQVR42mP8z8BQDwAEhQGAh"
                         "KmMIQAAAABJRU5ErkJggg==",
                         "image/png;base64",
                         "Embedded image");
    std::vector<Embedding> embeddings = list_of(embedding1)(embedding2);

    SuccessResponse response(embeddings);
    // clang-format off
    EXPECT_THAT(codec.encode(response), StrEq(
            "[\"success\",{"
                "\"embeddings\":["
                    "{\"label\":\"Embedded text\",\"mime_type\":\"text/plain\",\"src\":\"Some text\"},"
                    "{\"label\":\"Embedded image\",\"mime_type\":\"image/png;base64\",\"src\":\"iVBORw0KGgoAAAANSUhEUgAAAAEAAAABCAYAAAAfFcSJAAAADUlEQVR42mP8z8BQDwAEhQGAhKmMIQAAAABJRU5ErkJggg==\"}]"
            "}]"));
    // clang-format on
}

TEST_F(WireMessageCodecTest, handlesSimpleFailureResponse) {
    FailureResponse response;
    EXPECT_THAT(codec.encode(response), StrEq("[\"fail\"]"));
}

TEST_F(WireMessageCodecTest, handlesDetailedFailureResponse) {
    Embedding embedding1("Some text", "text/plain", "Embedded text");
    Embedding embedding2("iVBORw0KGgoAAAANSUhEUgAAAAEAAAABCAYAAAAfFcSJAAAADUlEQVR42mP8z8BQDwAEhQGAh"
                         "KmMIQAAAABJRU5ErkJggg==",
                         "image/png;base64",
                         "Embedded image");
    std::vector<Embedding> embeddings = list_of(embedding1)(embedding2);

    FailureResponse response("My message", "ExceptionClassName", embeddings);
    // clang-format off
    EXPECT_THAT(codec.encode(response), StrEq(
                "[\"fail\",{"
                    "\"embeddings\":["
                        "{\"label\":\"Embedded text\",\"mime_type\":\"text/plain\",\"src\":\"Some text\"},"
                        "{\"label\":\"Embedded image\",\"mime_type\":\"image/png;base64\",\"src\":\"iVBORw0KGgoAAAANSUhEUgAAAAEAAAABCAYAAAAfFcSJAAAADUlEQVR42mP8z8BQDwAEhQGAhKmMIQAAAABJRU5ErkJggg==\"}],"
                    "\"exception\":\"ExceptionClassName\","
                    "\"message\":\"My message\""
                "}]"));
    // clang-format off
}

TEST_F(WireMessageCodecTest, handlesPendingResponse) {
    PendingResponse response("I'm lazy!");
    EXPECT_THAT(codec.encode(response), StrEq("[\"pending\",\"I'm lazy!\"]"));
}

TEST_F(WireMessageCodecTest, handlesEmptyStepMatchesResponse) {
    StepMatchesResponse response(std::vector<StepMatch>(0));
    EXPECT_THAT(codec.encode(response), StrEq("[\"success\",[]]"));
}

TEST_F(WireMessageCodecTest, handlesStepMatchesResponse) {
    std::vector<StepMatch> matches;
    StepMatch sm1;
    sm1.id = "1234";
    sm1.source = "MyClass.cpp:56";
    sm1.regexp = "Some (.*) regexp";
    matches.push_back(sm1);
    StepMatch sm2;
    sm2.id = "9876";
    StepMatchArg sm2arg1;
    sm2arg1.value = "odd";
    sm2arg1.position = 5;
    sm2.args.push_back(sm2arg1);
    matches.push_back(sm2);
    StepMatchesResponse response(matches);

    EXPECT_THAT(codec.encode(response), StrEq(
            "[\"success\",[{"
                "\"args\":[],"
                "\"id\":\"1234\","
                "\"regexp\":\"Some (.*) regexp\","
                "\"source\":\"MyClass.cpp:56\""
            "},{"
                "\"args\":[{"
                    "\"pos\":5,"
                    "\"val\":\"odd\""
                "}],"
                "\"id\":\"9876\""
            "}]]"));
}

TEST_F(WireMessageCodecTest, handlesSnippetTextResponse) {
    SnippetTextResponse response("GIVEN(...)");
    EXPECT_THAT(codec.encode(response), StrEq("[\"success\",\"GIVEN(...)\"]"));
}

/*
 * Command response
 */

TEST(WireCommandsTest, succesfulInvokeReturnsSuccess) {
    MockCukeEngine engine;
    InvokeCommand invokeCommand("x", CukeEngine::invoke_args_type(), CukeEngine::invoke_table_type());
    EXPECT_CALL(engine, invokeStep(_, _, _)).Times(1).WillOnce(Return(InvokeResult::success()));

    boost::shared_ptr<const WireResponse> response(invokeCommand.run(engine));
    EXPECT_PTRTYPE(SuccessResponse, response.get());
    EXPECT_THAT(static_cast<const SuccessResponse&>(*response).getEmbeddings(), IsEmpty());
}

TEST(WireCommandsTest, succesfulInvokeWithEmbeddingsReturnsSuccess) {
    Embedding embedding1("Some text", "text/plain", "Embedded text");
    Embedding embedding2("iVBORw0KGgoAAAANSUhEUgAAAAEAAAABCAYAAAAfFcSJAAAADUlEQVR42mP8z8BQDwAEhQGAh"
                         "KmMIQAAAABJRU5ErkJggg==",
                         "image/png;base64",
                         "Embedded image");
    std::vector<Embedding> embeddings = list_of(embedding1)(embedding2);

    MockCukeEngine engine;
    InvokeCommand invokeCommand(
        "x", CukeEngine::invoke_args_type(), CukeEngine::invoke_table_type());
    EXPECT_CALL(engine, invokeStep(_, _, _))
        .Times(1)
        .WillOnce(Return(InvokeResult::success(embeddings)));

    boost::shared_ptr<const WireResponse> response(invokeCommand.run(engine));
    EXPECT_PTRTYPE(SuccessResponse, response.get());
    EXPECT_THAT(static_cast<const SuccessResponse&>(*response).getEmbeddings(),
                ElementsAre(embedding1, embedding2));
}

TEST(WireCommandsTest, failingInvokeReturnsFailure) {
    MockCukeEngine engine;
    InvokeCommand invokeCommand("x", CukeEngine::invoke_args_type(), CukeEngine::invoke_table_type());
    EXPECT_CALL(engine, invokeStep(_, _, _)).Times(1).WillOnce(Return(InvokeResult::failure("A")));

    boost::shared_ptr<const WireResponse> response(invokeCommand.run(engine));
    EXPECT_PTRTYPE(FailureResponse, response.get());
    EXPECT_EQ(static_cast<const FailureResponse&>(*response).getMessage(), "A");
    EXPECT_EQ(static_cast<const FailureResponse&>(*response).getExceptionType(), "");
    EXPECT_THAT(static_cast<const FailureResponse&>(*response).getEmbeddings(), IsEmpty());
}

TEST(WireCommandsTest, failingInvokeWithEmbeddingsReturnsFailure) {
    Embedding embedding1("Some text", "text/plain", "Embedded text");
    Embedding embedding2("iVBORw0KGgoAAAANSUhEUgAAAAEAAAABCAYAAAAfFcSJAAAADUlEQVR42mP8z8BQDwAEhQGAh"
                         "KmMIQAAAABJRU5ErkJggg==",
                         "image/png;base64",
                         "Embedded image");
    std::vector<Embedding> embeddings = list_of(embedding1)(embedding2);

    MockCukeEngine engine;
    InvokeCommand invokeCommand(
        "x", CukeEngine::invoke_args_type(), CukeEngine::invoke_table_type());
    EXPECT_CALL(engine, invokeStep(_, _, _))
        .Times(1)
        .WillOnce(Return(InvokeResult::failure("A", embeddings)));

    boost::shared_ptr<const WireResponse> response(invokeCommand.run(engine));
    EXPECT_PTRTYPE(FailureResponse, response.get());
    EXPECT_EQ(static_cast<const FailureResponse&>(*response).getMessage(), "A");
    EXPECT_EQ(static_cast<const FailureResponse&>(*response).getExceptionType(), "");
    EXPECT_THAT(static_cast<const FailureResponse&>(*response).getEmbeddings(),
                ElementsAre(embedding1, embedding2));
}

TEST(WireCommandsTest, pendingStepReturnsPending) {
    MockCukeEngine engine;
    InvokeCommand invokeCommand("x", CukeEngine::invoke_args_type(), CukeEngine::invoke_table_type());
    EXPECT_CALL(engine, invokeStep(_, _, _)).Times(1).WillOnce(Return(InvokeResult::pending("S")));

    boost::shared_ptr<const WireResponse> response(invokeCommand.run(engine));
    EXPECT_PTRTYPE(PendingResponse, response.get());
    EXPECT_EQ(static_cast<const PendingResponse&>(*response).getMessage(), "S");
}

TEST(WireCommandsTest, throwingInvokeExceptionReturnsFailure) {
    MockCukeEngine engine;
    InvokeCommand invokeCommand(
        "x", CukeEngine::invoke_args_type(), CukeEngine::invoke_table_type());
    EXPECT_CALL(engine, invokeStep(_, _, _)).Times(1).WillOnce(Throw(InvokeException("ex")));

    boost::shared_ptr<const WireResponse> response(invokeCommand.run(engine));
    EXPECT_PTRTYPE(FailureResponse, response.get());
    EXPECT_EQ(static_cast<const FailureResponse&>(*response).getMessage(), "ex");
    EXPECT_EQ(static_cast<const FailureResponse&>(*response).getExceptionType(), "InvokeException");
    EXPECT_THAT(static_cast<const FailureResponse&>(*response).getEmbeddings(), IsEmpty());
}

TEST(WireCommandsTest, throwingAnythingInvokeReturnsFailure) {
    MockCukeEngine engine;
    InvokeCommand invokeCommand("x", CukeEngine::invoke_args_type(), CukeEngine::invoke_table_type());
    EXPECT_CALL(engine, invokeStep(_, _, _))
            .Times(1)
            .WillOnce(Throw(string("something")));

    boost::shared_ptr<const WireResponse> response(invokeCommand.run(engine));
    EXPECT_PTRTYPE(FailureResponse, response.get());
    EXPECT_EQ(static_cast<const FailureResponse&>(*response).getMessage(), "");
    EXPECT_EQ(static_cast<const FailureResponse&>(*response).getExceptionType(), "");
    EXPECT_THAT(static_cast<const FailureResponse&>(*response).getEmbeddings(), IsEmpty());
}






