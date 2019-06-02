#include "cucumber-cpp/internal/step/StepManager.hpp"

namespace cucumber {
namespace internal {


StepInfo::StepInfo(const std::string &stepMatcher, const std::string source) :
    regex(stepMatcher),
    source(source) {
    static step_id_type currentId = 0;
    id = ++currentId;
}

SingleStepMatch StepInfo::matches(const std::string &stepDescription) const {
    SingleStepMatch stepMatch;
    boost::shared_ptr<RegexMatch> regexMatch(regex.find(stepDescription));
    if (regexMatch->matches()) {
        stepMatch.stepInfo = shared_from_this();
        stepMatch.submatches = regexMatch->getSubmatches();
    }
    return stepMatch;
}

SingleStepMatch::operator const void *() const {
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

const Table & InvokeArgs::getTableArg() const {
    return tableArg;
}

Table & InvokeArgs::getVariableTableArg() {
    return tableArg;
}

Embedding::Embedding(const std::string& src, const std::string& mime, const std::string& label)
    : src(src), mime(mime), label(label) {}

Embedding::Embedding(const Embedding& other)
    : src(other.src), mime(other.mime), label(other.label) {}

Embedding& Embedding::operator=(const Embedding& rhs) {
    src = rhs.src;
    mime = rhs.mime;
    label = rhs.label;

    return *this;
}

InvokeResult::InvokeResult(const InvokeResultType type, const std::string& description)
    : type(type), description(description), embeddings() {}

InvokeResult::InvokeResult(const InvokeResultType type,
                           const std::string& description,
                           const std::vector<Embedding>& embeddings)
    : type(type), description(description), embeddings(embeddings) {}

InvokeResult::InvokeResult() :
    type(FAILURE) {
}

InvokeResult::InvokeResult(const InvokeResult& ir)
    : type(ir.type), description(ir.description), embeddings(ir.embeddings) {}

InvokeResult& InvokeResult::operator= (const InvokeResult &rhs) {
    this->type = rhs.type;
    this->description = rhs.description;
    this->embeddings = rhs.embeddings;
    return *this;
}

InvokeResult InvokeResult::success(const std::vector<Embedding>& embeddings) {
    return InvokeResult(SUCCESS, "", embeddings);
}

InvokeResult InvokeResult::failure(const std::string& description,
                                   const std::vector<Embedding>& embeddings) {
    return InvokeResult(FAILURE, description, embeddings);
}

InvokeResult InvokeResult::pending(const std::string& description) {
    return InvokeResult(PENDING, description);
}

void InvokeResult::setEmbeddings(const std::vector<Embedding>& embeddings) {
    this->embeddings = embeddings;
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

const std::string &InvokeResult::getDescription() const {
    return description;
}

const std::vector<Embedding>& InvokeResult::getEmbeddings() const {
    return embeddings;
}

step_id_type StepManager::addStep(boost::shared_ptr<StepInfo> stepInfo) {
    return steps().insert(std::make_pair(stepInfo->id, stepInfo)).first->first;
}

MatchResult StepManager::stepMatches(const std::string &stepDescription) {
    MatchResult matchResult;
    for (steps_type::iterator iter = steps().begin(); iter != steps().end(); ++iter) {
        const boost::shared_ptr<const StepInfo>& stepInfo = iter->second;
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


InvokeResult BasicStep::invoke(const InvokeArgs *pArgs) {
    this->pArgs = pArgs;
    currentArgIndex = 0;
    currentResult = InvokeResult::success();
    embeddings.clear();
    try {
        InvokeResult returnedResult = invokeStepBody();
        if (currentResult.isPending()) {
            return currentResult;
        } else {
            returnedResult.setEmbeddings(embeddings);
            return returnedResult;
        }
    } catch (const std::exception& ex) {
        return InvokeResult::failure(ex.what(), embeddings);
    } catch (const std::string& ex) {
        return InvokeResult::failure(ex, embeddings);
    } catch (const char *ex) {
        return InvokeResult::failure(ex, embeddings);
    } catch (...) {
        // Cucumber needs a description here
        return InvokeResult::failure("Unknown exception", embeddings);
    }
}

void BasicStep::pending(const std::string& description) {
    currentResult = InvokeResult::pending(description);
}

void BasicStep::embed(const std::string& src, const std::string& mime, const std::string& label) {
    embeddings.push_back(Embedding(src, mime, label));
}

const InvokeArgs *BasicStep::getArgs() {
    return pArgs;
}

}
}

