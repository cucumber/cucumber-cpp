#ifndef CUKEBINS_STEPMANAGERTESTDOUBLE_HPP_
#define CUKEBINS_STEPMANAGERTESTDOUBLE_HPP_

#include <cukebins/internal/step/StepManager.hpp>

namespace cukebins {
namespace internal {

class StepInfoNoOp : public StepInfo {
public:
    StepInfoNoOp(const std::string &stepMatcher) : StepInfo(stepMatcher) {}
    InvokeResult invokeStep(command_args_type *args) {
		InvokeResult result;
		result.success = true;
		return result;
	}
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
        step_id_type id = 0;
        for (steps_type::const_iterator i = steps().begin(); i != steps().end(); ++i) {
            StepInfo *stepInfo = i->second;
            if (stepInfo->regex.str() == stepMatcher) {
                id = stepInfo->id;
                break;
            }
        }
        return id;
    }
};

}
}

#endif /* CUKEBINS_STEPMANAGERTESTDOUBLE_HPP_ */
