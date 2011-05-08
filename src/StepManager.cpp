#include "cukebins/internal/step/StepManager.hpp"

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

namespace cukebins {
namespace internal {


// declared here because it should not be used outside
class PendingStep {
    const char *description;

public:
    PendingStep(const char *description) :
        description(description) {
    }

    const char *getDescription() {
        return description;
    }
};



StepInfo::StepInfo(const std::string &stepMatcher) :
    regex(stepMatcher) {
    static step_id_type currentId = 0;
    id = ++currentId;
}

SingleStepMatch StepInfo::matches(const std::string &stepDescription) {
    SingleStepMatch stepMatch;
    shared_ptr<RegexMatch> regexMatch(regex.find(stepDescription));
    if (regexMatch->matches()) {
        stepMatch.id = id;
        stepMatch.submatches = regexMatch->getSubmatches();
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


InvokeResult::InvokeResult(const InvokeResultType type, const char *description) :
    type(type),
    description(description) {
}

InvokeResult::InvokeResult() :
    type(FAILURE) {
}

InvokeResult::InvokeResult(const InvokeResult &ir) :
    type(ir.type),
    description(ir.description) {
}

InvokeResult& InvokeResult::operator= (const InvokeResult &rhs) {
    this->type = rhs.type;
    this->description = rhs.description;
    return *this;
}

InvokeResult InvokeResult::success() {
    return InvokeResult(SUCCESS, 0);
}

InvokeResult InvokeResult::failure() {
    return InvokeResult(FAILURE, 0);
}

InvokeResult InvokeResult::pending(const char *description) {
    return InvokeResult(PENDING, description);
}

bool InvokeResult::isSuccess() {
    return (type == SUCCESS);
}

bool InvokeResult::isPending() {
    return (type == PENDING);
}

const char *InvokeResult::getType() const {
    switch (type) {
    case SUCCESS:
        return "success";
    case FAILURE:
        return "fail";
    case PENDING:
        return "pending";
    default:
        return 0;
    }
}

const char *InvokeResult::getDescription() const {
    return description;
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
    try {
        return invokeStepBody();
    } catch (PendingStep &e) {
        const char * d = e.getDescription();
        return InvokeResult::pending(d);
    } catch (...) {
        return InvokeResult::failure();
    }
}

void BasicStep::pending() {
    pending(0);
}

void BasicStep::pending(const char *description) {
    throw PendingStep(description);
}


}
}

