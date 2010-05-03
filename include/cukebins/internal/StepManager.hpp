#ifndef CUKEBINS_STEPMANAGER_HPP_
#define CUKEBINS_STEPMANAGER_HPP_

#include <string>
#include <vector>
#include <map>

#include <boost/regex.hpp>

namespace cukebins {
namespace internal {

struct SubExpression {
    std::string value;
    int position;
};

class Match;

class MatchResult {
public:
    typedef std::vector<Match> results_type;

private:
    results_type resultSet;

public:
    operator void *() {
        return (void *) resultSet.size();
    }

    operator bool() {
        return !resultSet.empty();
    }

    const results_type getResultSet() {
        return resultSet;
    }

    void addMatch(Match match);
};

class StepInfo {
public:
    typedef unsigned int id_type;
    typedef boost::regex regex_type;

    id_type id;
    regex_type regex;
    std::string testName;

    StepInfo() :
        id(0) {
    }
    ;
    StepInfo(const std::string &stepMatcher, const std::string &testName) :
        regex(stepMatcher.c_str()), testName(testName) {
        static id_type currentId = 0;
        id = ++currentId;
    }
    ;
    StepInfo(const StepInfo &stepInfo) :
        regex(stepInfo.regex), id(stepInfo.id), testName(stepInfo.testName) {
    }
    ;

    Match matches(const std::string &stepDescription);
};

class Match {
public:
    typedef std::vector<SubExpression> subexpressions_type;

    StepInfo::id_type id;
    subexpressions_type subExpressions;

    Match() :
        id(0) {
    }
    ;
    Match(const Match &match) :
        id(match.id), subExpressions(match.subExpressions) {
    }
    ;
    Match & operator =(const Match &match) {
        id = match.id;
        subExpressions = match.subExpressions;
        return *this;
    }

    operator void *() {
        return (void *) id;
    }
};

void MatchResult::addMatch(Match match) {
    resultSet.push_back(match);
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

class StepManager {
protected:
    typedef std::map<StepInfo::id_type, StepInfo> steps_type;
    steps_type& steps();

    void addStep(StepInfo &stepInfo) {
        steps().insert(std::make_pair(stepInfo.id, stepInfo));
    }
public:
    virtual ~StepManager() {
    }
    ;

    StepInfo::id_type addStepDefinition(const std::string &stepMatcher, const std::string &testName) {
        StepInfo stepInfo(stepMatcher, testName);
        addStep(stepInfo);
        return stepInfo.id;
    }

    MatchResult stepMatches(const std::string &stepDescription) {
        MatchResult matchResult;
        for (steps_type::iterator iter = steps().begin(); iter != steps().end(); ++iter) {
            StepInfo *stepInfo = &(iter->second);
            Match currentMatch = stepInfo->matches(stepDescription);
            if (currentMatch) {
                matchResult.addMatch(currentMatch);
            }
        }
        return matchResult;
    }

    StepInfo getStep(StepInfo::id_type id) {
        return steps()[id];
    }
};

/**
 * Needed to fix the "static initialization order fiasco"
 * http://www.parashift.com/c++-faq-lite/ctors.html#faq-10.12
 */
inline StepManager::steps_type& StepManager::steps() {
    static steps_type *steps = new steps_type();
    return *steps;
}

static int registerStep(const std::string &stepMatcher, const std::string &testName) {
    StepManager s;
    return s.addStepDefinition(stepMatcher, testName);
}

}
}

#endif /* CUKEBINS_STEPMANAGER_HPP_ */
