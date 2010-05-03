// for now it works with gtest only, but it should switch library depending
// on the testing library included
#include <gtest/gtest.h>
#include <cukebins/wireserver.hpp>

#include <Calculator.h>

CUKE_CONTEXT(CalcCtx) {
    Calculator calc;
    double result;
};

GIVEN(CalcCtx, EnterNumber, "^I have entered (\\d+) into the calculator$") {
    CUKE_PARAM(1, double, n);
    context->calc.push(n);
}

WHEN(CalcCtx, Add, "^I press add") {
    context->result = context->calc.add();
}

WHEN(CalcCtx, Divide, "^I press divide") {
    context->result = context->calc.divide();
}

THEN(CalcCtx, CheckResult, "^the result should be (.*) on the screen$") {
    CUKE_PARAM(1, double, expected);
    EXPECT_EQ(expected, context->result);
}

