#include <gtest/gtest.h>
#include <cukebins/wireserver.hpp>

#include <Calculator.h>

struct CalcCtx {
    Calculator calc;
    double result;
};

GIVEN(CalcCtx, "^I have entered (\\d+) into the calculator$") {
    REGEX_PARAM(double, n);
    context->calc.push(n);
}

WHEN(CalcCtx, "^I press add") {
    context->result = context->calc.add();
}

WHEN(CalcCtx, "^I press divide") {
    context->result = context->calc.divide();
}

THEN(CalcCtx, "^the result should be (.*) on the screen$") {
    REGEX_PARAM(double, expected);
    EXPECT_EQ(expected, context->result);
}
