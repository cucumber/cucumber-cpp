# language: en
# language: en
Feature: Calculate age
  In orther to understand cucumber, I want to create a simple example from scratch

  Scenario Outline: Calculate age
    Given I was born in <year_of_birth>
    When the current year is <current_year>
    Then my age should be <age>

  Examples:
    | year_of_birth | current_year | age |
    |          1991 |         2020 |  29 |
    |          1989 |         2020 |  31 |
    |          1993 |         2020 |  27 |
