#include <CppSpec/CppSpec.h>
#include <cukebins/cukebins.hpp>

#include "../../utils/DriverTestRunner.hpp"

THEN(SUCCEED_MATCHER) {
    USING_CONTEXT(cukebins::internal::SomeContext, ctx);
    specify(true, should.equal(true));
}

THEN(FAIL_MATCHER) {
    USING_CONTEXT(cukebins::internal::SomeContext, ctx);
    specify(true, should.equal(false));
}

THEN(PENDING_MATCHER_1) {
    pending();
}

THEN(PENDING_MATCHER_2) {
    pending(PENDING_DESCRIPTION);
}

using namespace cukebins::internal;

int main(int argc, char **argv) {
    return DriverTest().run();
}
