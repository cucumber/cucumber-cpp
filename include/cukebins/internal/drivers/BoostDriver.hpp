#ifndef CUKEBINS_BOOSTDRIVER_HPP_
#define CUKEBINS_BOOSTDRIVER_HPP_

#include <boost/bind.hpp>
#include <cukebins/internal/StepManager.hpp>

namespace cukebins {
namespace internal {

class BoostTestObserver;

class BoostStep : public BasicStep {
protected:
    const InvokeResult invokeStepBody();

private:
    void initBoostTest();
    void runWithMasterSuite();

    static BoostTestObserver boostTestObserver;
};

#define STEP_INHERITANCE(step_name) ::cukebins::internal::BoostStep

}
}

#endif /* CUKEBINS_BOOSTDRIVER_HPP_ */
