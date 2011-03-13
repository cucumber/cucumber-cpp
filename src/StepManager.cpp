#include "cukebins/internal/step/StepManager.hpp"

namespace cukebins {
namespace internal {

StepInfo::StepInfo(const std::string &stepMatcher) :
    regex(stepMatcher.c_str()) {
    static step_id_type currentId = 0;
    id = ++currentId;
}

Match StepInfo::matches(const std::string &stepDescription) {
    Match currentMatch;
    boost::cmatch matchResults;
    if (boost::regex_search(stepDescription.c_str(), matchResults, regex)) {
        currentMatch.id = id;
        for (boost::cmatch::size_type i = 1; i < matchResults.size(); ++i) {
            SubExpression s;
            s.value = matchResults.str(i);
            s.position = matchResults.position(i);
            currentMatch.subExpressions.push_back(s);
        }
    }
    return currentMatch;
}

Match::Match() :
    id(0) {
}

Match::Match(const Match &match) :
    id(match.id),
    subExpressions(match.subExpressions) {
}

Match & Match::operator =(const Match &match) {
    id = match.id;
    subExpressions = match.subExpressions;
    return *this;
}

Match::operator void *() {
    return (void *) id;
}

MatchResult::operator void *() {
    return (void *) resultSet.size();
}

MatchResult::operator bool() {
    return !resultSet.empty();
}

const match_results_type MatchResult::getResultSet() {
    return resultSet;
}

void MatchResult::addMatch(Match match) {
    resultSet.push_back(match);
}

StepManager::~StepManager() {
}

void StepManager::addStep(StepInfo *stepInfo) {
    steps().insert(std::make_pair(stepInfo->id, stepInfo));
}

MatchResult StepManager::stepMatches(const std::string &stepDescription) {
    MatchResult matchResult;
    for (steps_type::iterator iter = steps().begin(); iter != steps().end(); ++iter) {
        StepInfo *stepInfo = iter->second;
        Match currentMatch = stepInfo->matches(stepDescription);
        if (currentMatch) {
            matchResult.addMatch(currentMatch);
        }
    }
    return matchResult;
}

StepInfo *StepManager::getStep(step_id_type id) {
    return steps()[id];
}

/**
 * Needed to fix the "static initialization order fiasco"
 * http://www.parashift.com/c++-faq-lite/ctors.html#faq-10.12
 */
StepManager::steps_type& StepManager::steps() {
    static steps_type *steps = new steps_type();
    return *steps;
}


InvokeResult BasicStep::invoke(shared_ptr<command_args_type> args) {
    invokeArgsPtr = args;
    currentArgIndex = 0;
    return invokeStepBody();
}


}
}

