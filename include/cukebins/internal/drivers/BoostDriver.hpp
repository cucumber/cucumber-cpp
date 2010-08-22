#ifndef CUKEBINS_BOOSTDRIVER_HPP_
#define CUKEBINS_BOOSTDRIVER_HPP_

#include <boost/bind.hpp>
#include <cukebins/internal/CukeCommands.hpp>

namespace cukebins {
namespace internal {

namespace {

bool boost_test_init() {
    return true;
}

}

class BoostCommands : public AbstractCommands, public boost::unit_test::test_observer {
protected:
    const InvokeResult invokeNoArgs(StepInfo *stepInfo) {
        InvokeResult result;
        try {
            runWithMasterSuite(stepInfo);
            result.success = (failureCount == 0);
        } catch (...) {
        }
        return result;
    }
private:
    void runWithMasterSuite(StepInfo *stepInfo) {
        using namespace boost::unit_test;
        test_case *tc = BOOST_TEST_CASE(boost::bind(&StepInfo::invokeStep, stepInfo));
        initBoostTest();
        framework::master_test_suite().add(tc);
        framework::run(tc, false);
        framework::master_test_suite().remove(tc->p_id);
    }

    void initBoostTest() {
        using namespace boost::unit_test;
        if (!framework::is_initialized()) {
            int argc = 2;
            char *argv[] = { (char *) "", (char *) "--log_level=nothing" };
            framework::init(&boost_test_init, argc, argv);
            framework::register_observer(*this);
        }
    }

    unsigned int failureCount;
public:
    void test_start(boost::unit_test::counter_t amount) {
        failureCount = 0;
    }
    void assertion_result(bool passed) {
        if (!passed) {
            ++failureCount;
        }
    }
};

typedef BoostCommands CukeCommands;
#define STEP_INHERITANCE(step_name) ::cukebins::internal::BasicStep

}
}

#endif /* CUKEBINS_BOOSTDRIVER_HPP_ */
