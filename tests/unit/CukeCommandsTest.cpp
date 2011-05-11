#include <gtest/gtest.h>

#include <cukebins/internal/step/StepMacros.hpp>
#include "../utils/CukeCommandsFixture.hpp"

using namespace cukebins::internal;

using std::string;
using boost::shared_ptr;

class CukeCommandsTest : public CukeCommandsFixture {
protected:
    void addStepWithMatcher(const std::string &matcher) {
        addStepToManager<EmptyStep>(matcher);
    }
};

class CheckAllParameters : public FakeStep {
protected:
    static const int arg_0_int;
    static const double arg_1_double;
    static const string arg_2_string;
    static const string arg_3_string_with_spaces;

public:
    static shared_ptr<command_args_type> buildInvokeArgs() {
        shared_ptr<command_args_type> mixed_args(new command_args_type);
        mixed_args->push_back(toString(arg_0_int));
        mixed_args->push_back(toString(arg_1_double));
        mixed_args->push_back(toString(arg_2_string));
        mixed_args->push_back(toString(arg_3_string_with_spaces));
        return mixed_args;
    }
};

const int CheckAllParameters::arg_0_int(42);
const double CheckAllParameters::arg_1_double(4.2);
const string CheckAllParameters::arg_2_string("fortytwo");
const string CheckAllParameters::arg_3_string_with_spaces("forty two");

class CheckAllParametersWithoutMacro : public CheckAllParameters {
private:
    int int_arg;
    double double_arg;
    string string_arg;

public:
    void body() {
        EXPECT_EQ(arg_0_int, getInvokeArg<int>(0));
        EXPECT_EQ((double)arg_0_int, getInvokeArg<double>(0));
        EXPECT_NO_THROW(getInvokeArg<string>(0));

        EXPECT_EQ((int)arg_1_double, getInvokeArg<int>(1));
        EXPECT_EQ(arg_1_double, getInvokeArg<double>(1));
        EXPECT_NO_THROW(getInvokeArg<string>(1));

        EXPECT_THROW(getInvokeArg<int>(2), std::invalid_argument);
        EXPECT_THROW(getInvokeArg<double>(2), std::invalid_argument);
        EXPECT_EQ(arg_2_string, getInvokeArg<string>(2));

        EXPECT_THROW(getInvokeArg<int>(3), std::invalid_argument);
        EXPECT_THROW(getInvokeArg<double>(3), std::invalid_argument);
        EXPECT_EQ(arg_3_string_with_spaces, getInvokeArg<string>(3));
    }
};

class CheckAllParametersWithMacro : public CheckAllParameters {
public:
    void body() {
        REGEX_PARAM(int, got_arg_0_int);
        EXPECT_EQ(arg_0_int, got_arg_0_int);

        REGEX_PARAM(double, got_arg_1_double);
        EXPECT_EQ(arg_1_double, got_arg_1_double);

        REGEX_PARAM(string, got_arg_2_string);
        EXPECT_EQ(arg_2_string, got_arg_2_string);

        REGEX_PARAM(string, got_arg_3_string_with_spaces);
        EXPECT_EQ(arg_3_string_with_spaces, got_arg_3_string_with_spaces);
    }
};

TEST_F(CukeCommandsTest, matchesCorrectly) {
    addStepWithMatcher(STATIC_MATCHER);
    MatchResult result = cukeCommands.stepMatches(STATIC_MATCHER);
    EXPECT_EQ(stepInfoPtr->id, result.getResultSet().at(0).stepInfo->id);
}

TEST_F(CukeCommandsTest, invokeHandlesParametersWithoutMacro) {
    // The real test is in CheckAllParameters::body()
    runStepBodyTest<CheckAllParametersWithoutMacro>();
}

TEST_F(CukeCommandsTest, invokeHandlesParametersWithMacro) {
    // The real test is in CheckAllParameters::body()
    runStepBodyTest<CheckAllParametersWithMacro>();
}
