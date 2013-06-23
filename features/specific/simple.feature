Feature: Simple Feature

  This is just a simple feature meant to test the infrastructure.
  The features in this directory should test code specific to
  Cucumber-Cpp, like how to write hooks, the test frameworks, etc.

  Scenario: Simple Scenario
    Given the following feature:
      """
      Feature: Feature name

        Scenario: Scenario name
          Given a step
      """
    And a step definition file with:
      """
      #include <iostream>

      GIVEN("a step") {
        std::cout << "STEP RUN" << std::endl;
      }
      """
    When Cucumber runs the feature
    Then the step output should contain:
      """
      STEP RUN

      """
