#include <QTest>
#include "cucumber-cpp/autodetect.hpp"
#include <cstdlib>
#include <QApplication>
#include "CalculatorWidget.h"

std::istream& operator>> (std::istream& in, QString& val) { std::string s; in >> s; val = QString::fromLocal8Bit(s.c_str()); return in; }
std::ostream& operator<< (std::ostream& out, const QString& val) { out << val.toLocal8Bit().data(); return out; }

static int argc = 0;
static QApplication app(argc, 0);
static int milliseconds = -1;

int millisecondsToWait() {
    if (milliseconds < 0)
    {
        char* envVariable = getenv("CALCQT_STEP_DELAY");
        milliseconds = (envVariable) ? atoi(envVariable) : 0;
    }
    return milliseconds;
}

AFTER_STEP(){
    QTest::qWait(millisecondsToWait());
}

GIVEN("^I just turned on the calculator$") {
    cucumber::ScenarioScope<CalculatorWidget> calculator;
    calculator->move(0, 0);
    calculator->show();
#if QT_VERSION >= 0x050000
    QTest::qWaitForWindowExposed(calculator.get());
#else
    QTest::qWaitForWindowShown(calculator.get());
#endif
}

WHEN("^I press (\\d+)$") {
    REGEX_PARAM(unsigned int, n);
    cucumber::ScenarioScope<CalculatorWidget> calculator;
    QTest::keyClick(calculator.get(), Qt::Key_0 + n, Qt::NoModifier, 0);
}

WHEN("^I press add") {
    cucumber::ScenarioScope<CalculatorWidget> calculator;
    QTest::keyClick(calculator.get(), Qt::Key_Plus, Qt::NoModifier, 0);
}

WHEN("^I press calculate") {
    cucumber::ScenarioScope<CalculatorWidget> calculator;
    QTest::keyClick(calculator.get(), Qt::Key_Return, Qt::NoModifier, 0);
}

WHEN("^I press clear") {
    cucumber::ScenarioScope<CalculatorWidget> calculator;
    QTest::keyClick(calculator.get(), Qt::Key_Escape, Qt::NoModifier, 0);
}

WHEN("^I press subtract") {
    cucumber::ScenarioScope<CalculatorWidget> calculator;
    QTest::keyClick(calculator.get(), Qt::Key_Minus, Qt::NoModifier, 0);
}

THEN("^the display should be empty$") {
    cucumber::ScenarioScope<CalculatorWidget> calculator;
    QCOMPARE(calculator->display().size(), 0);
}

THEN("^the display should show (.*)$") {
    REGEX_PARAM(QString, expected);
    cucumber::ScenarioScope<CalculatorWidget> calculator;
    QCOMPARE(calculator->display(), expected);
}
