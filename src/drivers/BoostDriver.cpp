#include <cukebins/internal/drivers/BoostDriver.hpp>

#include <boost/test/unit_test.hpp>

namespace cukebins {
namespace internal {

class BoostTestObserver : public ::boost::unit_test::test_observer {
public:
    const bool lastTestSucceded();

private:
    void test_start(boost::unit_test::counter_t amount);
    void assertion_result(bool passed);

    unsigned int failureCount;
};

void BoostTestObserver::test_start(boost::unit_test::counter_t amount) {
    failureCount = 0;
}

void BoostTestObserver::assertion_result(bool passed) {
    if (!passed) {
        ++failureCount;
    }
}

const bool BoostTestObserver::lastTestSucceded() {
    return (failureCount == 0);
}

BoostTestObserver BoostStep::boostTestObserver;

namespace {

bool boost_test_init() {
    return true;
}

}

const InvokeResult BoostStep::invokeStepBody() {
    InvokeResult result;
    try {
        runWithMasterSuite();
        result.success = boostTestObserver.lastTestSucceded();
    } catch (...) {
    }
    return result;
}

void BoostStep::initBoostTest() {
    using namespace boost::unit_test;
    if (!framework::is_initialized()) {
        int argc = 2;
        char *argv[] = { (char *) "", (char *) "--log_level=nothing" };
        framework::init(&boost_test_init, argc, argv);
        framework::register_observer(boostTestObserver);
    }
}

void BoostStep::runWithMasterSuite() {
    using namespace boost::unit_test;
    initBoostTest();
    test_case *tc = BOOST_TEST_CASE(boost::bind(&BoostStep::body, this));
    framework::master_test_suite().add(tc);
    framework::run(tc, false);
    framework::master_test_suite().remove(tc->p_id);
}

}
}
