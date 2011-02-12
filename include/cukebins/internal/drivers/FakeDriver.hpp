#ifndef CUKEBINS_FAKEDRIVER_HPP_
#define CUKEBINS_FAKEDRIVER_HPP_

#include <cukebins/internal/StepManager.hpp>

namespace cukebins {
namespace internal {

class FakeStep : public BasicStep {
protected:
    const InvokeResult invokeStepBody();
};

#define STEP_INHERITANCE(step_name) ::cukebins::internal::FakeStep


const InvokeResult FakeStep::invokeStepBody() {
    InvokeResult result;
    // No try/catch block to throw the original exceptions to the testing framework
    stepBody();
    result.success = true;
    return result;
}

}
}

#endif /* CUKEBINS_FAKEDRIVER_HPP_ */
