#include <cucumber-cpp/internal/connectors/wire/WireProtocolCommands.hpp>
#include <boost/make_shared.hpp>

namespace cucumber {
namespace internal {

ScenarioCommand::ScenarioCommand(const CukeEngine::tags_type& tags) :
    tags(tags) {
}


BeginScenarioCommand::BeginScenarioCommand(const CukeEngine::tags_type& tags) :
    ScenarioCommand(tags) {
}

boost::shared_ptr<WireResponse> BeginScenarioCommand::run(CukeEngine& engine) const {
    const auto Start = std::chrono::steady_clock::now();
    engine.beginScenario(tags);
    const auto DurationMs = std::chrono::duration_cast<std::chrono::milliseconds>(
                                std::chrono::steady_clock::now() - Start)
                                .count();
    return boost::make_shared<SuccessResponse>(DurationMs);
}


EndScenarioCommand::EndScenarioCommand(const CukeEngine::tags_type& tags) :
    ScenarioCommand(tags) {
}

boost::shared_ptr<WireResponse> EndScenarioCommand::run(CukeEngine& engine) const {
    const auto Start = std::chrono::steady_clock::now();
    engine.endScenario(tags);
    const auto DurationMs = std::chrono::duration_cast<std::chrono::milliseconds>(
                                std::chrono::steady_clock::now() - Start)
                                .count();
    return boost::make_shared<SuccessResponse>(DurationMs);
}


StepMatchesCommand::StepMatchesCommand(const std::string & stepName) :
    stepName(stepName) {
}

boost::shared_ptr<WireResponse> StepMatchesCommand::run(CukeEngine& engine) const {
    std::vector<StepMatch> matchingSteps = engine.stepMatches(stepName);
    return boost::make_shared<StepMatchesResponse>(matchingSteps);
}


InvokeCommand::InvokeCommand(const std::string & stepId,
                             const CukeEngine::invoke_args_type& args,
                             const CukeEngine::invoke_table_type& tableArg) :
    stepId(stepId),
    args(args),
    tableArg(tableArg) {
}

boost::shared_ptr<WireResponse> InvokeCommand::run(CukeEngine& engine) const {
    try {
        const auto Start = std::chrono::steady_clock::now();
        engine.invokeStep(stepId, args, tableArg);
        const auto DurationMs = std::chrono::duration_cast<std::chrono::milliseconds>(
                                    std::chrono::steady_clock::now() - Start)
                                    .count();
        return boost::make_shared<SuccessResponse>(DurationMs);
    } catch (const InvokeFailureException& e) {
        return boost::make_shared<FailureResponse>(e.getMessage(), e.getExceptionType());
    } catch (const PendingStepException& e) {
        return boost::make_shared<PendingResponse>(e.getMessage());
    } catch (...) {
        return boost::make_shared<FailureResponse>();
    }
}


SnippetTextCommand::SnippetTextCommand(const std::string & keyword, const std::string & name, const std::string & multilineArgClass) :
    keyword(keyword),
    name(name),
    multilineArgClass(multilineArgClass) {
}

boost::shared_ptr<WireResponse> SnippetTextCommand::run(CukeEngine& engine) const {
    return boost::make_shared<SnippetTextResponse>(engine.snippetText(keyword, name, multilineArgClass));
}


boost::shared_ptr<WireResponse> FailingCommand::run(CukeEngine& /*engine*/) const {
    return boost::make_shared<FailureResponse>();
}

}
}
