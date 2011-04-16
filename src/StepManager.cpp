#include "cukebins/internal/step/StepManager.hpp"

namespace cukebins {
namespace internal {

StepInfo::StepInfo(const std::string &stepMatcher) :
    regex(stepMatcher) {
    static step_id_type currentId = 0;
    id = ++currentId;
}

SingleStepMatch StepInfo::matches(const std::string &stepDescription) {
    SingleStepMatch stepMatch;
    RegexMatch regexMatch = regex.find(stepDescription);
    if (regexMatch.matches()) {
        stepMatch.id = id;
        stepMatch.submatches = regexMatch.getSubmatches();
    }
    return stepMatch;
}

SingleStepMatch::SingleStepMatch() :
    id(0) {
}

SingleStepMatch::SingleStepMatch(const SingleStepMatch &match) :
    id(match.id),
    submatches(match.submatches) {
}

SingleStepMatch & SingleStepMatch::operator =(const SingleStepMatch &match) {
    id = match.id;
    submatches = match.submatches;
    return *this;
}

SingleStepMatch::operator void *() {
    return (void *) id;
}

MatchResult::operator void *() {
    return (void *) resultSet.size();
}

MatchResult::operator bool() {
    return !resultSet.empty();
}

const MatchResult::match_results_type MatchResult::getResultSet() {
    return resultSet;
}

void MatchResult::addMatch(SingleStepMatch match) {
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
        SingleStepMatch currentMatch = stepInfo->matches(stepDescription);
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


InvokeResult BasicStep::invoke(command_args_type *args) {
    invokeArgsPtr = args;
    currentArgIndex = 0;
    return invokeStepBody();
}


}
}

