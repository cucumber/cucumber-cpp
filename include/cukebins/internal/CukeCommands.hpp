#ifndef CUKEBINS_CUKECOMMANDS_HPP_
#define CUKEBINS_CUKECOMMANDS_HPP_

#include "StepManager.hpp"
#include "ContextManager.hpp"

#include <map>
#include <string>
#include <sstream>

namespace cukebins {
namespace internal {

struct InvokeResult {
    bool success;
};

class AbstractCommands {
public:
    typedef std::vector<std::string> args_type;

private:
    static shared_ptr<args_type> invokeArgsPtr;

protected:
    StepManager stepManager;
    ContextManager contextManager;

    virtual InvokeResult invokeNoArgs(StepInfo::id_type id) = 0;
public:
    virtual void beginScenario() = 0;
    //	virtual void snippetText() = 0;

    MatchResult stepMatches(const std::string description) {
        return stepManager.stepMatches(description);
    }
    // don't override! virtual for testin purpose
    virtual InvokeResult invoke(StepInfo::id_type id, shared_ptr<args_type> args) {
        invokeArgsPtr = args;
        InvokeResult result = invokeNoArgs(id);
        invokeArgsPtr.reset();
        return result;
    }
    void endScenario() {
        contextManager.purgeContexts();
    }

private:
    template<class T>
    friend T getInvokeArg(args_type::size_type i);
};

template<class T>
T fromString(const std::string& s) {
    std::istringstream stream(s);
    T t;
    stream >> t;
    if (stream.fail()) {
        throw std::invalid_argument("Cannot convert parameter");
    }
    return t;
}

template<class T>
std::string toString(T arg) {
    std::stringstream s;
    s << arg;
    return s.str();
}

template<class T>
T getInvokeArg(AbstractCommands::args_type::size_type i) {
    if (!AbstractCommands::invokeArgsPtr || i > AbstractCommands::invokeArgsPtr->size()) {
        throw std::invalid_argument("Parameter not found");
    }
    return fromString<T> (AbstractCommands::invokeArgsPtr->at(i - 1));
}

shared_ptr<AbstractCommands::args_type> AbstractCommands::invokeArgsPtr;

#define CUKE_PARAM(index, type, name) type name = ::cukebins::internal::getInvokeArg<type>(index)

}
}

#endif /* CUKEBINS_CUKECOMMANDS_HPP_ */
