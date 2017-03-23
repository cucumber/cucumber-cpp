#include "cucumber-cpp/internal/CukeCommands.hpp"

#include <sstream>
#include <boost/algorithm/string.hpp>
#include <boost/make_shared.hpp>

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

void CukeCommands::beginScenario(const TagExpression::tag_list& tags) {
    if (!hasStarted) {
        hasStarted = true;
        hookRegistrar.execBeforeAllHooks();
    }

    currentScenario = boost::make_shared<Scenario>(tags);
    hookRegistrar.execBeforeHooks(currentScenario.get());
}

void CukeCommands::endScenario() {
    hookRegistrar.execAfterHooks(currentScenario.get());
    contextManager.purgeContexts();
    currentScenario.reset();
}

const std::string CukeCommands::snippetText(const std::string stepKeyword, const std::string stepName) const {
    std::stringstream text;
    text << boost::to_upper_copy(stepKeyword)
        << "(\""
        << escapeCString("^" + escapeRegex(stepName) + "$")
        << "\") {\n"
        << "    pending();\n"
        << "}\n";
    return text.str();
}

const std::string CukeCommands::escapeRegex(const std::string reg) const {
    return regex_replace(reg, boost::regex("[\\|\\(\\)\\[\\]\\{\\}\\^\\$\\*\\+\\?\\.\\\\]"), "\\\\&", boost::match_default | boost::format_sed);
}

const std::string CukeCommands::escapeCString(const std::string str) const {
    return regex_replace(str, boost::regex("[\"\\\\]"), "\\\\&", boost::match_default | boost::format_sed);
}

MatchResult CukeCommands::stepMatches(const std::string description) const {
    return stepManager.stepMatches(description);
}

InvokeResult CukeCommands::invoke(step_id_type id, const InvokeArgs *pArgs) {
    boost::shared_ptr<const StepInfo> stepInfo = stepManager.getStep(id);
    InvokeResult result = hookRegistrar.execStepChain(currentScenario.get(), stepInfo, pArgs);
    hookRegistrar.execAfterStepHooks(currentScenario.get());
    return result;
}

}
}
