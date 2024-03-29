#include <cucumber-cpp/internal/drivers/BoostDriver.hpp>

#include <sstream>

#include <boost/function.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/unit_test_log_formatter.hpp>
#include <mutex>
#include <boost/version.hpp>

using namespace ::boost::unit_test;
using ::boost::execution_exception;

namespace cucumber {
namespace internal {

namespace {

test_case* testCase = 0;
boost::function<void()> currentTestBody;

void exec_test_body() {
    if (currentTestBody) {
        currentTestBody();
    }
}

bool boost_test_init() {
    testCase = BOOST_TEST_CASE(&exec_test_body);
    framework::master_test_suite().add(testCase);

    return true;
}

// Freed by Boost's unit test framework on exit
static CukeBoostLogInterceptor* logInterceptor = 0;

}

class CukeBoostLogInterceptor : public ::boost::unit_test::unit_test_log_formatter {
public:
    const InvokeResult getResult() const;
    void reset();

    // Formatter
    void log_start(std::ostream&, counter_t /*test_cases_amount*/) override{};
    void log_finish(std::ostream&) override{};
    void log_build_info(std::ostream&, bool /*log_build_info*/) override{};

    void test_unit_start(std::ostream&, test_unit const& /*tu*/) override{};
    void test_unit_finish(std::ostream&, test_unit const& /*tu*/, unsigned long /*elapsed*/)
        override{};
    void test_unit_skipped(std::ostream&, test_unit const& /*tu*/) override{};

    void log_exception_start(std::ostream&, log_checkpoint_data const&, execution_exception const&)
        override{};
    void log_exception_finish(std::ostream&) override{};

    void log_entry_start(std::ostream&, log_entry_data const&, log_entry_types /*let*/) override{};
    void log_entry_value(std::ostream&, const_string value) override;
    void log_entry_value(std::ostream&, lazy_ostream const& value) override;
    void log_entry_finish(std::ostream&) override{};

    void entry_context_start(std::ostream&, log_level /*l*/) override {
    }
    void log_entry_context(std::ostream&, log_level /*l*/, const_string /*value*/) override {
    }
    void entry_context_finish(std::ostream&, log_level /*l*/) override {
    }

private:
    std::stringstream description;
};

void CukeBoostLogInterceptor::reset() {
    description.str("");
}

/*
 * Threshold level set to log_all_errors, so we should be fine logging everything
 */
void CukeBoostLogInterceptor::log_entry_value(std::ostream&, const_string value) {
    description << value;
}

void CukeBoostLogInterceptor::log_entry_value(std::ostream&, lazy_ostream const& value) {
    description << value;
}

const InvokeResult CukeBoostLogInterceptor::getResult() const {
    std::string d = description.str();
    if (d.empty()) {
        return InvokeResult::success();
    } else {
        return InvokeResult::failure(description.str());
    }
}

const InvokeResult BoostStep::invokeStepBody() {
    static std::once_flag initialized;
    std::call_once(initialized, BoostStep::initBoostTest);

    logInterceptor->reset();
    runWithMasterSuite();
    return logInterceptor->getResult();
}

void BoostStep::initBoostTest() {
    int argc = 1;
    char dummyArg[] = "dummy";
    char* argv[] = {dummyArg};
    framework::init(&boost_test_init, argc, argv);
    framework::finalize_setup_phase();

    logInterceptor = new CukeBoostLogInterceptor;
    ::boost::unit_test::unit_test_log.set_formatter(logInterceptor);
    ::boost::unit_test::unit_test_log.set_threshold_level(log_all_errors);
}

void BoostStep::runWithMasterSuite() {
    currentTestBody = std::bind(&BoostStep::body, this);

    ::boost::unit_test::framework::run(testCase, false);

    currentTestBody.clear();
}

}
}
