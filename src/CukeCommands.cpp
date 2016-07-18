#include "cucumber-cpp/internal/CukeCommands.hpp"

#include <sstream>
#include <boost/algorithm/string.hpp>

namespace cucumber {
namespace internal {

shared_ptr<Scenario> CukeCommands::currentScenario;

CukeCommands::CukeCommands() : hasStarted(false) {
}

CukeCommands::~CukeCommands() {
    if (hasStarted) {
        hookRegistrar.execAfterAllHooks();
    }
}

void CukeCommands::beginScenario(const TagExpression::tag_list *tags) {
    if (!hasStarted) {
        hasStarted = true;
        hookRegistrar.execBeforeAllHooks();
    }

    currentScenario = shared_ptr<Scenario>(new Scenario(tags));
    hookRegistrar.execBeforeHooks(currentScenario.get());
}

void CukeCommands::endScenario() {
    hookRegistrar.execAfterHooks(currentScenario.get());
    contextManager.purgeContexts();
    currentScenario.reset();
}

const std::string CukeCommands::snippetText(const std::string stepKeyword, const std::string stepName) const {
    const boost::regex esc("[\".^$|()\\[\\]{}*+?\\\\]");
    const std::string rep("\\\\&");
    std::stringstream text;
    text << boost::to_upper_copy(stepKeyword)
        << "(\"^"
        << regex_replace(stepName, esc, rep, boost::match_default | boost::format_sed)
        << "$\") {\n"
        << "    pending();\n"
        << "}\n";
    return text.str();
}

MatchResult CukeCommands::stepMatches(const std::string description) const {
    return stepManager.stepMatches(description);
}

InvokeResult CukeCommands::invoke(step_id_type id, const InvokeArgs *pArgs) {
    StepInfo *stepInfo = stepManager.getStep(id);
    InvokeResult result = hookRegistrar.execStepChain(currentScenario.get(), stepInfo, pArgs);
    hookRegistrar.execAfterStepHooks(currentScenario.get());
    return result;
}

}
}
