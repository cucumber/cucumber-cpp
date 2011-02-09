#include "cukebins/internal/CukeCommands.hpp"

namespace cukebins {
namespace internal {

void CukeCommands::beginScenario() {
}

void CukeCommands::endScenario() {
    contextManager.purgeContexts();
}

MatchResult CukeCommands::stepMatches(const std::string description) {
    return stepManager.stepMatches(description);
}

InvokeResult CukeCommands::invoke(step_id_type id, shared_ptr<command_args_type> args) {
    StepInfo *stepInfo = stepManager.getStep(id);
    InvokeResult result;
    if (stepInfo) {
        result = stepInfo->invokeStep(args);
    }
    return result;
}

}
}
