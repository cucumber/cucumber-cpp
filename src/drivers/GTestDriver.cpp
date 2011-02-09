#include <cukebins/internal/drivers/GTestDriver.hpp>

#include <gtest/gtest.h>

namespace cukebins {
namespace internal {

bool GTestStep::initialized(false);

const InvokeResult GTestStep::invokeStepBody() {
    InvokeResult result;
    if (!initialized) {
        initGTest();
        initFlags();
    }
    try {
        stepBody();
        result.success = true;
    } catch (...) {
    }
    return result;
}

void GTestStep::initGTest() {
    int fake_argc = 1;
    char *fake_argv[1];
    fake_argv[0] = (char *) "cukebins";
    ::testing::InitGoogleTest(&fake_argc, fake_argv);
    initialized = true;
}

void GTestStep::initFlags() {
    ::testing::GTEST_FLAG(throw_on_failure) = true;  // let CukeBins drive
    ::testing::GTEST_FLAG(break_on_failure) = false; // turn off debugger breakpoints
    ::testing::GTEST_FLAG(catch_exceptions) = true;
}

}
}
