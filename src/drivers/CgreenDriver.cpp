#include <cucumber-cpp/internal/drivers/CgreenDriver.hpp>

#include <boost/bind.hpp>
#include <boost/function.hpp>

using namespace cgreen;

namespace cucumber {
namespace internal {


bool CgreenStep::initialized = false;

boost::function< void() > currentTestBody;

static const char* const blacklist[] = {
        "Running",
        "Completed",
        "Failure",
        "currentTest",
        "\n",
        "/"
};

Ensure(currentTest) {
    if(currentTestBody)
    {
        currentTestBody();
    }
}

class CukeCgreenInterceptor {
public:
    const std::string getCgreenOutput();
    static void clearCgreenOutput();
    static void reset_reporter();
    static int cgreenPrinter(const char* format, ...);
    static TestSuite *cgreenSuite;
    static TestReporter *cgreenReporter;
    ~CukeCgreenInterceptor();

private:
    static std::string cgreenOutput;
    static bool is_blacklisted(const char* input);
};

static CukeCgreenInterceptor cgreenInterceptor;

std::string CukeCgreenInterceptor::cgreenOutput = std::string();

TestSuite * CukeCgreenInterceptor::cgreenSuite = NULL;

TestReporter * CukeCgreenInterceptor::cgreenReporter = NULL;

CukeCgreenInterceptor::~CukeCgreenInterceptor() {
    destroy_test_suite(cgreenSuite);
    destroy_reporter(cgreenReporter);
}

const std::string CukeCgreenInterceptor::getCgreenOutput() {
    return cgreenOutput;
}


void CukeCgreenInterceptor::clearCgreenOutput() {
    cgreenOutput.clear();
}

void CukeCgreenInterceptor::reset_reporter() {
    clearCgreenOutput();

    cgreenReporter->passes = 0;
    cgreenReporter->skips = 0;
    cgreenReporter->failures = 0;
    cgreenReporter->exceptions = 0;
}

int CukeCgreenInterceptor::cgreenPrinter(const char* format, ...) {
    char buffer[10000] = { 0 };

    va_list argPtr;

    va_start(argPtr, format);

    vsprintf(buffer, format, argPtr);

    if(not is_blacklisted(buffer)) {
        cgreenOutput += "\n\n";
        cgreenOutput += buffer;
        cgreenOutput += "\n\n";
    }

    va_end(argPtr);

    std::string blackBoxInfo = "\"initCgreenTest\"";
    std::size_t position = cgreenOutput.find(blackBoxInfo);

    if(position != std::string::npos) {
        cgreenOutput.resize(position);
    }

    return cgreenOutput.length();
}

bool CukeCgreenInterceptor::is_blacklisted(const char* input) {
    const int blacklist_size =
            sizeof(blacklist)
                    / sizeof(blacklist[ 0 ]);

    if(strlen(input) < 1) {
        return true;
    }

    for(int idx = 0; idx < blacklist_size; ++idx) {
        if(strncmp(input, blacklist[idx], strlen(blacklist[idx])) == 0) {
            return true;
        }
    }

    return false;
}

void CgreenStep::setReporterPrinter(
        TestReporter* reporter,
        TextPrinter* newPrinter) {
    TextMemo* memo = (TextMemo*)reporter->memo;
    memo->printer = newPrinter;
}

const InvokeResult CgreenStep::invokeStepBody() {
    int suite_result = EXIT_FAILURE;

    if(not initialized) {
        initCgreenTest();
    } else {
        cgreenInterceptor.reset_reporter();
    }

    currentTestBody = boost::bind(&CgreenStep::body, this);

    suite_result = run_single_test(
            cgreenInterceptor.cgreenSuite,
            "currentTest",
            cgreenInterceptor.cgreenReporter);

    if (suite_result == EXIT_SUCCESS) {
        return InvokeResult::success();
    } else {
        return InvokeResult::failure(cgreenInterceptor.getCgreenOutput());
    }
}

void CgreenStep::initCgreenTest() {
    cgreenInterceptor.cgreenSuite = create_test_suite();

    cgreenInterceptor.cgreenReporter = create_text_reporter();

    cgreenInterceptor.clearCgreenOutput();

    setReporterPrinter(
            cgreenInterceptor.cgreenReporter,
            cgreenInterceptor.cgreenPrinter);

    add_test(cgreenInterceptor.cgreenSuite, currentTest);

    initialized = true;
}

} // namespace internal
} // namespace cucumber
