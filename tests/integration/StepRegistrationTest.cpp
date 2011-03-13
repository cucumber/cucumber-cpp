#include <gtest/gtest.h>

#include <cukebins/internal/step/StepManager.hpp>
#include <cukebins/internal/step/StepMacros.hpp>
#include <cukebins/internal/drivers/FakeDriver.hpp>

using namespace cukebins::internal;

#define MANUAL_STEP_MATCHER "manual step"

class ManualStep : public FakeStep {
public:
    void body() {};
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
