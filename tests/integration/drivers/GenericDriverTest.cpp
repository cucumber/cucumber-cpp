#include <cucumber-cpp/generic.hpp>

#include "../../utils/DriverTestRunner.hpp"

using namespace cucumber;

THEN(SUCCEED_MATCHER) {
    ScenarioScope<SomeContext> ctx;
    // Do not throw for successes
}

THEN(FAIL_MATCHER) {
    ScenarioScope<SomeContext> ctx;
    throw std::runtime_error("Failure description");
}

THEN(PENDING_MATCHER_1) {
    pending();
}

THEN(PENDING_MATCHER_2) {
    pending(PENDING_DESCRIPTION);
}

using namespace cucumber::internal;

int main(int argc, char **argv) {
    return DriverTest().run();
}
