Feature: Wire Encoding Feature

  This is just a simple feature meant to test
  transmission of UTF-8 over the WireProtocol.

  Scenario: Multibyte Character Step Matching
    Given the following feature:
      """
      Feature: Match Regex With Multibyte Character String

        Scenario: Match Regex
          Given a step which uses regex to match the following text: 'カラオケ機' and 'ASCII'
      """
    And a step definition file with support code including:
      """
      #include <iostream>
      #include <string>

      GIVEN("a step which uses regex to match the following text: '(.+)' and '(.+)'") {
        REGEX_PARAM(std::string, Match1);
        REGEX_PARAM(std::string, Match2);
        std::cout << Match1 << Match2 << std::endl;
      }
      """
    When Cucumber runs the feature
    Then the step output should contain:
      """
      カラオケ機ASCII

      """
