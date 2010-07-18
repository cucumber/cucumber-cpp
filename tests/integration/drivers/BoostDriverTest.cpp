#include <boost/test/unit_test.hpp>
#include <cukebins/cukebins.hpp>

THEN("Succeed!") {
    BOOST_CHECK(true);
}

THEN("Fail!") {
    BOOST_CHECK(false);
}

#include "../../utils/DriverTestRunner.hpp"

using namespace cukebins::internal;

int main(int argc, char **argv) {
    return DriverTest<BoostCommands>().run();
}
