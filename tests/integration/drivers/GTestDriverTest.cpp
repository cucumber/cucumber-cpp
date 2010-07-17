#include <gtest/gtest.h>
#include <cukebins/cukebins.hpp>

THEN("Succeed!") {
    ASSERT_TRUE(true);
}

THEN("Fail!") {
    ASSERT_TRUE(false);
}

#include "../../utils/DriverTestRunner.hpp"

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
    }
};

int main() {
    GTestDriverTest test;
    return test.run();
}
