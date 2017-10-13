#include <cgreen/cgreen.h> // general unit testing
#include <cgreen/mocks.h> // mocking functionality
#include <cucumber-cpp/autodetect.hpp>

#include <Calculator.h>

using cucumber::ScenarioScope;

struct CalcCtx {
    Calculator calc;
    double result;
};

GIVEN("^I have entered (\\d+) into the calculator$") {
    REGEX_PARAM(double, n);
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

THEN("^the result should be (.*) on the screen$") {
    REGEX_PARAM(double, expected);
    ScenarioScope<CalcCtx> context;

    assert_that_double(expected, is_equal_to_double(context->result));
}