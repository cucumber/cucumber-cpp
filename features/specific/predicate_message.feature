@boost
Feature: Predicate Message Feature

  This is just a simple feature meant to test boost's 
  validation macros that also output custom message.
  Problem was Cucumber-Cpp didn't capture those messages
  and even didn't recognize failed asserations.
  Expected results when using following Boost macros:
    BOOST_WARN_MESSAGE( predicate, message ) -> should pass
    BOOST_CHECK_MESSAGE( predicate, message ) -> should fail
    BOOST_REQUIRE_MESSAGE( predicate, message ) -> should fail

  Scenario: BOOST_WARN_MESSAGE Scenario
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
        BOOST_WARN_MESSAGE(false, "boost message");
      }
      """
    When Cucumber runs the feature
    Then the scenario passes 

  Scenario: BOOST_CHECK_MESSAGE Scenario
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
        BOOST_CHECK_MESSAGE(false, "boost message");
      }
      """
    When Cucumber runs the feature
    Then the failure message "boost message" is output

  Scenario: BOOST_REQUIRE_MESSAGE Scenario
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
        BOOST_REQUIRE_MESSAGE(false, "boost message");
      }
      """
    When Cucumber runs the feature
    Then the failure message "boost message" is output

