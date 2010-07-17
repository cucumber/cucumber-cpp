#ifndef CUKEBINS_STEPMANAGERTESTDOUBLE_HPP_
#define CUKEBINS_STEPMANAGERTESTDOUBLE_HPP_

#include <cukebins/internal/StepManager.hpp>

namespace cukebins {
namespace internal {

class StepInfoNoOp : public StepInfo {
public:
    StepInfoNoOp(const std::string &stepMatcher) : StepInfo(stepMatcher) {}
    void invokeStep() {}
};

class StepManagerTestDouble : public StepManager {
public:
    void clearSteps() {
        steps().clear();
    }

    steps_type::size_type count() {
        return steps().size();
    }

    step_id_type addStepDefinition(const std::string &stepMatcher) {
        StepInfo *stepInfo = new StepInfoNoOp(stepMatcher);
        addStep(stepInfo);
        return stepInfo->id;
    }

    void addStepDefinitionWithId(step_id_type desiredId, const std::string &stepMatcher) {
        StepInfo *stepInfo = new StepInfoNoOp(stepMatcher);
        stepInfo->id = desiredId;
        addStep(stepInfo);
    }

    const step_id_type getStepId(const std::string &stepMatcher) {
        for (steps_type::const_iterator i = steps().begin(); i != steps().end(); ++i) {
            StepInfo *stepInfo = i->second;
            if (stepInfo->regex.str() == stepMatcher) {
                return stepInfo->id;
            }
        }
    }
};

}
}

#endif /* CUKEBINS_STEPMANAGERTESTDOUBLE_HPP_ */
