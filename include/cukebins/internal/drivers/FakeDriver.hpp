#ifndef CUKEBINS_FAKEDRIVER_HPP_
#define CUKEBINS_FAKEDRIVER_HPP_

#include <cukebins/internal/CukeCommands.hpp>

namespace cukebins {
namespace internal {

class CukeCommands : public AbstractCommands {
protected:
    const InvokeResult invokeNoArgs(StepInfo *stepInfo);
};

#define STEP_INHERITANCE(step_name) ::cukebins::internal::BasicStep


const InvokeResult CukeCommands::invokeNoArgs(StepInfo *stepInfo) {
    InvokeResult result;
    result.success = true;
    return result;
}

}
}

#endif /* CUKEBINS_FAKEDRIVER_HPP_ */
