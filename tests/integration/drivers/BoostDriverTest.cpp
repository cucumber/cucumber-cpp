#include <boost/test/unit_test.hpp>
#include <cukebins/cukebins.hpp>

#include "../../utils/DriverTestRunner.hpp"

THEN(SUCCEED_MATCHER) {
    USING_CONTEXT(cukebins::internal::SomeContext, ctx);
    BOOST_CHECK(true);
}

THEN(FAIL_MATCHER) {
    USING_CONTEXT(cukebins::internal::SomeContext, ctx);
    BOOST_CHECK(false);
}

using namespace cukebins::internal;

int main(int argc, char **argv) {
    return DriverTest<BoostCommands>().run();
}
