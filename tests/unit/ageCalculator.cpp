#include <gtest/gtest.h>
#include <cucumber-cpp/autodetect.hpp>

using cucumber::ScenarioScope;

struct AgeCalcCtx {
  double year_of_birth;
  double current_year;
};

GIVEN("^I was born in (\\d+)$") {
  REGEX_PARAM(double, year_of_birth);
  ScenarioScope<AgeCalcCtx> context;

  context->year_of_birth = year_of_birth;
  EXPECT_EQ(true, false);
}

WHEN("^the current year is (\\d+)$") {
  REGEX_PARAM(double, current_year);
  ScenarioScope<AgeCalcCtx> context;

  context->current_year = current_year;
  EXPECT_EQ(true, false);
}

THEN("^my age should be (\\d+)$") {
  REGEX_PARAM(double, expected_age);
  ScenarioScope<AgeCalcCtx> context;
  double calculated_age = context->current_year - context->year_of_birth;
  EXPECT_EQ(expected_age, calculated_age);
  EXPECT_EQ(true, false);
}
