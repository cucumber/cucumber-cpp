#ifndef CUKE_GENERICDRIVER_HPP_
#define CUKE_GENERICDRIVER_HPP_

#include "../step/StepManager.hpp"
#include <cucumber-cpp/internal/CukeExport.hpp>

namespace cucumber {
namespace internal {

class CUCUMBER_CPP_EXPORT GenericStep : public BasicStep {
protected:
    virtual const InvokeResult invokeStepBody();
};

#define STEP_INHERITANCE(step_name) virtual ::cucumber::internal::GenericStep

}
}

#endif /* CUKE_GENERICDRIVER_HPP_ */
