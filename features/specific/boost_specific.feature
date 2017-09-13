@boost
Feature: Check macros used with boost test driver

  This is just a simple feature meant to test boost's 
  specyfic validation macros interop with cucumber.

  Scenario Outline: <check> macro
    Given the following feature:
      """
      Feature: Feature name
        Scenario: Scenario name
          Given a step
      """
    And a step definition file with:
      """
      #include <boost/test/unit_test.hpp>
      #include <cucumber-cpp/autodetect.hpp>
      GIVEN("a step") {
        <check>;
      }
      """
    When Cucumber runs the feature
    Then the scenario <passes or fails?>

    Examples:
      | check                                         | passes or fails?                    |
      | BOOST_CHECK(false)                            | fails                               |
      | BOOST_CHECK_MESSAGE(false, "boost message")   | fails with message "boost message"  |
      | BOOST_ERROR("boost message")                  | fails with message "boost message"  |
      | BOOST_FAIL("boost message")                   | fails with message "boost message"  |
      | BOOST_REQUIRE(false)                          | fails                               |
      | BOOST_REQUIRE_MESSAGE(false, "boost message") | fails with message "boost message"  |
      | BOOST_WARN(false)                             | passes                              |
      | BOOST_WARN_MESSAGE(false, "boost message")    | passes                              |
