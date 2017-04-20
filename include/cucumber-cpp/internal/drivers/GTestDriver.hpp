#ifndef CUKE_GTESTDRIVER_HPP_
#define CUKE_GTESTDRIVER_HPP_

#include "../CukeDll.hpp"
#include "../step/StepManager.hpp"

#include <iostream>

namespace cucumber {
namespace internal {

class CUKE_API_ GTestStep : public BasicStep {
protected:
    const InvokeResult invokeStepBody();

private:
    void initGTest();
    void initFlags();

protected:
    static bool initialized;
};

#define STEP_INHERITANCE(step_name) ::cucumber::internal::GTestStep

}
}

#endif /* CUKE_GTESTDRIVER_HPP_ */
