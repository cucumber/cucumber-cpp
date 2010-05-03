// GTest needs to be included before cukebins to register GTest tests
#include <gtest/gtest.h>
#include <cukebins/cukebins.hpp>

using namespace std;
using namespace cukebins::internal;

class GTestCommandsDouble : public GTestCommands {
public:
    bool isInitialized() {
        return initialized;
    }
};

class StepManagerDouble : public StepManager {
public:
    StepInfo::id_type getStepId(const string &stepName) {
        StepInfo::id_type stepId = 0;
        for (steps_type::const_iterator i = steps().begin(); i != steps().end(); ++i) {
            string testName = i->second.testName;
            string testFinalPart = testName.substr(testName.size() - stepName.size());
            if (testFinalPart == stepName) {
                stepId = i->second.id;
                break;
            }
        }
        return stepId;
    }
};

class GTestDriverTest : public ::testing::Test {
protected:
    GTestCommandsDouble cukeCommands;
    StepManagerDouble stepManager;
    shared_ptr<AbstractCommands::args_type> no_args;

    bool isRegisteredWithGTest(const string &stepName) {
        testing::UnitTest *gTestRunner = testing::UnitTest::GetInstance();
        for (int i = 0; i < gTestRunner->total_test_case_count(); ++i) {
            const testing::TestCase *testCase = gTestRunner->GetTestCase(i);
            for (int j = 0; j < testCase->total_test_count(); ++j) {
                const testing::TestInfo* testInfo = testCase->GetTestInfo(j);
                if (stepName == testInfo->name()) {
                    return true;
                }
            }
        }
        return false;
    }

    bool gTestIsReady() {
        return cukeCommands.isInitialized();
    }

    StepInfo::id_type getStepId(const string &stepName) {
        return stepManager.getStepId(stepName);
    }
};

CUKE_CONTEXT(EmptyContext) {};
GIVEN(EmptyContext, GivenStep, "given matcher") {}
WHEN(EmptyContext, WhenStep, "when matcher") {}
THEN(EmptyContext, ThenStep, "then matcher") {}

TEST_F(GTestDriverTest, gTestRegistersSteps) {
    EXPECT_TRUE(isRegisteredWithGTest(CUKE_TEST_REAL_NAME_(GivenStep)));
    EXPECT_TRUE(isRegisteredWithGTest(CUKE_TEST_REAL_NAME_(WhenStep)));
    EXPECT_TRUE(isRegisteredWithGTest(CUKE_TEST_REAL_NAME_(ThenStep)));
}

/*
 * We cannot really test if GTest has been initialized because
 * we use GTest and because the function is not exported
 */
TEST_F(GTestDriverTest, beginScenarioInitsGTest) {
    cukeCommands.beginScenario();
    EXPECT_TRUE(gTestIsReady());
}

THEN(EmptyContext, ShouldSucceed, "Succeed!") {
    ASSERT_TRUE(true);
}
THEN(EmptyContext, ShouldFail, "Fail!") {
    ASSERT_TRUE(false);
}

TEST_F(GTestDriverTest, invokeRunsTests) {
    EXPECT_TRUE(cukeCommands.invoke(getStepId("ShouldSucceed"), no_args).success);
    EXPECT_FALSE(cukeCommands.invoke(42, no_args).success);
    // There are some problems driving GTest with itself
    //EXPECT_FALSE(cukeCommands.invoke(getStepId("ShouldFail"), no_args).success);
}
