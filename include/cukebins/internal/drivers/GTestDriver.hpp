#ifndef CUKEBINS_GTESTDRIVER_HPP_
#define CUKEBINS_GTESTDRIVER_HPP_

#include <cukebins/internal/StepManager.hpp>

#include <iostream>

namespace cukebins {
namespace internal {

class GTestStep : public BasicStep {
protected:
    const InvokeResult invokeStepBody();

private:
    void initGTest();
    void initFlags();

protected:
    static bool initialized;
};

#define STEP_INHERITANCE(step_name) ::cukebins::internal::GTestStep

}
}

#endif /* CUKEBINS_GTESTDRIVER_HPP_ */
