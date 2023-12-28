#include <QtTest>
#include <cucumber-cpp/autodetect.hpp>

#include "utils/DriverTestRunner.hpp"

using namespace cucumber;

THEN(SUCCEED_MATCHER) {
    ScenarioScope<SomeContext> ctx;
    QVERIFY(true);
}

THEN(FAIL_MATCHER) {
    ScenarioScope<SomeContext> ctx;
    QVERIFY(false);
}

THEN(PENDING_MATCHER_1) {
    pending();
}

THEN(PENDING_MATCHER_2) {
    pending(PENDING_DESCRIPTION);
}

using namespace cucumber::internal;

class QtTestStepDouble : public QtTestStep {
public:
    QtTestStepDouble() :
        QtTestStep(),
        testRun(false) {
    }

    const InvokeResult invokeStepBody() override {
        return QtTestStep::invokeStepBody();
    }

    void body() override {
        testRun = true;
    }

    bool testRun;
};

class QtTestDriverTest : public DriverTest {
public:
    void runAllTests() override {
        stepInvocationRunsStepBody();
        DriverTest::runAllTests();
    }

private:
    void stepInvocationRunsStepBody() {
        QtTestStepDouble framework;
        expectFalse("The test body has not been run", framework.testRun);
        framework.invokeStepBody();
        expectTrue("The test body has been run", framework.testRun);
    }
};

int main() {
    QtTestDriverTest test;
    return test.run();
}
