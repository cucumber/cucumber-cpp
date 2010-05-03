#ifndef CUKEBINS_STEPMANAGERTESTDOUBLE_HPP_
#define CUKEBINS_STEPMANAGERTESTDOUBLE_HPP_

#include <cukebins/internal/StepManager.hpp>

namespace cukebins {
namespace internal {

class StepManagerTestDouble : public StepManager {
public:
    void clearSteps() {
        steps().clear();
    }
    steps_type::size_type count() {
        return steps().size();
    }
    void addStepDefinitionWithId(StepInfo::id_type desiredId, const std::string &stepMatcher,
            const std::string &testName) {
        StepInfo stepInfo(stepMatcher, testName);
        stepInfo.id = desiredId;
        addStep(stepInfo);
    }
};

}
}

#endif /* CUKEBINS_STEPMANAGERTESTDOUBLE_HPP_ */
