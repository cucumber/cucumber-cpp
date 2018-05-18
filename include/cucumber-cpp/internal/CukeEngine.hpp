#ifndef CUKE_CUKEENGINE_HPP_
#define CUKE_CUKEENGINE_HPP_

#include <cstddef>
#include <string>
#include <vector>

#include <boost/multi_array.hpp>

#include <cucumber-cpp/internal/CukeExport.hpp>

namespace cucumber {
namespace internal {

class CUCUMBER_CPP_EXPORT StepMatchArg {
public:
    std::string value;
    std::ptrdiff_t position;
};

class CUCUMBER_CPP_EXPORT StepMatch {
public:
    std::string id;
    std::vector<StepMatchArg> args;
    std::string source;
    std::string regexp;
};

class CUCUMBER_CPP_EXPORT InvokeException {
private:
    const std::string message;

public:
    InvokeException(const std::string & message);
    InvokeException(const InvokeException &rhs);

    const std::string getMessage() const;

    virtual ~InvokeException() {}
};

class CUCUMBER_CPP_EXPORT InvokeFailureException : public InvokeException {
private:
    const std::string exceptionType;

public:
    InvokeFailureException(const std::string & message, const std::string & exceptionType);
    InvokeFailureException(const InvokeFailureException &rhs);

    const std::string getExceptionType() const;
};

class CUCUMBER_CPP_EXPORT PendingStepException : public InvokeException {
public:
    PendingStepException(const std::string & message);
    PendingStepException(const PendingStepException &rhs);
};

/**
 * The entry point to Cucumber.
 *
 * It uses standard types (as much as possible) to be easier to call.
 * Returns standard types if possible.
 */
class CukeEngine {
private:
    typedef std::vector<std::string> string_array;
    typedef boost::multi_array<std::string, 2> string_2d_array;
public:
    typedef string_array tags_type;
    typedef string_array invoke_args_type;
    typedef string_2d_array invoke_table_type;

    /**
     * Finds steps whose regexp match some text.
     */
    virtual std::vector<StepMatch> stepMatches(const std::string & name) const = 0;

    /**
     * Starts a scenario.
     */
    virtual void beginScenario(const tags_type & tags) = 0;

    /**
     * Invokes a step passing arguments to it.
     *
     * @throws InvokeException if the test fails or it is pending
     */
    virtual void invokeStep(const std::string & id, const invoke_args_type & args, const invoke_table_type & tableArg) = 0;

    /**
     * Ends a scenario.
     */
    virtual void endScenario(const tags_type & tags) = 0;

    /**
     * Returns the step definition for a pending step.
     */
    virtual std::string snippetText(const std::string & keyword, const std::string & name, const std::string & multilineArgClass) const = 0;

    CUCUMBER_CPP_EXPORT CukeEngine();
    CUCUMBER_CPP_EXPORT virtual ~CukeEngine();
};

}
}

#endif /* CUKE_CUKEENGINE_HPP_ */
