# language: en
Feature: GUI initalization
  test the initial status of the calculator

  Scenario: Initialization
    Given I just turned on the calculator
    Then the display should be empty

