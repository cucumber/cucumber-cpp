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
    }
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

static const InvokeArgs NO_INVOKE_ARGS;

#define SUCCEED_MATCHER   "Succeed!"
#define FAIL_MATCHER      "Fail!"
#define PENDING_MATCHER_1 "Pending without description"
#define PENDING_MATCHER_2 "Pending with description"

#define PENDING_DESCRIPTION    "Describe me!"
#define NO_PENDING_DESCRIPTION (const char *) 0

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

    template<typename T>
    void expectEqual(T val1, T val2) {
        updateState(val1 == val2);
    }

    virtual void runAllTests() {
        invokeRunsTests();
        contextConstructorAndDesctructorGetCalled();
    }

    CukeCommands cukeCommands;
private:
    StepManagerTestDouble stepManager;
    ContextListener listener;

    int failedTests;

    void updateState(bool testSuccessState) {
        std::cout << (testSuccessState ? "SUCCESS" : "FAILURE") << std::endl;
        failedTests += testSuccessState ? 0 : 1;
    }

    step_id_type getStepIdFromMatcher(const std::string &stepMatcher) {
        return stepManager.getStepId(stepMatcher);
    }

    void invokeRunsTests() {
        InvokeResult result;

        cukeCommands.beginScenario(0);

        result = cukeCommands.invoke(getStepIdFromMatcher(SUCCEED_MATCHER), &NO_INVOKE_ARGS);
        expectTrue(result.isSuccess());

        result = cukeCommands.invoke(getStepIdFromMatcher(FAIL_MATCHER), &NO_INVOKE_ARGS);
        expectFalse(result.isSuccess());
        expectFalse(result.isPending());

        result = cukeCommands.invoke(getStepIdFromMatcher(PENDING_MATCHER_1), &NO_INVOKE_ARGS);
        expectTrue(result.isPending());
        expectEqual(NO_PENDING_DESCRIPTION, result.getDescription());

        result = cukeCommands.invoke(getStepIdFromMatcher(PENDING_MATCHER_2), &NO_INVOKE_ARGS);
        expectTrue(result.isPending());
        expectEqual(PENDING_DESCRIPTION, result.getDescription());

        result = cukeCommands.invoke(42, &NO_INVOKE_ARGS);
        expectFalse(result.isSuccess());

        cukeCommands.endScenario();
    }

    void contextConstructorAndDesctructorGetCalled() {
        contextConstructorAndDesctructorGetCalledOn(SUCCEED_MATCHER);
        contextConstructorAndDesctructorGetCalledOn(FAIL_MATCHER);
    }

    void contextConstructorAndDesctructorGetCalledOn(const std::string stepMatcher) {
        listener.reset();
        cukeCommands.beginScenario(0);
        cukeCommands.invoke(getStepIdFromMatcher(stepMatcher), &NO_INVOKE_ARGS);
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
