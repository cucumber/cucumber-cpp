#ifndef CUKEBINS_GTESTDRIVER_HPP_
#define CUKEBINS_GTESTDRIVER_HPP_

#include <cukebins/internal/CukeCommands.hpp>

namespace cukebins {
namespace internal {

class GTestCommands : public AbstractCommands {
protected:
    static bool initialized;

private:
    void initGTest() {
        int fake_argc = 1;
        char *fake_argv[1];
        fake_argv[0] = (char *) "cukebins";
        ::testing::InitGoogleTest(&fake_argc, fake_argv);
        initialized = true;
    }

    void initFlags() {
        ::testing::GTEST_FLAG(throw_on_failure) = true;  // let CukeBins drive
        ::testing::GTEST_FLAG(break_on_failure) = false; // turn off debugger breakpoints
        ::testing::GTEST_FLAG(catch_exceptions) = true;
    }

public:
    void beginScenario() {
        if (!initialized) {
            initGTest();
            initFlags();
        }
    }

protected:
    const InvokeResult invokeNoArgs(StepInfo *stepInfo) {
        InvokeResult result;
        try {
            stepInfo->invokeStep();
            result.success = true;
        } catch (...) {
        }
        return result;
    }
};

bool GTestCommands::initialized = false;

typedef GTestCommands CukeCommands;
#define STEP_INHERITANCE(step_name) ::cukebins::internal::BasicStep

}
}

#endif /* CUKEBINS_GTESTDRIVER_HPP_ */
