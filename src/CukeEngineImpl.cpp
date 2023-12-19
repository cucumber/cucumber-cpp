#include "cucumber-cpp/internal/CukeEngineImpl.hpp"

namespace cucumber {
namespace internal {

namespace {

std::string convertId(step_id_type id) {
    std::stringstream ss;
    ss << id;
    return ss.str();
}

step_id_type convertId(const std::string& stringid) {
    std::stringstream ss(stringid);
    step_id_type id;
    ss >> id;
    return id;
}
}

std::vector<StepMatch> CukeEngineImpl::stepMatches(const std::string& name) const {
    std::vector<StepMatch> engineResult;
    MatchResult commandResult = cukeCommands.stepMatches(name);
    for (const SingleStepMatch& commandMatch : commandResult.getResultSet()) {
        StepMatch engineMatch;
        engineMatch.id = convertId(commandMatch.stepInfo->id);
        engineMatch.source = commandMatch.stepInfo->source;
        engineMatch.regexp = commandMatch.stepInfo->regex.str();
        for (const RegexSubmatch& commandMatchArg : commandMatch.submatches) {
            StepMatchArg engineMatchArg;
            engineMatchArg.value = commandMatchArg.value;
            engineMatchArg.position = commandMatchArg.position;
            engineMatch.args.push_back(engineMatchArg);
        }
        engineResult.push_back(engineMatch);
    }
    return engineResult;
}

void CukeEngineImpl::beginScenario(const tags_type& tags) {
    cukeCommands.beginScenario(tags);
}

void CukeEngineImpl::invokeStep(
    const std::string& id, const invoke_args_type& args, const invoke_table_type& tableArg
) {
    InvokeArgs commandArgs;
    try {
        for (const std::string& a : args) {
            commandArgs.addArg(a);
        }

        if (!tableArg.empty() && !tableArg.front().empty()) {
            Table& commandTableArg = commandArgs.getVariableTableArg();
            for (const auto& arg : tableArg[0]) {
                commandTableArg.addColumn(arg);
            }

            for (std::size_t i = 1; i < tableArg.size(); ++i) {
                Table::row_type row;
                for (const auto& arg : tableArg[i]) {
                    row.push_back(arg);
                }
                commandTableArg.addRow(row);
            }
        }
    } catch (...) {
        throw InvokeException("Unable to decode arguments");
    }

    InvokeResult commandResult;
    try {
        commandResult = cukeCommands.invoke(convertId(id), &commandArgs);
    } catch (...) {
        throw InvokeException("Uncatched exception");
    }
    switch (commandResult.getType()) {
    case SUCCESS:
        return;
    case FAILURE:
        throw InvokeFailureException(commandResult.getDescription(), "");
    case PENDING:
        throw PendingStepException(commandResult.getDescription());
    }
}

void CukeEngineImpl::endScenario(const tags_type& /*tags*/) {
    cukeCommands.endScenario();
}

std::string CukeEngineImpl::snippetText(
    const std::string& keyword, const std::string& name, const std::string& /*multilineArgClass*/
) const {
    return cukeCommands.snippetText(keyword, name);
}

}
}
