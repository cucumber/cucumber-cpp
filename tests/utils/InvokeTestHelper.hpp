#ifndef CUKEBINS_INVOKETEST_HPP_
#define CUKEBINS_INVOKETEST_HPP_

#include <cukebins/internal/CukeCommands.hpp>
#include <cukebins/internal/drivers/FakeDriver.hpp>

namespace cukebins {
namespace internal {

class InvokeTestHelper {
public:
    typedef AbstractCommands::args_type args_type;
public:
    virtual args_type *buildInvokeArgs() = 0;
    virtual void testBeforeInvoke() = 0;
    virtual void testOnInvoke() = 0;
    virtual void testAfterInvoke() = 0;
};

template<class T>
class CukeCommandsTestInjector : public CukeCommands {
private:
    T testMethods;
    shared_ptr<args_type> args;
public:
    CukeCommandsTestInjector() :
        args(testMethods.buildInvokeArgs()) {
    }
    InvokeResult buildArgsAndInvoke() {
        testMethods.testBeforeInvoke();
        InvokeResult result = CukeCommands::invoke(0, args);
        testMethods.testAfterInvoke();
        return result;
    }
    InvokeResult invokeNoArgs(StepInfo::id_type id) {
        testMethods.testOnInvoke();
        return CukeCommands::invokeNoArgs(id);
    }
};

}
}

#endif /* CUKEBINS_INVOKETEST_HPP_ */
