#include <cstdlib>
#include <gtest/gtest.h>
#include <cucumber-cpp/autodetect.hpp>
#include <QApplication>
#include <QTest>

#include "CalculatorWidget.h"

static int argc = 0;
static QApplication app(argc, 0);
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
    cucumber::ScenarioScope<CalculatorWidget> calculator;
    calculator->move(0, 0);
    calculator->show();
#if QT_VERSION >= 0x050000
    QTest::qWaitForWindowExposed(calculator.get());
#else
    QTest::qWaitForWindowShown(calculator.get());
#endif
    QTest::qWait(millisecondsToWait());
}

WHEN("^I press (\\d+)$") {
    REGEX_PARAM(unsigned int, n);
    cucumber::ScenarioScope<CalculatorWidget> calculator;
    QTest::keyClick(calculator.get(), Qt::Key_0 + n, Qt::NoModifier, millisecondsToWait());
}

WHEN("^I press add") {
    cucumber::ScenarioScope<CalculatorWidget> calculator;
    QTest::keyClick(calculator.get(), Qt::Key_Plus, Qt::NoModifier, millisecondsToWait());
}

WHEN("^I press calculate") {
    cucumber::ScenarioScope<CalculatorWidget> calculator;
    QTest::keyClick(calculator.get(), Qt::Key_Return, Qt::NoModifier, millisecondsToWait());
}

WHEN("^I press clear") {
    cucumber::ScenarioScope<CalculatorWidget> calculator;
    QTest::keyClick(calculator.get(), Qt::Key_Escape, Qt::NoModifier, millisecondsToWait());
}

WHEN("^I press subtract") {
    cucumber::ScenarioScope<CalculatorWidget> calculator;
    QTest::keyClick(calculator.get(), Qt::Key_Minus, Qt::NoModifier, millisecondsToWait());
}

THEN("^the display should be empty$") {
    cucumber::ScenarioScope<CalculatorWidget> calculator;
    EXPECT_EQ(calculator->display().size(), 0);
    QTest::qWait(millisecondsToWait());
}

THEN("^the display should show (.*)$") {
    REGEX_PARAM(QString, expected);
    cucumber::ScenarioScope<CalculatorWidget> calculator;
    EXPECT_EQ(expected, calculator->display());
    QTest::qWait(millisecondsToWait());
}
