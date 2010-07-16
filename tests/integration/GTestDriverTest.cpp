// GTest needs to be included before cukebins to register GTest tests
#include <gtest/gtest.h>
#include <cukebins/cukebins.hpp>

#include <iostream>

using namespace std;
using namespace cukebins::internal;

class GTestCommandsDouble : public GTestCommands {
public:
    bool isInitialized() {
        return initialized;
    }
};

class StepManagerDouble : public StepManager {
public:
    const step_id_type getStepId(const string &stepMatcher) {
        return getStepInfoFromMatcher(stepMatcher)->id;
    }
private:
    const StepInfo *getStepInfoFromMatcher(const string &stepMatcher) {
        for (steps_type::const_iterator i = steps().begin(); i != steps().end(); ++i) {
			StepInfo *stepInfo = i->second;
            if (stepInfo->regex.str() == stepMatcher) {
                return stepInfo;
            }
        }
    }
};

THEN("Succeed!") {
    ASSERT_TRUE(true);
}

THEN("Fail!") {
    ASSERT_TRUE(false);
}

class GTestDriverTest {
public:
    bool beginScenarioInitsGTest() {
        cukeCommands.beginScenario();
        return gTestIsReady();
    }

    bool invokeRunsTests() {
        return cukeCommands.invoke(getStepIdFromMatcher("Succeed!"), no_args).success &&
            !cukeCommands.invoke(42, no_args).success &&
            !cukeCommands.invoke(getStepIdFromMatcher("Fail!"), no_args).success;
    }
private:
    GTestCommandsDouble cukeCommands;
    StepManagerDouble stepManager;
    shared_ptr<command_args_type> no_args;

    bool gTestIsReady() {
        return cukeCommands.isInitialized();
    }

    step_id_type getStepIdFromMatcher(const string &stepMatcher) {
        return stepManager.getStepId(stepMatcher);
    }
};

int main() {
    GTestDriverTest test;
    cout << "beginScenarioInitsGTest " << flush;
    cout << (test.beginScenarioInitsGTest() ? "OK" : "NO") << endl;
    cout << "invokeRunsTests " << flush;
    cout << (test.invokeRunsTests() ? "OK" : "NO") << endl;
    return 0;
}

