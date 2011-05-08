#include <gtest/gtest.h>

#include <cukebins/internal/drivers/FakeDriver.hpp>

using namespace cukebins::internal;

#define PENDING_STEP_DESCRIPTION "A description"

class PendingStep : public FakeStep {
    void body() {
        pending();
    }
};

class PendingStepWithDescription : public FakeStep {
    void body() {
        pending(PENDING_STEP_DESCRIPTION);
    }
};


TEST(BadicStepTest, handlesPendingSteps) {
    PendingStep pendingStep;
    PendingStepWithDescription pendingStepWithDescription;
    InvokeResult result;

    result = pendingStep.invoke(0);
    ASSERT_TRUE(result.isPending());

    result = pendingStepWithDescription.invoke(0);
    ASSERT_TRUE(result.isPending());
    ASSERT_STREQ(PENDING_STEP_DESCRIPTION, result.getDescription());
}
