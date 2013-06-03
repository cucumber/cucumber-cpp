
#include <cstdlib>

#include <boost/test/unit_test.hpp>
#include <cucumber-cpp/defs.hpp>

#include <QApplication>
#include <QTest>

#include <CalculatorWidget.h>

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
std::ostream& operator<< (std::ostream& out, const QString& val) { out << val.toAscii().data(); return out; }

GIVEN("^I just turned on the calculator$") {
    cucumber::ScenarioScope<CalculatorWidget> calculator;
    calculator->move(0, 0);
    calculator->show();
    QTest::qWaitForWindowShown(calculator.get());
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
    BOOST_CHECK_EQUAL(calculator->display().size(), 0);
    QTest::qWait(millisecondsToWait());
}

THEN("^the display should show (.*)$") {
    REGEX_PARAM(QString, expected);
    cucumber::ScenarioScope<CalculatorWidget> calculator;
    BOOST_CHECK_EQUAL(expected, calculator->display());
    QTest::qWait(millisecondsToWait());
}
