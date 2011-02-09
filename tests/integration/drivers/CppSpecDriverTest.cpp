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

using namespace cukebins::internal;

int main(int argc, char **argv) {
    return DriverTest().run();
}
