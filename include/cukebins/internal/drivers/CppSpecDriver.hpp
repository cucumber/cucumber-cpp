#ifndef CUKEBINS_CPPSPECDRIVER_HPP_
#define CUKEBINS_CPPSPECDRIVER_HPP_

#include <cukebins/internal/StepManager.hpp>

namespace cukebins {
namespace internal {

class CppSpecStep : public BasicStep {
protected:
    const InvokeResult invokeStepBody();
};

#define STEP_INHERITANCE(step_name) ::cukebins::internal::CppSpecStep, CppSpec::Specification<void, step_name >

}
}

#endif /* CUKEBINS_CPPSPECDRIVER_HPP_ */
