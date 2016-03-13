#include <boost/version.hpp>
#include <boost/test/unit_test.hpp>
#include <cucumber-cpp/autodetect.hpp>

#include "../../utils/DriverTestRunner.hpp"

using namespace cucumber;

THEN(SUCCEED_MATCHER) {
    ScenarioScope<SomeContext> ctx;
    BOOST_CHECK(true);
}

THEN(FAIL_MATCHER) {
    ScenarioScope<SomeContext> ctx;
    BOOST_CHECK(false);
}

THEN(PENDING_MATCHER_1) {
    pending();
}

THEN(PENDING_MATCHER_2) {
    pending(PENDING_DESCRIPTION);
}

using namespace cucumber::internal;

#if BOOST_VERSION >= 105900
namespace boost {
    namespace unit_test {
        namespace framework {
            bool is_initialized() {
                return boost::unit_test::framework::master_test_suite().argc > 0; 
            }
        }
    }
}
#endif

class BoostStepDouble : public BoostStep {
public:
    const InvokeResult invokeStepBody() {
        return BoostStep::invokeStepBody();
    };

    void body() {};
};

class BoostDriverTest : public DriverTest {
public:
    virtual void runAllTests() {
        stepInvocationInitsBoostTest();
        DriverTest::runAllTests();
    }

private:
    void stepInvocationInitsBoostTest() {
        std::cout << "= Init =" << std::endl;
        using namespace boost::unit_test;
        BoostStepDouble step;
        expectFalse("Framework is not initialized before the first test", framework::is_initialized());
        step.invokeStepBody();
        expectTrue("Framework is initialized after the first test", framework::is_initialized());
    }
};

int main(int argc, char **argv) {
    BoostDriverTest test;
    return test.run();
}
