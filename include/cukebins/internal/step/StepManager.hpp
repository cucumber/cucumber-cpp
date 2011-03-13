#ifndef CUKEBINS_STEPMANAGER_HPP_
#define CUKEBINS_STEPMANAGER_HPP_

#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include <boost/regex.hpp>
#include <boost/shared_ptr.hpp>

namespace cukebins {
namespace internal {

using boost::shared_ptr;

class Match;
struct SubExpression;

typedef unsigned int step_id_type;
typedef boost::regex step_regex_type;
typedef std::vector<Match> match_results_type;
typedef std::vector<SubExpression> match_subexpressions_type;

typedef std::vector<std::string> command_args_type; // TODO Rename


// FIXME
struct InvokeResult {
    InvokeResult() : success(false) {}

    bool success;
};


class StepInfo {
public:
    StepInfo(const std::string &stepMatcher);
    Match matches(const std::string &stepDescription);
    virtual InvokeResult invokeStep(shared_ptr<command_args_type> args) = 0;

    step_id_type id;
    step_regex_type regex;
};


class BasicStep {
public:
    InvokeResult invoke(shared_ptr<command_args_type> args);

protected:
    virtual const InvokeResult invokeStepBody() = 0;
    virtual void body() = 0;

    template<class T> T getInvokeArg(command_args_type::size_type i);
    template<class T> T getInvokeArg();

private:
    shared_ptr<command_args_type> invokeArgsPtr;
    command_args_type::size_type currentArgIndex; // TODO init to 0
};


template<class T>
class StepInvoker : public StepInfo {
public:
    StepInvoker(const std::string &stepMatcher);

    InvokeResult invokeStep(shared_ptr<command_args_type> args);
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

template<typename T>
T fromString(const std::string& s) {
    std::istringstream stream(s);
    T t;
    stream >> t;
    if (stream.fail()) {
        throw std::invalid_argument("Cannot convert parameter");
    }
    return t;
}

template<>
inline std::string fromString(const std::string& s) {
    return s;
}

template<typename T>
std::string toString(T arg) {
    std::stringstream s;
    s << arg;
    return s.str();
}

template<typename T>
T BasicStep::getInvokeArg(command_args_type::size_type i) {
    if (!invokeArgsPtr || i >= invokeArgsPtr->size()) {
        throw std::invalid_argument("Parameter not found");
    }
    return fromString<T>(invokeArgsPtr->at(i));
}

template<typename T>
T BasicStep::getInvokeArg() {
    return getInvokeArg<T>(currentArgIndex++);
}


template<class T>
StepInvoker<T>::StepInvoker(const std::string &stepMatcher) :
    StepInfo(stepMatcher) {
}

template<class T>
InvokeResult StepInvoker<T>::invokeStep(shared_ptr<command_args_type> args) {
    T t;
    return t.invoke(args);
}


}
}

#endif /* CUKEBINS_STEPMANAGER_HPP_ */
