#include <gmock/gmock.h>

#include <cucumber-cpp/internal/drivers/GenericDriver.hpp>

using namespace cucumber::internal;
using namespace testing;

#define PENDING_STEP_DESCRIPTION "A description"

namespace {

class PendingStep : public GenericStep {
    void body() {
        pending();
    }
};

class PendingStepWithDescription : public GenericStep {
    void body() {
        pending(PENDING_STEP_DESCRIPTION);
    }
};

const char* embedding_src = "iVBORw0KGgoAAAANSUhEUgAAAAEAAAABCAYAAAAfFcSJAAAADUlEQVR42mP8z8BQDwAEhQ"
                            "GAhKmMIQAAAABJRU5ErkJggg==";
const char* embedding_mime = "image/png;base64";
const char* embedding_label = "Embedded image";

class SuccessfulStep : public GenericStep {
    void body() {
        embed(embedding_src, embedding_mime, embedding_label);
    }
};

class FailedStepThrowingException : public GenericStep {
    void body() {
        embed(embedding_src, embedding_mime, embedding_label);
        throw std::runtime_error("runtime_error");
    }
};

class FailedStepThrowingString : public GenericStep {
    void body() {
        embed(embedding_src, embedding_mime, embedding_label);
        throw std::string("string");
    }
};

class FailedStepThrowingCharacterArray : public GenericStep {
    void body() {
        embed(embedding_src, embedding_mime, embedding_label);
        throw(const char*)("char*");
    }
};

class Dummy {};

class FailedStepThrowingUnknown : public GenericStep {
    void body() {
        embed(embedding_src, embedding_mime, embedding_label);
        throw Dummy();
    }
};

static const InvokeArgs NO_INVOKE_ARGS;

} // anonymous namespace

TEST(BasicStepTest, handlesPendingSteps) {
    InvokeResult result;

    result = PendingStep().invoke(&NO_INVOKE_ARGS);
    ASSERT_EQ(result.getType(), PENDING);
    ASSERT_EQ(result.getDescription(), "");
    ASSERT_THAT(result.getEmbeddings(), IsEmpty());

    result = PendingStepWithDescription().invoke(&NO_INVOKE_ARGS);
    ASSERT_EQ(result.getType(), PENDING);
    ASSERT_EQ(result.getDescription(), PENDING_STEP_DESCRIPTION);
    ASSERT_THAT(result.getEmbeddings(), IsEmpty());
}

TEST(BasicStepTest, handlesSuccessfulStep) {
    InvokeResult result;

    result = SuccessfulStep().invoke(&NO_INVOKE_ARGS);
    ASSERT_EQ(result.getType(), SUCCESS);
    ASSERT_EQ(result.getDescription(), "");
    ASSERT_THAT(result.getEmbeddings(),
                ElementsAre(Embedding(embedding_src, embedding_mime, embedding_label)));
}

TEST(BasicStepTest, handlesFailedStep) {
    InvokeResult result;

    result = FailedStepThrowingException().invoke(&NO_INVOKE_ARGS);
    ASSERT_EQ(result.getType(), FAILURE);
    ASSERT_EQ(result.getDescription(), "runtime_error");
    ASSERT_THAT(result.getEmbeddings(),
                ElementsAre(Embedding(embedding_src, embedding_mime, embedding_label)));

    result = FailedStepThrowingString().invoke(&NO_INVOKE_ARGS);
    ASSERT_EQ(result.getType(), FAILURE);
    ASSERT_EQ(result.getDescription(), "string");
    ASSERT_THAT(result.getEmbeddings(),
                ElementsAre(Embedding(embedding_src, embedding_mime, embedding_label)));

    result = FailedStepThrowingCharacterArray().invoke(&NO_INVOKE_ARGS);
    ASSERT_EQ(result.getType(), FAILURE);
    ASSERT_EQ(result.getDescription(), "char*");
    ASSERT_THAT(result.getEmbeddings(),
                ElementsAre(Embedding(embedding_src, embedding_mime, embedding_label)));

    result = FailedStepThrowingUnknown().invoke(&NO_INVOKE_ARGS);
    ASSERT_EQ(result.getType(), FAILURE);
    ASSERT_EQ(result.getDescription(), "Unknown exception");
    ASSERT_THAT(result.getEmbeddings(),
                ElementsAre(Embedding(embedding_src, embedding_mime, embedding_label)));
}
