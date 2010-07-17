#ifndef CUKEBINS_CPPSPECDRIVER_HPP_
#define CUKEBINS_CPPSPECDRIVER_HPP_

#include <cukebins/internal/CukeCommands.hpp>

namespace cukebins {
namespace internal {

class CppSpecCommands : public AbstractCommands {
protected:
    const InvokeResult invokeNoArgs(StepInfo *stepInfo) {
        InvokeResult result;
        if (stepInfo) {
            try {
                stepInfo->invokeStep();
                result.success = true;
            } catch (...) {
            }
        }
        return result;
    }
};

typedef CppSpecCommands CukeCommands;
#define STEP_INHERITANCE(step_name) CppSpec::Specification<void, step_name >

}
}

#endif /* CUKEBINS_CPPSPECDRIVER_HPP_ */
