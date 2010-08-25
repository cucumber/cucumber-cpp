#ifndef CUKEBINS_STEPMANAGER_HPP_
#define CUKEBINS_STEPMANAGER_HPP_

#include <string>
#include <vector>
#include <map>

#include <boost/regex.hpp>

namespace cukebins {
namespace internal {

class Match;
struct SubExpression;

typedef unsigned int step_id_type;
typedef boost::regex step_regex_type;
typedef std::vector<Match> match_results_type;
typedef std::vector<SubExpression> match_subexpressions_type;

class StepInfo {
public:
    StepInfo(const std::string &stepMatcher);
    Match matches(const std::string &stepDescription);
    virtual void invokeStep() = 0;

    step_id_type id;
    step_regex_type regex;
};

/**
 * Used when the steps do not need inheritance
 */
class BasicStep {
   virtual void invoke() = 0;
};

template<class T>
class StepInvoker : public StepInfo {
public:
    StepInvoker(const std::string &stepMatcher);

    void invokeStep();
};


class Match {
public:
    Match();
    Match(const Match &match);

    Match & operator =(const Match &match);
    operator void *();

    step_id_type id;
    match_subexpressions_type subExpressions;
};

struct SubExpression {
    std::string value;
    int position;
};

class MatchResult {
public:
    const match_results_type getResultSet();
    void addMatch(Match match);

    operator void *();
    operator bool();

private:
    match_results_type resultSet;
};

class StepManager {
protected:
    typedef std::map<step_id_type, StepInfo *> steps_type;

public:
    virtual ~StepManager();

    void addStep(StepInfo *stepInfo);
    MatchResult stepMatches(const std::string &stepDescription);
    StepInfo *getStep(step_id_type id);
protected:
    steps_type& steps();
};


template<class T>
static int registerStep(const std::string &stepMatcher) {
   StepManager s;
   StepInfo *stepInfo = new StepInvoker<T>(stepMatcher);
   s.addStep(stepInfo);
   return stepInfo->id;
}


template<class T>
StepInvoker<T>::StepInvoker(const std::string &stepMatcher) :
    StepInfo(stepMatcher) {
}

template<class T>
void StepInvoker<T>::invokeStep() {
    T t;
    t.invoke();
}

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
inline StepManager::steps_type& StepManager::steps() {
    static steps_type *steps = new steps_type();
    return *steps;
}

}
}

#endif /* CUKEBINS_STEPMANAGER_HPP_ */
