# language: en
Feature: Addition
  In order to avoid silly mistakes
  As a math idiot 
  I want to be told the sum of two numbers

  Scenario Outline: Add two numbers
    Given I just turned on the calculator
    When I press <button1>
    And I press add
    And I press <button2>
    And I press calculate
    Then the display should show <result>

  Examples:
    | button1 | button2 | result |
    | 2       | 3       | 5      |
    | 7       | 5       | 12     |
    | 9       | 1       | 10     |
