#include <cucumber-cpp/autodetect.hpp>

#include "Calculator.hpp"

class CalculatorCtx {
public:
    CalculatorCtx() {
        QObject::connect(&calculator, &Calculator::updateDisplay, [this](QString value) {
            display = value;
        });
    }

    Calculator calculator{};
    QString display{};
};

std::istream& operator>>(std::istream& in, QString& val) {
    std::string s;
    in >> s;
    val = s.c_str();
    return in;
}
std::ostream& operator<<(std::ostream& out, const QString& val) {
    out << val.toLatin1().data();
    return out;
}

GIVEN("^I just turned on the calculator$") {
    cucumber::ScenarioScope<CalculatorCtx> ctx;
}

WHEN("^I press (\\d+)$") {
    REGEX_PARAM(unsigned int, n);
    cucumber::ScenarioScope<CalculatorCtx> ctx;
    ctx->calculator.number(n);
}

WHEN("^I press add") {
    cucumber::ScenarioScope<CalculatorCtx> ctx;
    ctx->calculator.add();
}

WHEN("^I press calculate") {
    cucumber::ScenarioScope<CalculatorCtx> ctx;
    ctx->calculator.calculate();
}

WHEN("^I press clear") {
    cucumber::ScenarioScope<CalculatorCtx> ctx;
    ctx->calculator.clear();
}

WHEN("^I press subtract") {
    cucumber::ScenarioScope<CalculatorCtx> ctx;
    ctx->calculator.subtract();
}

THEN("^the display should be empty$") {
    cucumber::ScenarioScope<CalculatorCtx> ctx;
    EXPECT_EQ(QString{}, ctx->display);
}

THEN("^the display should show (.*)$") {
    REGEX_PARAM(QString, expected);
    cucumber::ScenarioScope<CalculatorCtx> ctx;
    EXPECT_EQ(expected, ctx->display);
}
