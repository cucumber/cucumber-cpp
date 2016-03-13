#include <cucumber-cpp/autodetect.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/math/constants/constants.hpp>
#include <boost/format.hpp>

using std::string;
using cucumber::ScenarioScope;

namespace {

    bool isCloseEnough(double value, double expected) {
        return (std::abs(value - expected) > 0.00001);
    }

    void verifyEqual(double value, double expected) {
        if (isCloseEnough(value, expected)) {
            throw (boost::format("Value %1% not equal to %2%") % value % expected).str();
        }
    }

    void verifyNotEqual(double value, double expected) {
        if (!isCloseEnough(value, expected)) {
            throw (boost::format("Value %1% equal to %2%") % value % expected).str();
        }
    }
}

GIVEN("^a calculator") {
    // nothing to do
}

WHEN("^the calculator computes PI$") {
    ScenarioScope<RpnCalculator> calc;
    calc->pi();
}

WHEN("^the calculator adds up ([\\d\\.]+) and ([\\d\\.]+)$") {
    REGEX_PARAM(string, n1);
    REGEX_PARAM(string, n2);

    ScenarioScope<RpnCalculator> calc;
    calc->push(n1);
    calc->push(n2);
    calc->push("+");
}

WHEN("^the calculator adds up \"([^\"]*)\" and \"([^\"]*)\"$") {
    REGEX_PARAM(string, n1);
    REGEX_PARAM(string, n2);

    ScenarioScope<RpnCalculator> calc;
    calc->push(n1);
    calc->push(n2);
    calc->push("+");
}

WHEN("^the calculator adds up \"([^\"]*)\", \"([^\"]*)\" and \"([^\"]*)\"$") {
    REGEX_PARAM(string, n1);
    REGEX_PARAM(string, n2);
    REGEX_PARAM(string, n3);

    ScenarioScope<RpnCalculator> calc;
    calc->push(n1);
    calc->push(n2);
    calc->push(n3);
    calc->push("+");
    calc->push("+");
}

WHEN("^the calculator adds up the following numbers:$") {
    REGEX_PARAM(string, numberString);

    ScenarioScope<RpnCalculator> calc;
    vector<string> numbers;
    boost::split(numbers, numberString,boost::is_any_of("\n"));

    for (size_t i=0; i < numbers.size(); ++i) {
        calc->push(numbers[i]);
        if (i != 0) {
            calc->push("+");
        }
    }
}

THEN("^the calculator returns PI$") {
    ScenarioScope<RpnCalculator> calc;
    verifyEqual(calc->value(), boost::math::constants::pi<double>());
}

THEN("^the calculator returns \"([^\"]*)\"$") {
    REGEX_PARAM(double, expected);

    ScenarioScope<RpnCalculator> calc;
    verifyEqual(calc->value(), expected);
}

THEN("^the calculator does not return ([\\d\\.]+)$") {
    REGEX_PARAM(double, expected);

    ScenarioScope<RpnCalculator> calc;
    double value = calc->value();
    verifyNotEqual(calc->value(), expected);
}

