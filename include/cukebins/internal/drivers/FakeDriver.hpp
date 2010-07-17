#ifndef CUKEBINS_FAKEDRIVER_HPP_
#define CUKEBINS_FAKEDRIVER_HPP_

#include <cukebins/internal/CukeCommands.hpp>

namespace cukebins {
namespace internal {

class CukeCommands : public AbstractCommands {
protected:
    const InvokeResult invokeNoArgs(StepInfo *stepInfo) {
        InvokeResult result;
        result.success = true;
        return result;
    }
};

#define STEP_INHERITANCE(step_name) ::cukebins::internal::BasicStep

}
}

#endif /* CUKEBINS_FAKEDRIVER_HPP_ */
