#include <gtest/gtest.h>

#include <cukebins/internal/CukeCommands.hpp>
#include <cukebins/internal/Macros.hpp>
#include <cukebins/internal/drivers/FakeDriver.hpp>

#include "../utils/StepManagerTestDouble.hpp"

#include <boost/shared_ptr.hpp>

using namespace cukebins::internal;

using std::string;
using boost::shared_ptr;

class CheckAllParameters : public FakeStep {
private:
    static const int arg_0_int;
    static const double arg_1_double;
    static const string arg_2_string;

public:
    static shared_ptr<command_args_type> buildInvokeArgs() {
        shared_ptr<command_args_type> mixed_args(new command_args_type);
        mixed_args->push_back(toString(arg_0_int));
        mixed_args->push_back(toString(arg_1_double));
        mixed_args->push_back(toString(arg_2_string));
        return mixed_args;
    }

    /* tests */

    void stepBody() {
        REGEX_PARAM(int, got_arg_0_int);
        EXPECT_EQ(arg_0_int, got_arg_0_int);
        EXPECT_EQ((double)arg_0_int, getInvokeArg<double>(0));
        EXPECT_NO_THROW(getInvokeArg<string>(0));

        REGEX_PARAM(double, got_arg_1_double);
        EXPECT_EQ((int)arg_1_double, getInvokeArg<int>(1));
        EXPECT_EQ(arg_1_double, got_arg_1_double);
        EXPECT_NO_THROW(getInvokeArg<string>(1));

        REGEX_PARAM(string, got_arg_2_string);
        EXPECT_THROW(getInvokeArg<int>(2), std::invalid_argument);
        EXPECT_THROW(getInvokeArg<double>(2), std::invalid_argument);
        EXPECT_EQ(arg_2_string, got_arg_2_string);
    }
};

const int CheckAllParameters::arg_0_int(42);
const double CheckAllParameters::arg_1_double(4.2);
const string CheckAllParameters::arg_2_string("fortytwo");

const string MATCHER("MATCHER");

class CukeCommandsTest : public ::testing::Test {
    StepManagerTestDouble stepManager;

protected:
    CukeCommands cukeCommands;
    shared_ptr<StepInfo> stepInfoPtr;

    virtual void SetUp() {
        stepInfoPtr = shared_ptr<StepInfo>(new StepInvoker<CheckAllParameters>(MATCHER));
        stepManager.addStep(stepInfoPtr.get());
    }

    virtual void TearDown() {
        stepManager.clearSteps();
    }
};

TEST_F(CukeCommandsTest, matchesCorrectly) {
    MatchResult result = cukeCommands.stepMatches(MATCHER);
    EXPECT_EQ(stepInfoPtr->id, result.getResultSet().at(0).id);
}

TEST_F(CukeCommandsTest, invokeHandlesParameters) {
    // The real test is in CheckAllParameters::stepBody()
    cukeCommands.invoke(stepInfoPtr->id, CheckAllParameters::buildInvokeArgs());
}
