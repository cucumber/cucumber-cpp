#include "cukebins/internal/CukeCommands.hpp"

namespace cukebins {
namespace internal {

shared_ptr<Scenario> CukeCommands::currentScenario;

void CukeCommands::beginScenario(const TagExpression::tag_list *tags) {
    currentScenario = shared_ptr<Scenario>(new Scenario(tags));
    hookRegistrar.execBeforeHooks(currentScenario.get());
}

void CukeCommands::endScenario() {
    contextManager.purgeContexts();
    hookRegistrar.execAfterHooks(currentScenario.get());
    currentScenario.reset();
}

MatchResult CukeCommands::stepMatches(const std::string description) {
    return stepManager.stepMatches(description);
}

InvokeResult CukeCommands::invoke(step_id_type id, command_args_type *args) {
    StepInfo *stepInfo = stepManager.getStep(id);
    InvokeResult result = hookRegistrar.execStepChain(currentScenario.get(), stepInfo, args);
    hookRegistrar.execAfterStepHooks(currentScenario.get());
    return result;
}

}
}
