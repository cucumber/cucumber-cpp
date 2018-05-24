#include <boost/test/unit_test.hpp>
// Pretend to be GTest
#define EXPECT_EQ BOOST_CHECK_EQUAL
#define ASSERT_TRUE BOOST_REQUIRE
#include "CalculatorQtSteps.cpp"
