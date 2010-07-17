#ifndef CUKEBINS_DRIVERTESTRUNNER_HPP_
#define CUKEBINS_DRIVERTESTRUNNER_HPP_

#include "StepManagerTestDouble.hpp"

#include <iostream>

namespace cukebins {
namespace internal {

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

    virtual void runAllTests() {
        invokeRunsTests();
    }

    T cukeCommands;
private:
    StepManagerTestDouble stepManager;
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
        expectTrue(cukeCommands.invoke(getStepIdFromMatcher("Succeed!"), no_args).success);
        expectFalse(cukeCommands.invoke(42, no_args).success);
        expectFalse(cukeCommands.invoke(getStepIdFromMatcher("Fail!"), no_args).success);
    }
};

}
}

#endif /* CUKEBINS_DRIVERTESTRUNNER_HPP_ */
