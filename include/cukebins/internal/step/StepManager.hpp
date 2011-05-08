#ifndef CUKEBINS_STEPMANAGER_HPP_
#define CUKEBINS_STEPMANAGER_HPP_

#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "../utils/Regex.hpp"

namespace cukebins {
namespace internal {

typedef unsigned int step_id_type;

typedef std::vector<std::string> command_args_type; // TODO Rename


class SingleStepMatch {
public:
    typedef RegexMatch::submatches_type submatches_type;

    SingleStepMatch();
    SingleStepMatch(const SingleStepMatch &match);

    SingleStepMatch & operator =(const SingleStepMatch &match);
    operator void *();

    step_id_type id;
    submatches_type submatches;
};

class MatchResult {
public:
    typedef std::vector<SingleStepMatch> match_results_type;

    const match_results_type getResultSet();
    void addMatch(SingleStepMatch match);

    operator void *();
    operator bool();

private:
    match_results_type resultSet;
};

namespace {
    enum InvokeResultType {
        SUCCESS,
        FAILURE,
        PENDING
    };
}

class InvokeResult {
private:
    InvokeResultType type;
    const char *description;

    InvokeResult(const InvokeResultType type, const char *description);
public:
    InvokeResult();
    InvokeResult(const InvokeResult &ir);
    InvokeResult& operator= (const InvokeResult& rhs);

    static InvokeResult success();
    static InvokeResult failure();
    static InvokeResult pending(const char *description);

    bool isSuccess();
    bool isPending();
    const char *getType() const;
    const char *getDescription() const;
};


class StepInfo {
public:
    StepInfo(const std::string &stepMatcher);
    SingleStepMatch matches(const std::string &stepDescription);
    virtual InvokeResult invokeStep(command_args_type *args) = 0;

    step_id_type id;
    Regex regex;
};


class BasicStep {
public:
    InvokeResult invoke(command_args_type *args);

protected:
    virtual const InvokeResult invokeStepBody() = 0;
    virtual void body() = 0;

    void pending(const char *description);
    void pending();

    template<class T> T getInvokeArg(command_args_type::size_type i);
    template<class T> T getInvokeArg();

private:
    command_args_type *invokeArgsPtr;
    command_args_type::size_type currentArgIndex; // TODO init to 0
};


template<class T>
class StepInvoker : public StepInfo {
public:
    StepInvoker(const std::string &stepMatcher);

    InvokeResult invokeStep(command_args_type *args);
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
InvokeResult StepInvoker<T>::invokeStep(command_args_type *args) {
    T t;
    return t.invoke(args);
}


}
}

#endif /* CUKEBINS_STEPMANAGER_HPP_ */
