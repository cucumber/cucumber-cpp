# language: en
Feature: Subtraction
  In order to avoid silly mistakes
  As a math idiot 
  I want to be told the difference of two numbers

  Scenario Outline: Subtract two numbers
    Given I just turned on the calculator
    When I press <button1>
    And I press subtract
    And I press <button2>
    And I press calculate
    Then the display should show <result>

  Examples:
    | button1 | button2 | result |
    | 2       | 3       | -1     |
    | 7       | 5       | 2      |
    | 9       | 1       | 8      |
