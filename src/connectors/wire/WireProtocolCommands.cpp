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
    engine.beginScenario(tags);
    return boost::make_shared<SuccessResponse>();
}


EndScenarioCommand::EndScenarioCommand(const CukeEngine::tags_type& tags) :
    ScenarioCommand(tags) {
}

boost::shared_ptr<WireResponse> EndScenarioCommand::run(CukeEngine& engine) const {
    engine.endScenario(tags);
    return boost::make_shared<SuccessResponse>();
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
        const InvokeResult commandResult = engine.invokeStep(stepId, args, tableArg);

        switch (commandResult.getType()) {
        case SUCCESS:
            return boost::make_shared<SuccessResponse>(commandResult.getEmbeddings());
        case FAILURE:
            return boost::make_shared<FailureResponse>(
                commandResult.getDescription(), "", commandResult.getEmbeddings());
        case PENDING:
            return boost::make_shared<PendingResponse>(commandResult.getDescription());
        }

        return boost::make_shared<FailureResponse>("Unknown result");
    } catch (const InvokeException& e) {
        return boost::make_shared<FailureResponse>(e.getMessage(), "InvokeException");
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
