#include <gtest/gtest.h>

#include <cukebins/internal/PPMacros.hpp>
#include <cukebins/internal/CukeCommands.hpp>
#include <cukebins/internal/drivers/FakeDriver.hpp>

#include "../utils/InvokeTestHelper.hpp"

#include <boost/assign/list_of.hpp>

using namespace std;
using namespace cukebins::internal;

class CheckAllParameters : public InvokeTestHelper {
public:

    /* data setup */

    int arg_0_int;
    double arg_1_double;
    string arg_2_string;

    CheckAllParameters() {
        arg_0_int = 42;
        arg_1_double = 4.2;
        arg_2_string = "fortytwo";
    }

    args_type *buildInvokeArgs() {
        args_type *mixed_args = new args_type;
        mixed_args->push_back(toString(arg_0_int));
        mixed_args->push_back(toString(arg_1_double));
        mixed_args->push_back(toString(arg_2_string));
        return mixed_args;
    }

    /* tests */

    void testBeforeInvoke() {
    }

    void testOnInvoke() {
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

    void testAfterInvoke() {}
};

TEST(CukeCommandsTest, handlesParameters) {
    CukeCommandsTestInjector<CheckAllParameters> commands;
    commands.buildArgsAndInvoke();
}
