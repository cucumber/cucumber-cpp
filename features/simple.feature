Feature: Simple Feature

  Scenario: Simple Scenario
    Given a feature file with:
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
    When I run cucumber-cpp
    Then the output should contain:
      """
      STEP RUN

      """
