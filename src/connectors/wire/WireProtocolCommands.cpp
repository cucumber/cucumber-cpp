#include <cucumber-cpp/internal/connectors/wire/WireProtocolCommands.hpp>

namespace cucumber {
namespace internal {

ScenarioCommand::ScenarioCommand(const CukeEngine::tags_type& tags) :
    tags(tags) {
}

BeginScenarioCommand::BeginScenarioCommand(const CukeEngine::tags_type& tags) :
    ScenarioCommand(tags) {
}

std::shared_ptr<WireResponse> BeginScenarioCommand::run(CukeEngine& engine) const {
    engine.beginScenario(tags);
    return std::make_shared<SuccessResponse>();
}

EndScenarioCommand::EndScenarioCommand(const CukeEngine::tags_type& tags) :
    ScenarioCommand(tags) {
}

std::shared_ptr<WireResponse> EndScenarioCommand::run(CukeEngine& engine) const {
    engine.endScenario(tags);
    return std::make_shared<SuccessResponse>();
}

StepMatchesCommand::StepMatchesCommand(const std::string& stepName) :
    stepName(stepName) {
}

std::shared_ptr<WireResponse> StepMatchesCommand::run(CukeEngine& engine) const {
    std::vector<StepMatch> matchingSteps = engine.stepMatches(stepName);
    return std::make_shared<StepMatchesResponse>(matchingSteps);
}

InvokeCommand::InvokeCommand(
    const std::string& stepId,
    const CukeEngine::invoke_args_type& args,
    const CukeEngine::invoke_table_type& tableArg
) :
    stepId(stepId),
    args(args),
    tableArg(tableArg) {
}

std::shared_ptr<WireResponse> InvokeCommand::run(CukeEngine& engine) const {
    try {
        engine.invokeStep(stepId, args, tableArg);
        return std::make_shared<SuccessResponse>();
    } catch (const InvokeFailureException& e) {
        return std::make_shared<FailureResponse>(e.getMessage(), e.getExceptionType());
    } catch (const PendingStepException& e) {
        return std::make_shared<PendingResponse>(e.getMessage());
    } catch (...) {
        return std::make_shared<FailureResponse>();
    }
}

SnippetTextCommand::SnippetTextCommand(
    const std::string& keyword, const std::string& name, const std::string& multilineArgClass
) :
    keyword(keyword),
    name(name),
    multilineArgClass(multilineArgClass) {
}

std::shared_ptr<WireResponse> SnippetTextCommand::run(CukeEngine& engine) const {
    return std::make_shared<SnippetTextResponse>(
        engine.snippetText(keyword, name, multilineArgClass)
    );
}

std::shared_ptr<WireResponse> FailingCommand::run(CukeEngine& /*engine*/) const {
    return std::make_shared<FailureResponse>();
}

}
}
