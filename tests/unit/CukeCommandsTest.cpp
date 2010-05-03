#include <gtest/gtest.h>

#include <cukebins/internal/CukeCommands.hpp>
#include <cukebins/internal/drivers/FakeDriver.hpp>

#include "../utils/InvokeTestHelper.hpp"

#include <boost/assign/list_of.hpp>

using namespace std;
using namespace cukebins::internal;

class CheckAllParameters : public InvokeTestHelper {
public:

    /* data setup */

    int arg_1_int;
    double arg_2_double;
    string arg_3_string;

    CheckAllParameters() {
        arg_1_int = 42;
        arg_2_double = 4.2;
        arg_3_string = "fortytwo";
    }

    args_type *buildInvokeArgs() {
        args_type *mixed_args = new args_type;
        mixed_args->push_back(toString(arg_1_int));
        mixed_args->push_back(toString(arg_2_double));
        mixed_args->push_back(toString(arg_3_string));
        return mixed_args;
    }

    /* tests */

    void testBeforeInvoke() {
    }

    void testOnInvoke() {
        CUKE_PARAM(1, int, got_arg_1_int);
        EXPECT_EQ(arg_1_int, got_arg_1_int);
        EXPECT_EQ((double)arg_1_int, getInvokeArg<double>(1));
        EXPECT_NO_THROW(getInvokeArg<string>(1));

        CUKE_PARAM(2, double, got_arg_2_double);
        EXPECT_EQ((int)arg_2_double, getInvokeArg<int>(2));
        EXPECT_EQ(arg_2_double, got_arg_2_double);
        EXPECT_NO_THROW(getInvokeArg<string>(2));

        CUKE_PARAM(3, string, got_arg_3_string);
        EXPECT_THROW(getInvokeArg<int>(3), std::invalid_argument);
        EXPECT_THROW(getInvokeArg<double>(3), std::invalid_argument);
        EXPECT_EQ(arg_3_string, got_arg_3_string);
    }

    void testAfterInvoke() {}
};

TEST(CukeCommandsTest, handlesParameters) {
    CukeCommandsTestInjector<CheckAllParameters> commands;
    commands.buildArgsAndInvoke();
}
