#ifndef CUKE_STEPMANAGERTESTDOUBLE_HPP_
#define CUKE_STEPMANAGERTESTDOUBLE_HPP_

#include <cucumber-cpp/internal/step/StepManager.hpp>

namespace cucumber {
namespace internal {

class StepInfoNoOp : public StepInfo {
public:
    StepInfoNoOp(const std::string& stepMatcher, const std::string source) :
        StepInfo(stepMatcher, source) {
    }
    InvokeResult invokeStep(const InvokeArgs*) const override {
        return InvokeResult::success();
    }
};

class StepInfoPending : public StepInfo {
private:
    const char* description;

public:
    StepInfoPending(const std::string& stepMatcher, const char* description) :
        StepInfo(stepMatcher, ""),
        description(description) {
    }

    InvokeResult invokeStep(const InvokeArgs*) const override {
        return InvokeResult::pending(description);
    }
};

/*
 * FIXME This should be a mock testing it receives a table argument
 */
class StepInfoWithTableArg : public StepInfo {
    const unsigned short expectedSize;

public:
    StepInfoWithTableArg(const std::string& stepMatcher, const unsigned short expectedSize) :
        StepInfo(stepMatcher, ""),
        expectedSize(expectedSize) {
    }

    InvokeResult invokeStep(const InvokeArgs* pArgs) const override {
        if (pArgs->getTableArg().hashes().size() == expectedSize) {
            return InvokeResult::success();
        } else {
            return InvokeResult::failure("Failed");
        }
    }
};

class StepManagerTestDouble : public StepManager {
public:
    static void clearSteps() {
        steps().clear();
    }

    static steps_type::size_type count() {
        return steps().size();
    }

    static step_id_type addStepDefinition(const std::string& stepMatcher) {
        return addStep(std::make_shared<StepInfoNoOp>(stepMatcher, ""));
    }

    static step_id_type addStepDefinitionWithId(
        step_id_type desiredId, const std::string& stepMatcher
    ) {
        return addStepDefinitionWithId(desiredId, stepMatcher, "");
    }

    static step_id_type addStepDefinitionWithId(
        step_id_type desiredId, const std::string& stepMatcher, const std::string source
    ) {
        std::shared_ptr<StepInfo> stepInfo(std::make_shared<StepInfoNoOp>(stepMatcher, source));
        stepInfo->id = desiredId;
        return addStep(stepInfo);
    }

    static step_id_type addPendingStepDefinitionWithId(
        step_id_type desiredId, const std::string& stepMatcher
    ) {
        return addPendingStepDefinitionWithId(desiredId, stepMatcher, 0);
    }

    static step_id_type addPendingStepDefinitionWithId(
        step_id_type desiredId, const std::string& stepMatcher, const char* description
    ) {
        std::shared_ptr<StepInfo> stepInfo(
            std::make_shared<StepInfoPending>(stepMatcher, description)
        );
        stepInfo->id = desiredId;
        return addStep(stepInfo);
    }

    static step_id_type addTableStepDefinitionWithId(
        step_id_type desiredId, const std::string& stepMatcher, const unsigned short expectedSize
    ) {
        std::shared_ptr<StepInfo> stepInfo(
            std::make_shared<StepInfoWithTableArg>(stepMatcher, expectedSize)
        );
        stepInfo->id = desiredId;
        return addStep(stepInfo);
    }

    static step_id_type getStepId(const std::string& stepMatcher) {
        step_id_type id = 0;
        for (steps_type::const_iterator i = steps().begin(); i != steps().end(); ++i) {
            const StepInfo& stepInfo = *i->second;
            if (stepInfo.regex.str() == stepMatcher) {
                id = stepInfo.id;
                break;
            }
        }
        return id;
    }
};

}
}

#endif /* CUKE_STEPMANAGERTESTDOUBLE_HPP_ */
