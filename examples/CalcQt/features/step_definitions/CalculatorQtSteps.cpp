#include <cstdlib>
#include <cucumber-cpp/autodetect.hpp>
#include <QApplication>
#include <QTest>

#include "CalculatorWidget.h"

/// Helper class to ensure that QApplication gets destroyed before main() returns
/// and that CalculatorWidget gets destroyed before QApplication gets destroyed.
struct CalculatorCtx {
    CalculatorCtx()
      : argc(0)
      , app(argc, NULL)
    {}

    int argc;
    QApplication app;
    CalculatorWidget calculator;
};

static int milliseconds = -1;

int millisecondsToWait() {
    if (milliseconds < 0)
    {
        char* envVariable = getenv("CALCQT_STEP_DELAY");
        milliseconds = (0 != envVariable) ? atoi(envVariable) : 0;
    }
    return milliseconds;
}

std::istream& operator>> (std::istream& in, QString& val) { std::string s; in >> s; val = s.c_str(); return in; }
std::ostream& operator<< (std::ostream& out, const QString& val) { out << val.toLatin1().data(); return out; }

GIVEN("^I just turned on the calculator$") {
    cucumber::ScenarioScope<CalculatorCtx> ctx;
    ctx->calculator.move(0, 0);
    ctx->calculator.show();
#if QT_VERSION >= 0x050000
    const bool window_shown = QTest::qWaitForWindowExposed(&ctx->calculator);
#else
    const bool window_shown = QTest::qWaitForWindowShown(&ctx->calculator);
#endif
    ASSERT_TRUE(window_shown);
    QTest::qWait(millisecondsToWait());
}

WHEN("^I press (\\d+)$") {
    REGEX_PARAM(unsigned int, n);
    cucumber::ScenarioScope<CalculatorCtx> ctx;
    QTest::keyClick(&ctx->calculator, Qt::Key_0 + n, Qt::NoModifier, millisecondsToWait());
}

WHEN("^I press add") {
    cucumber::ScenarioScope<CalculatorCtx> ctx;
    QTest::keyClick(&ctx->calculator, Qt::Key_Plus, Qt::NoModifier, millisecondsToWait());
}

WHEN("^I press calculate") {
    cucumber::ScenarioScope<CalculatorCtx> ctx;
    QTest::keyClick(&ctx->calculator, Qt::Key_Return, Qt::NoModifier, millisecondsToWait());
}

WHEN("^I press clear") {
    cucumber::ScenarioScope<CalculatorCtx> ctx;
    QTest::keyClick(&ctx->calculator, Qt::Key_Escape, Qt::NoModifier, millisecondsToWait());
}

WHEN("^I press subtract") {
    cucumber::ScenarioScope<CalculatorCtx> ctx;
    QTest::keyClick(&ctx->calculator, Qt::Key_Minus, Qt::NoModifier, millisecondsToWait());
}

THEN("^the display should be empty$") {
    cucumber::ScenarioScope<CalculatorCtx> ctx;
    EXPECT_EQ(ctx->calculator.display().size(), 0);
    QTest::qWait(millisecondsToWait());
}

THEN("^the display should show (.*)$") {
    REGEX_PARAM(QString, expected);
    cucumber::ScenarioScope<CalculatorCtx> ctx;
    EXPECT_EQ(expected, ctx->calculator.display());
    QTest::qWait(millisecondsToWait());
}
