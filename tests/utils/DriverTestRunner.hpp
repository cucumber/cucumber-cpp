#ifndef CUKEBINS_DRIVERTESTRUNNER_HPP_
#define CUKEBINS_DRIVERTESTRUNNER_HPP_

#include "StepManagerTestDouble.hpp"

#include <iostream>

namespace cukebins {
namespace internal {

class ContextListener {
private:
    static int createdContexts;
    static int destroyedContexts;
public:
    int getCreatedContexts() {
        return createdContexts;
    }
    int getDestroyedContexts() {
        return destroyedContexts;
    }
    void notifyCreation() {
        ++createdContexts;
    }
    void notifyDestruction() {
        ++destroyedContexts;
    }
    void reset() {
        createdContexts = 0;
        destroyedContexts = 0;
    };
};

int ContextListener::createdContexts = 0;
int ContextListener::destroyedContexts = 0;


class SomeContext {
private:
    ContextListener listener;
public:
    SomeContext() {
        listener.notifyCreation();
    }
    ~SomeContext() {
        listener.notifyDestruction();
    }
};


#define SUCCEED_MATCHER "Succeed!"
#define FAIL_MATCHER    "Fail!"


template<class T>
class DriverTest {
public:
    int run() {
        runAllTests();
        return failedTests;
    }

    DriverTest() {
        failedTests = 0;
    }
protected:
    void expectTrue(bool condition) {
        updateState(condition);
    }

    void expectFalse(bool condition) {
        updateState(!condition);
    }

    void expectEqual(int val1, int val2) {
        updateState(val1 == val2);
    }

    virtual void runAllTests() {
        invokeRunsTests();
        contextConstructorAndDesctructorGetCalled();
    }

    T cukeCommands;
private:
    StepManagerTestDouble stepManager;
    ContextListener listener;

    shared_ptr<command_args_type> no_args;
    int failedTests;

    void updateState(bool testSuccessState) {
        std::cout << (testSuccessState ? "SUCCESS" : "FAILURE") << std::endl;
        failedTests += testSuccessState ? 0 : 1;
    }

    step_id_type getStepIdFromMatcher(const std::string &stepMatcher) {
        return stepManager.getStepId(stepMatcher);
    }

    void invokeRunsTests() {
        cukeCommands.beginScenario();
        expectTrue(cukeCommands.invoke(getStepIdFromMatcher(SUCCEED_MATCHER), no_args).success);
        expectFalse(cukeCommands.invoke(42, no_args).success);
        expectFalse(cukeCommands.invoke(getStepIdFromMatcher(FAIL_MATCHER), no_args).success);
        cukeCommands.endScenario();
    }

    void contextConstructorAndDesctructorGetCalled() {
        contextConstructorAndDesctructorGetCalledOn(SUCCEED_MATCHER);
        contextConstructorAndDesctructorGetCalledOn(FAIL_MATCHER);
    }

    void contextConstructorAndDesctructorGetCalledOn(const std::string stepMatcher) {
        listener.reset();
        cukeCommands.beginScenario();
        cukeCommands.invoke(getStepIdFromMatcher(stepMatcher), no_args);
        expectEqual(1, listener.getCreatedContexts());
        expectEqual(0, listener.getDestroyedContexts());
        cukeCommands.endScenario();
        expectEqual(1, listener.getCreatedContexts());
        expectEqual(1, listener.getDestroyedContexts());
    }
};

}
}

#endif /* CUKEBINS_DRIVERTESTRUNNER_HPP_ */
