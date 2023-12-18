#include "cucumber-cpp/internal/CukeCommands.hpp"
#include "cucumber-cpp/internal/hook/HookRegistrar.hpp"

#include <sstream>

namespace cucumber {
namespace internal {

std::shared_ptr<Scenario> CukeCommands::currentScenario;

CukeCommands::CukeCommands() :
    hasStarted(false) {
}

CukeCommands::~CukeCommands() {
    if (hasStarted) {
        HookRegistrar::execAfterAllHooks();
    }
}

void CukeCommands::beginScenario(const TagExpression::tag_list& tags) {
    if (!hasStarted) {
        hasStarted = true;
        HookRegistrar::execBeforeAllHooks();
    }

    currentScenario = std::make_shared<Scenario>(tags);
    HookRegistrar::execBeforeHooks(currentScenario.get());
}

void CukeCommands::endScenario() {
    HookRegistrar::execAfterHooks(currentScenario.get());
    contextManager.purgeContexts();
    currentScenario.reset();
}

const std::string CukeCommands::snippetText(
    const std::string stepKeyword, const std::string stepName
) const {
    std::stringstream text;
    std::string stepKeywordUpperCase;
    std::transform(
        stepKeyword.begin(), stepKeyword.end(), std::back_inserter(stepKeywordUpperCase), ::toupper
    );
    text << stepKeywordUpperCase << "(\"" << escapeCString("^" + escapeRegex(stepName) + "$")
         << "\") {\n"
         << "    pending();\n"
         << "}\n";
    return text.str();
}

const std::string CukeCommands::escapeRegex(const std::string reg) const {
    return regex_replace(
        reg,
        std::regex("[\\|\\(\\)\\[\\]\\{\\}\\^\\$\\*\\+\\?\\.\\\\]"),
        "\\\\&",
        std::regex_constants::match_default | std::regex_constants::format_sed
    );
}

const std::string CukeCommands::escapeCString(const std::string str) const {
    return regex_replace(
        str,
        std::regex("[\"\\\\]"),
        "\\\\&",
        std::regex_constants::match_default | std::regex_constants::format_sed
    );
}

MatchResult CukeCommands::stepMatches(const std::string description) const {
    return StepManager::stepMatches(description);
}

InvokeResult CukeCommands::invoke(step_id_type id, const InvokeArgs* pArgs) {
    const StepInfo* const stepInfo = StepManager::getStep(id);
    InvokeResult result = HookRegistrar::execStepChain(currentScenario.get(), stepInfo, pArgs);
    HookRegistrar::execAfterStepHooks(currentScenario.get());
    return result;
}

}
}
