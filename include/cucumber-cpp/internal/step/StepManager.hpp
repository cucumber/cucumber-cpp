#ifndef CUKE_STEPMANAGER_HPP_
#define CUKE_STEPMANAGER_HPP_

#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include <boost/config.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>

#ifndef BOOST_NO_VARIADIC_TEMPLATES
    #include <type_traits>
#endif

#include <cucumber-cpp/internal/CukeExport.hpp>
#include "../Table.hpp"
#include "../utils/IndexSequence.hpp"
#include "../utils/Regex.hpp"

namespace cucumber {
namespace internal {

typedef unsigned int step_id_type;

class StepInfo;

class CUCUMBER_CPP_EXPORT SingleStepMatch {
public:
    typedef RegexMatch::submatches_type submatches_type;

    operator const void *() const;

    boost::shared_ptr<const StepInfo> stepInfo;
    submatches_type submatches;
};

class CUCUMBER_CPP_EXPORT MatchResult {
public:
    typedef std::vector<SingleStepMatch> match_results_type;

    const match_results_type& getResultSet();
    void addMatch(SingleStepMatch match);

#ifndef BOOST_NO_EXPLICIT_CONVERSION_OPERATORS
    explicit
#endif
    operator bool() const;

private:
    match_results_type resultSet;
};

class CUCUMBER_CPP_EXPORT InvokeArgs {
    typedef std::vector<std::string> args_type;
public:
    typedef args_type::size_type size_type;
    
    InvokeArgs() { }
 
    void addArg(const std::string arg);
    Table & getVariableTableArg();

    template<class T> T getInvokeArg(size_type i) const;
    const Table & getTableArg() const;
private:
    Table tableArg;
    args_type args;
};

enum InvokeResultType {
    SUCCESS,
    FAILURE,
    PENDING
};

class CUCUMBER_CPP_EXPORT InvokeResult {
private:
    InvokeResultType type;
    std::string description;

    InvokeResult(const InvokeResultType type, const char *description);
public:
    InvokeResult();
    InvokeResult(const InvokeResult &ir);
    InvokeResult& operator= (const InvokeResult& rhs);

    static InvokeResult success();
    static InvokeResult failure(const char *description);
    static InvokeResult failure(const std::string &description);
    static InvokeResult pending(const char *description);

    bool isSuccess() const;
    bool isPending() const;
    InvokeResultType getType() const;
    const std::string &getDescription() const;
};

class CUCUMBER_CPP_EXPORT StepInfo : public boost::enable_shared_from_this<StepInfo> {
public:
    StepInfo(const std::string &stepMatcher, const std::string source);
    SingleStepMatch matches(const std::string &stepDescription) const;
    virtual InvokeResult invokeStep(const InvokeArgs * pArgs) const = 0;

    step_id_type id;
    Regex regex;
    const std::string source;
private:
    // Shut up MSVC warning C4512: assignment operator could not be generated
    StepInfo& operator=(const StepInfo& other);
};

class CUCUMBER_CPP_EXPORT BasicStep {
public:
    InvokeResult invoke(const InvokeArgs *pArgs);

protected:
    typedef const Table table_type;
    typedef const Table::hashes_type table_hashes_type;

    virtual const InvokeResult invokeStepBody() = 0;
    virtual void body() = 0;

    void pending(const char *description);
    void pending();

    template<class T> const T getInvokeArg();
    const InvokeArgs *getArgs();

#ifdef BOOST_NO_VARIADIC_TEMPLATES
    // Special case for zero arguments, only thing we bother to support on C++98
    template <typename Derived, typename R>
    static R invokeWithArgs(Derived& that, R (Derived::* f)()) {
        return (that.*f)();
    }
#else
    template <typename Derived, typename R, typename... Args, std::size_t... N>
    static R invokeWithIndexedArgs(Derived& that, R (Derived::* f)(Args...), index_sequence<N...>) {
        return (that.*f)(
                that.pArgs->template getInvokeArg<typename std::decay<Args>::type>(N)...
            );
    }

    template <typename Derived, typename R, typename... Args>
    static R invokeWithArgs(Derived& that, R (Derived::* f)(Args...)) {
        that.currentArgIndex = sizeof...(Args);
        return invokeWithIndexedArgs(
                that,
                f,
                index_sequence_for<Args...>{});
    }
#endif

private:
    // FIXME: awful hack because of Boost::Test
    InvokeResult currentResult;

    const InvokeArgs *pArgs;
    InvokeArgs::size_type currentArgIndex;
};


template<class T>
class StepInvoker : public StepInfo {
public:
    StepInvoker(const std::string &stepMatcher, const std::string source);

    InvokeResult invokeStep(const InvokeArgs *args) const;
};

class CUCUMBER_CPP_EXPORT StepManager {
protected:
    typedef std::map<step_id_type, boost::shared_ptr<const StepInfo> > steps_type;

public:
    static step_id_type addStep(boost::shared_ptr<StepInfo> stepInfo);
    static MatchResult stepMatches(const std::string &stepDescription);
    static const StepInfo* getStep(step_id_type id);
protected:
    static steps_type& steps();

private:
    // We're a singleton so don't allow instances
    StepManager()
#ifndef BOOST_NO_DELETED_FUNCTIONS
        = delete
#endif
        ;
};


static inline std::string toSourceString(const char *filePath, const int line) {
    using namespace std;
    stringstream s;
    string file(filePath);
    string::size_type pos = file.find_last_of("/\\");
    if (pos == string::npos) {
        s << file;
    } else {
       s << file.substr(++pos);
    }
    s << ":" << line;
    return s.str();
}

template<class T>
static int registerStep(const std::string &stepMatcher, const char *file, const int line) {
   return StepManager::addStep(boost::make_shared< StepInvoker<T> >(stepMatcher, toSourceString(file, line)));
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
T InvokeArgs::getInvokeArg(size_type i) const {
    if (i >= args.size()) {
        throw std::invalid_argument("Parameter not found");
    }
    return fromString<T>(args.at(i));
}


template<typename T>
const T BasicStep::getInvokeArg() {
    return pArgs->getInvokeArg<T>(currentArgIndex++);
}


template<class T>
StepInvoker<T>::StepInvoker(const std::string &stepMatcher, const std::string source) :
    StepInfo(stepMatcher, source) {
}

template<class T>
InvokeResult StepInvoker<T>::invokeStep(const InvokeArgs *pArgs) const {
    T t;
    return t.invoke(pArgs);
}


}
}

#endif /* CUKE_STEPMANAGER_HPP_ */
