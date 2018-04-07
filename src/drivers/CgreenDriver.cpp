#include <cucumber-cpp/internal/drivers/CgreenDriver.hpp>

#include <boost/bind.hpp>
#include <boost/function.hpp>

using namespace cgreen;

namespace cucumber {
namespace internal {


bool CgreenStep::initialized = false;

static const char* const blacklist[] = {
        "Running",
        "Completed",
        "Failure",
        "currentTest",
        "\n",
        "/"
};

class CukeCgreenInterceptor {
private:
    std::string cgreenOutput;
    bool is_blacklisted(const char* input);   
    CukeCgreenInterceptor();
    ~CukeCgreenInterceptor();
public:
    static CukeCgreenInterceptor & Instance();
    const std::string & getCgreenOutput();
    void clearCgreenOutput();
    void reset_reporter();
    static int cgreenPrinter(const char* format, ...);
    TestSuite *cgreenSuite;
    TestReporter *cgreenReporter;
    boost::function< void() > currentTestBody;
};

Ensure(currentTest) {
    if(CukeCgreenInterceptor::Instance().currentTestBody)
    {
        CukeCgreenInterceptor::Instance().currentTestBody();
    }
}

CukeCgreenInterceptor::CukeCgreenInterceptor() {
    cgreenSuite = NULL;
    cgreenReporter = NULL;
}

CukeCgreenInterceptor::~CukeCgreenInterceptor() {
    if(cgreenSuite) { destroy_test_suite(cgreenSuite); }
    if(cgreenReporter) { destroy_reporter(cgreenReporter); }
}

CukeCgreenInterceptor & CukeCgreenInterceptor::Instance() {
    static CukeCgreenInterceptor cgreenInterceptor;
    return cgreenInterceptor;
}

const std::string & CukeCgreenInterceptor::getCgreenOutput() {
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
    char buffer[10000];
    va_list argPtr;
    va_start(argPtr, format);
    if(vsnprintf(buffer, sizeof(buffer), format, argPtr) < 0) {
        return 0;
    }
    va_end(argPtr);

    if(!CukeCgreenInterceptor::Instance().is_blacklisted(buffer)) {
        CukeCgreenInterceptor::Instance().cgreenOutput += "\n\n";
        CukeCgreenInterceptor::Instance().cgreenOutput += buffer;
        CukeCgreenInterceptor::Instance().cgreenOutput += "\n\n";
    }

    static const char blackBoxInfo[] = "\"initCgreenTest\"";
    std::size_t position = CukeCgreenInterceptor::Instance().cgreenOutput.find(blackBoxInfo);
    if(position != std::string::npos) {
        CukeCgreenInterceptor::Instance().cgreenOutput.erase(position);
    }

    return CukeCgreenInterceptor::Instance().cgreenOutput.length();
}

bool CukeCgreenInterceptor::is_blacklisted(const char* input) {
    const int blacklist_size =
            sizeof(blacklist)
                    / sizeof(blacklist[ 0 ]);

    if(input[0] == '\0') {
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
    TextMemo* memo = reinterpret_cast<TextMemo*>(reporter->memo);
    memo->printer = newPrinter;
}

const InvokeResult CgreenStep::invokeStepBody() {
    if(!initialized) {
        initCgreenTest();
    } else {
        CukeCgreenInterceptor::Instance().reset_reporter();
    }

    CukeCgreenInterceptor::Instance().currentTestBody = boost::bind(&CgreenStep::body, this);

    int suite_result = run_single_test(
            CukeCgreenInterceptor::Instance().cgreenSuite,
            "currentTest",
            CukeCgreenInterceptor::Instance().cgreenReporter);

    if (suite_result == EXIT_SUCCESS) {
        return InvokeResult::success();
    } else {
        return InvokeResult::failure(CukeCgreenInterceptor::Instance().getCgreenOutput());
    }
}

void CgreenStep::initCgreenTest() {
    CukeCgreenInterceptor::Instance().cgreenSuite = create_test_suite();
    CukeCgreenInterceptor::Instance().cgreenReporter = create_text_reporter();
    CukeCgreenInterceptor::Instance().clearCgreenOutput();

    setReporterPrinter(
            CukeCgreenInterceptor::Instance().cgreenReporter,
            CukeCgreenInterceptor::Instance().cgreenPrinter);

    add_test(CukeCgreenInterceptor::Instance().cgreenSuite, currentTest);

    initialized = true;
}

} // namespace internal
} // namespace cucumber
