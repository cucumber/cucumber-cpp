#include "cucumber-cpp/internal/step/StepManager.hpp"

namespace cucumber {
namespace internal {

StepInfo::StepInfo(const std::string& stepMatcher, const std::string source) :
    regex(stepMatcher),
    source(source) {
    static step_id_type currentId = 0;
    id = ++currentId;
}

SingleStepMatch StepInfo::matches(const std::string& stepDescription) const {
    SingleStepMatch stepMatch;
    std::shared_ptr<RegexMatch> regexMatch(regex.find(stepDescription));
    if (regexMatch->matches()) {
        stepMatch.stepInfo = shared_from_this();
        stepMatch.submatches = regexMatch->getSubmatches();
    }
    return stepMatch;
}

SingleStepMatch::operator const void*() const {
    return stepInfo.get();
}

MatchResult::operator bool() const {
    return !resultSet.empty();
}

const MatchResult::match_results_type& MatchResult::getResultSet() {
    return resultSet;
}

void MatchResult::addMatch(SingleStepMatch match) {
    resultSet.push_back(match);
}

void InvokeArgs::addArg(const std::string arg) {
    args.push_back(arg);
}

const Table& InvokeArgs::getTableArg() const {
    return tableArg;
}

Table& InvokeArgs::getVariableTableArg() {
    return tableArg;
}

InvokeResult::InvokeResult(const InvokeResultType type, const char* description) :
    type(type) {
    if (description) {
        this->description = description;
    };
}

InvokeResult::InvokeResult() :
    type(FAILURE) {
}

InvokeResult::InvokeResult(const InvokeResult& ir) :
    type(ir.type),
    description(ir.description) {
}

InvokeResult& InvokeResult::operator=(const InvokeResult& rhs) {
    this->type = rhs.type;
    this->description = rhs.description;
    return *this;
}

InvokeResult InvokeResult::success() {
    return InvokeResult(SUCCESS, 0);
}

InvokeResult InvokeResult::failure(const char* description) {
    return InvokeResult(FAILURE, description);
}

InvokeResult InvokeResult::failure(const std::string& description) {
    return InvokeResult(FAILURE, description.c_str());
}

InvokeResult InvokeResult::pending(const char* description) {
    return InvokeResult(PENDING, description);
}

bool InvokeResult::isSuccess() const {
    return (type == SUCCESS);
}

bool InvokeResult::isPending() const {
    return (type == PENDING);
}

InvokeResultType InvokeResult::getType() const {
    return type;
}

const std::string& InvokeResult::getDescription() const {
    return description;
}

step_id_type StepManager::addStep(std::shared_ptr<StepInfo> stepInfo) {
    return steps().insert(std::make_pair(stepInfo->id, stepInfo)).first->first;
}

MatchResult StepManager::stepMatches(const std::string& stepDescription) {
    MatchResult matchResult;
    for (steps_type::iterator iter = steps().begin(); iter != steps().end(); ++iter) {
        const std::shared_ptr<const StepInfo>& stepInfo = iter->second;
        SingleStepMatch currentMatch = stepInfo->matches(stepDescription);
        if (currentMatch) {
            matchResult.addMatch(currentMatch);
        }
    }
    return matchResult;
}

const StepInfo* StepManager::getStep(step_id_type id) {
    const steps_type::const_iterator step = steps().find(id);
    if (step == steps().end()) {
        return NULL;
    }
    return step->second.get();
}

/**
 * Needed to fix the "static initialization order fiasco"
 * http://www.parashift.com/c++-faq-lite/ctors.html#faq-10.12
 */
StepManager::steps_type& StepManager::steps() {
    static steps_type steps;
    return steps;
}

InvokeResult BasicStep::invoke(const InvokeArgs* pArgs) {
    this->pArgs = pArgs;
    currentArgIndex = 0;
    currentResult = InvokeResult::success();
    try {
        InvokeResult returnedResult = invokeStepBody();
        if (currentResult.isPending()) {
            return currentResult;
        } else {
            return returnedResult;
        }
    } catch (const std::exception& ex) {
        return InvokeResult::failure(ex.what());
    } catch (const std::string& ex) {
        return InvokeResult::failure(ex);
    } catch (const char* ex) {
        return InvokeResult::failure(ex);
    } catch (...) {
        // Cucumber needs a description here
        return InvokeResult::failure("Unknown exception");
    }
}

void BasicStep::pending() {
    pending(0);
}

void BasicStep::pending(const char* description) {
    currentResult = InvokeResult::pending(description);
}

const InvokeArgs* BasicStep::getArgs() {
    return pArgs;
}

}
}
