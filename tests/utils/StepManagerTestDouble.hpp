#ifndef CUKEBINS_STEPMANAGERTESTDOUBLE_HPP_
#define CUKEBINS_STEPMANAGERTESTDOUBLE_HPP_

#include <cukebins/internal/step/StepManager.hpp>

namespace cukebins {
namespace internal {

class StepInfoNoOp : public StepInfo {
public:
    StepInfoNoOp(const std::string &stepMatcher, const std::string source) : StepInfo(stepMatcher, source) {}
    InvokeResult invokeStep(command_args_type *args) {
        return InvokeResult::success();
    }
};

class StepInfoPending : public StepInfo {
private:
    const char *description;
public:
    StepInfoPending(const std::string &stepMatcher, const char *description) :
        StepInfo(stepMatcher, ""),
        description(description) {
    }

    InvokeResult invokeStep(command_args_type *args) {
        return InvokeResult::pending(description);
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
        StepInfo *stepInfo = new StepInfoNoOp(stepMatcher, "");
        addStep(stepInfo);
        return stepInfo->id;
    }

    void addStepDefinitionWithId(step_id_type desiredId, const std::string &stepMatcher) {
        addStepDefinitionWithId(desiredId, stepMatcher, "");
    }

    void addStepDefinitionWithId(step_id_type desiredId, const std::string &stepMatcher,
            const std::string source) {
        StepInfo *stepInfo = new StepInfoNoOp(stepMatcher, source);
        stepInfo->id = desiredId;
        addStep(stepInfo);
    }

    void addPendingStepDefinitionWithId(step_id_type desiredId, const std::string &stepMatcher) {
        addPendingStepDefinitionWithId(desiredId, stepMatcher, 0);
    }

    void addPendingStepDefinitionWithId(step_id_type desiredId,
            const std::string &stepMatcher, const char *description) {
        StepInfo *stepInfo = new StepInfoPending(stepMatcher, description);
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
