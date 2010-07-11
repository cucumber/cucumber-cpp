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
    const StepInfo::id_type getStepId(const string &stepMatcher) {
        return getStepInfoFromMatcher(stepMatcher)->id;
    }

    const string getStepName(const string &stepMatcher) {
        return getStepInfoFromMatcher(stepMatcher)->testName;
    }
private:
    const StepInfo *getStepInfoFromMatcher(const string &stepMatcher) {
        for (steps_type::const_iterator i = steps().begin(); i != steps().end(); ++i) {
            if (i->second.regex.str() == stepMatcher) {
                return &(i->second);
            }
        }
    }
};

class GTestDriverTest : public ::testing::Test {
protected:
    GTestCommandsDouble cukeCommands;
    StepManagerDouble stepManager;
    shared_ptr<AbstractCommands::args_type> no_args;

    bool gTestIsReady() {
        return cukeCommands.isInitialized();
    }

    StepInfo::id_type getStepIdFromMatcher(const string &stepMatcher) {
        return stepManager.getStepId(stepMatcher);
    }

    bool isRegisteredWithGTest(const string &stepMatcher) {
        const string &stepName = getStepNameFromMatcher(stepMatcher);
        testing::UnitTest *gTestRunner = testing::UnitTest::GetInstance();
        for (int i = 0; i < gTestRunner->total_test_case_count(); ++i) {
            const testing::TestCase *testCase = gTestRunner->GetTestCase(i);
            for (int j = 0; j < testCase->total_test_count(); ++j) {
                const testing::TestInfo* testInfo = testCase->GetTestInfo(j);
                if (isSuffixOf(testInfo->name(), stepName)) {
                    return true;
                }
            }
        }
        return false;
    }

    bool isSuffixOf(const string &suffix, const string &str) {
        return equal(suffix.rbegin(), suffix.rend(), str.rbegin());
    }
private:
    const string getStepNameFromMatcher(const string &stepMatcher) {
        return stepManager.getStepName(stepMatcher);
    }
};


struct EmptyContext {};
GIVEN(EmptyContext, "given matcher") {}
WHEN(EmptyContext, "when matcher") {}
THEN(EmptyContext, "then matcher") {}

TEST_F(GTestDriverTest, gTestRegistersSteps) {
    EXPECT_TRUE(isRegisteredWithGTest("given matcher"));
    EXPECT_TRUE(isRegisteredWithGTest("when matcher"));
    EXPECT_TRUE(isRegisteredWithGTest("then matcher"));
}

/*
 * We cannot really test if GTest has been initialized because
 * we use GTest and because the function is not exported
 */
TEST_F(GTestDriverTest, beginScenarioInitsGTest) {
    cukeCommands.beginScenario();
    EXPECT_TRUE(gTestIsReady());
}

THEN(EmptyContext, "Succeed!") {
    ASSERT_TRUE(true);
}
//THEN(EmptyContext, "Fail!") {
//    ASSERT_TRUE(false);
//}

TEST_F(GTestDriverTest, invokeRunsTests) {
    EXPECT_TRUE(cukeCommands.invoke(getStepIdFromMatcher("Succeed!"), no_args).success);
    EXPECT_FALSE(cukeCommands.invoke(42, no_args).success);
    // There are some problems driving GTest with itself
    //EXPECT_FALSE(cukeCommands.invoke(getStepIdFromMatcher("Fail!"), no_args).success);
}
