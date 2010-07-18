#include <gtest/gtest.h>
#include <cukebins/cukebins.hpp>

#include "../../utils/DriverTestRunner.hpp"

THEN(SUCCEED_MATCHER) {
    USING_CONTEXT(cukebins::internal::SomeContext, ctx);
    ASSERT_TRUE(true);
}

THEN(FAIL_MATCHER) {
    USING_CONTEXT(cukebins::internal::SomeContext, ctx);
    ASSERT_TRUE(false);
}

using namespace cukebins::internal;

class GTestCommandsDouble : public GTestCommands {
public:
    bool isInitialized() {
        return initialized;
    }
};

class GTestDriverTest : public DriverTest<GTestCommandsDouble> {
public:
    virtual void runAllTests() {
        beginScenarioInitsGTest();
        DriverTest<GTestCommandsDouble>::runAllTests();
    }

private:
    void beginScenarioInitsGTest() {
        cukeCommands.beginScenario();
        expectTrue(cukeCommands.isInitialized());
        cukeCommands.endScenario();
    }
};

int main() {
    GTestDriverTest test;
    return test.run();
}
