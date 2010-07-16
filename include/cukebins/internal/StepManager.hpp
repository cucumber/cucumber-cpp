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
    step_id_type id;
    step_regex_type regex;

    StepInfo(const std::string &stepMatcher) :
        regex(stepMatcher.c_str()) {
        static step_id_type currentId = 0;
        id = ++currentId;
    }

    Match matches(const std::string &stepDescription);

    virtual void invokeStep() = 0;
};

class Step {
   virtual void invoke() = 0;
};

template<class T>
class StepInvoker : public StepInfo {
public:
   StepInvoker(const std::string &stepMatcher) : StepInfo(stepMatcher) {}

   void invokeStep() {
       T t;
       t.invoke();
   }
};

class Match {
public:
    step_id_type id;
    match_subexpressions_type subExpressions;

    Match() :
        id(0) {
    }

    Match(const Match &match) :
        id(match.id), subExpressions(match.subExpressions) {
    }

    Match & operator =(const Match &match) {
        id = match.id;
        subExpressions = match.subExpressions;
        return *this;
    }

    operator void *() {
        return (void *) id;
    }
};

struct SubExpression {
    std::string value;
    int position;
};

class MatchResult {
private:
    match_results_type resultSet;

public:
    operator void *() {
        return (void *) resultSet.size();
    }

    operator bool() {
        return !resultSet.empty();
    }

    const match_results_type getResultSet() {
        return resultSet;
    }

    void addMatch(Match match);
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
    typedef std::map<step_id_type, StepInfo *> steps_type;
    steps_type& steps();

public:
    virtual ~StepManager() {
    }

    void addStep(StepInfo *stepInfo) {
        steps().insert(std::make_pair(stepInfo->id, stepInfo));
    }

    MatchResult stepMatches(const std::string &stepDescription) {
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

    StepInfo *getStep(step_id_type id) {
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

template<class T>
static int registerStep(const std::string &stepMatcher) {
   StepManager s;
   StepInfo *stepInfo = new StepInvoker<T>(stepMatcher);
   s.addStep(stepInfo);
   return stepInfo->id;
}

}
}

#endif /* CUKEBINS_STEPMANAGER_HPP_ */
