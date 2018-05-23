#ifndef CUKE_GTESTDRIVER_HPP_
#define CUKE_GTESTDRIVER_HPP_

#include <cucumber-cpp/internal/CukeExport.hpp>
#include "../step/StepManager.hpp"

#include <iostream>

namespace cucumber {
namespace internal {

class CUCUMBER_CPP_EXPORT GTestStep : public BasicStep {
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
