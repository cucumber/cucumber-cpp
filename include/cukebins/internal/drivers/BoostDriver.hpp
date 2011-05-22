#ifndef CUKEBINS_BOOSTDRIVER_HPP_
#define CUKEBINS_BOOSTDRIVER_HPP_

#include <cukebins/internal/step/StepManager.hpp>

namespace cukebins {
namespace internal {

class CukeBoostLogInterceptor;

class BoostStep : public BasicStep {
protected:
    const InvokeResult invokeStepBody();

private:
    void initBoostTest();
    void runWithMasterSuite();
};

#define STEP_INHERITANCE(step_name) ::cukebins::internal::BoostStep

}
}

#endif /* CUKEBINS_BOOSTDRIVER_HPP_ */
