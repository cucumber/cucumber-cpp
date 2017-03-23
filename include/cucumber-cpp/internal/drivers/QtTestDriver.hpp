#ifndef CUKE_QTTESTDRIVER_HPP_
#define CUKE_QTTESTDRIVER_HPP_

#include "../step/StepManager.hpp"

namespace cucumber {
namespace internal {

class QtTestStep : public BasicStep{
    friend class QtTestObject;
public:
    QtTestStep(): BasicStep() {}

protected:
    const InvokeResult invokeStepBody();
};

#define STEP_INHERITANCE(step_name) ::cucumber::internal::QtTestStep

}
}

#endif /* CUKE_QTTESTDRIVER_HPP_ */
