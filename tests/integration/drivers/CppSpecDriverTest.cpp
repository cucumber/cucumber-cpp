#include <CppSpec/CppSpec.h>
#include <cukebins/cukebins.hpp>

THEN("Succeed!") {
    specify(true, should.equal(true));
}

THEN("Fail!") {
    specify(true, should.equal(false));
}

#include "../../utils/DriverTestRunner.hpp"

using namespace cukebins::internal;

int main(int argc, char **argv) {
    return DriverTest<CppSpecCommands>().run();
}
