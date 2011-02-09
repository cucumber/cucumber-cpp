#include <cukebins/cukebins.hpp>

// GTest needs to be included after cukebins to trigger the Null Framework
// registration behaviour. Otherwise it would register GTest tests!
#include <gtest/gtest.h>

using namespace cukebins::internal;

#define MANUAL_STEP_MATCHER "manual step"

class ManualStep : public FakeStep {
public:
    void stepBody() {};
private:
    static const int cukeRegId;
};
const int ManualStep::cukeRegId = ::cukebins::internal::registerStep<ManualStep>(MANUAL_STEP_MATCHER);

TEST(StepRegistrationTest, manualRegistration) {
    StepManager stepManager;
    EXPECT_TRUE(stepManager.stepMatches(MANUAL_STEP_MATCHER));
}

#define GIVEN_MATCHER "given matcher"
#define WHEN_MATCHER "when matcher"
#define THEN_MATCHER "then matcher"

GIVEN(GIVEN_MATCHER) {}
WHEN(WHEN_MATCHER) {}
THEN(THEN_MATCHER) {}

TEST(StepRegistrationTest, macroRegistration) {
    StepManager stepManager;
    EXPECT_TRUE(stepManager.stepMatches(GIVEN_MATCHER));
    EXPECT_TRUE(stepManager.stepMatches(WHEN_MATCHER));
    EXPECT_TRUE(stepManager.stepMatches(THEN_MATCHER));
}
