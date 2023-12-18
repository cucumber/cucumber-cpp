#include <gtest/gtest.h>

#include <cucumber-cpp/internal/step/StepManager.hpp>
#include <cucumber-cpp/internal/step/StepMacros.hpp>
#include <cucumber-cpp/internal/drivers/GenericDriver.hpp>

using namespace cucumber::internal;

#define MANUAL_STEP_MATCHER "manual step"

class ManualStep : public GenericStep {
public:
    void body() override{};
    // private:
    static const int cukeRegId;
};
const int ManualStep::cukeRegId = ::cucumber::internal::registerStep<ManualStep>(
    MANUAL_STEP_MATCHER, "C:\\Path\\With/Barward/And\\Forward/Slashes.cpp", 42
);

TEST(StepRegistrationTest, manualRegistration) {
    // static_cast is necessary for GTest <= 1.7 and C++ >= 2011 because it
    // doesn't support contextually-convertible-to-bool
    EXPECT_TRUE(static_cast<bool>(StepManager::stepMatches(MANUAL_STEP_MATCHER)));
    EXPECT_EQ("Slashes.cpp:42", StepManager::getStep(ManualStep::cukeRegId)->source);
}

#define GIVEN_MATCHER "given matcher"
#define WHEN_MATCHER "when matcher"
#define THEN_MATCHER "then matcher"

GIVEN(GIVEN_MATCHER) {
}
WHEN(WHEN_MATCHER) {
}
THEN(THEN_MATCHER) {
}

TEST(StepRegistrationTest, macroRegistration) {
    EXPECT_TRUE(static_cast<bool>(StepManager::stepMatches(GIVEN_MATCHER)));
    EXPECT_TRUE(static_cast<bool>(StepManager::stepMatches(WHEN_MATCHER)));
    EXPECT_TRUE(static_cast<bool>(StepManager::stepMatches(THEN_MATCHER)));
}
