#include <CppSpec/CppSpec.h>
#include <cucumber-cpp/defs.hpp>

#include "../../utils/DriverTestRunner.hpp"

using namespace cucumber;

THEN(SUCCEED_MATCHER) {
    ScenarioScope<SomeContext> ctx;
    specify(true, should.equal(true));
}

THEN(FAIL_MATCHER) {
    ScenarioScope<SomeContext> ctx;
    specify(true, should.equal(false));
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
