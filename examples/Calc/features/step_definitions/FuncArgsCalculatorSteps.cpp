#include <gtest/gtest.h>
#include <cucumber-cpp/autodetect.hpp>

#include <Calculator.hpp>

using cucumber::ScenarioScope;

struct CalcCtx {
    Calculator calc;
    double result;
};

GIVEN("^I have entered (\\d+) into the calculator$", (const double n)) {
    ScenarioScope<CalcCtx> context;

    context->calc.push(n);
}

WHEN("^I press add") {
    ScenarioScope<CalcCtx> context;

    context->result = context->calc.add();
}

WHEN("^I press divide") {
    ScenarioScope<CalcCtx> context;

    context->result = context->calc.divide();
}

THEN("^the result should be (.*) on the screen$", (const double expected)) {
    ScenarioScope<CalcCtx> context;

    EXPECT_EQ(expected, context->result);
}
