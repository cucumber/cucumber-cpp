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
        using namespace boost::unit_test;
        BoostStepDouble step;
	expectFalse(framework::is_initialized());
	step.invokeStepBody();
        expectTrue(framework::is_initialized());
    }
};

int main(int argc, char **argv) {
    BoostDriverTest test;
    return test.run();
}
