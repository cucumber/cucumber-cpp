# language: en
Feature: GUI behavior
  tests for buttons

  Scenario Outline: Digit buttons
    Given I just turned on the calculator
    When I press <button>
    Then the display should show <output>

  Examples:
    | button | output |
    | 0      | 0      |
    | 1      | 1      |
    | 2      | 2      |
    | 3      | 3      |
    | 4      | 4      |
    | 5      | 5      |
    | 6      | 6      |
    | 7      | 7      |
    | 8      | 8      |
    | 9      | 9      |

  Scenario: Clear button
    Given I just turned on the calculator
    When I press 2
    And I press clear
    Then the display should be empty

  Scenario: Addition button
    Given I just turned on the calculator
    When I press add
    Then the display should show +

  Scenario: Subtraction button
    Given I just turned on the calculator
    When I press subtract
    Then the display should show -

  Scenario Outline: Digit sequence
    Given I just turned on the calculator
    When I press <button1>
    And I press <button2>
    Then the display should show <output>

  Examples:
    | button1 | button2 | output |
    | 2       | 2       | 22     |
    | 7       | 4       | 74     |
    | 3       | 8       | 38     |

  Scenario Outline: Addition operation
    Given I just turned on the calculator
    When I press <button1>
	 And I press add
    And I press <button2>
    Then the display should show <output>

  Examples:
    | button1 | button2 | output |
    | 2       | 2       | 2+2    |
    | 7       | 4       | 7+4    |
    | 3       | 8       | 3+8    |


  Scenario Outline: Subtraction operation
    Given I just turned on the calculator
    When I press <button1>
	 And I press subtract
    And I press <button2>
    Then the display should show <output>

  Examples:
    | button1 | button2 | output |
    | 2       | 2       | 2-2    |
    | 7       | 4       | 7-4    |
    | 3       | 8       | 3-8    |

