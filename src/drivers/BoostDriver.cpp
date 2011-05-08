#include <cukebins/internal/drivers/BoostDriver.hpp>

#include <boost/test/unit_test.hpp>

namespace cukebins {
namespace internal {

class BoostTestObserver : public ::boost::unit_test::test_observer {
public:
    const InvokeResult getResult();

private:
    void test_start(::boost::unit_test::counter_t amount);
    void assertion_result(bool passed);

    InvokeResult result;
};

void BoostTestObserver::test_start(::boost::unit_test::counter_t amount) {
    result = InvokeResult::success();
}

void BoostTestObserver::assertion_result(bool passed) {
    if (!passed) {
        result = InvokeResult::failure();
    }
}

const InvokeResult BoostTestObserver::getResult() {
    return result;
}

BoostTestObserver BoostStep::boostTestObserver;

namespace {

bool boost_test_init() {
    return true;
}

}

const InvokeResult BoostStep::invokeStepBody() {
    runWithMasterSuite();
    return boostTestObserver.getResult();
}

void BoostStep::initBoostTest() {
    using namespace ::boost::unit_test;
    if (!framework::is_initialized()) {
        int argc = 2;
        char *argv[] = { (char *) "", (char *) "--log_level=nothing" };
        framework::init(&boost_test_init, argc, argv);
        framework::register_observer(boostTestObserver);
    }
}

void BoostStep::runWithMasterSuite() {
    using namespace ::boost::unit_test;
    initBoostTest();
    test_case *tc = BOOST_TEST_CASE(boost::bind(&BoostStep::body, this));
    framework::master_test_suite().add(tc);
    framework::run(tc, false);
    framework::master_test_suite().remove(tc->p_id);
}

}
}
