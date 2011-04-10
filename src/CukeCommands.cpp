#include "cukebins/internal/CukeCommands.hpp"

namespace cukebins {
namespace internal {

void CukeCommands::beginScenario() {
    hookRegistrar.execBeforeHooks();
}

void CukeCommands::endScenario() {
    contextManager.purgeContexts();
    hookRegistrar.execAfterHooks();
}

MatchResult CukeCommands::stepMatches(const std::string description) {
    return stepManager.stepMatches(description);
}

InvokeResult CukeCommands::invoke(step_id_type id, command_args_type *args) {
    StepInfo *stepInfo = stepManager.getStep(id);
    InvokeResult result = hookRegistrar.execStepChain(stepInfo, args);
    hookRegistrar.execAfterStepHooks();
    return result;
}

}
}
