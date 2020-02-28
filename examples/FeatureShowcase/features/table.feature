# language: en
Feature: Table
  In order to explain how to use tagbles
  I have to make this silly example

  Background:
    Given the following actors are still active:
      | name           | born |
      | Al Pacino      | 1940 |
      | Robert De Niro | 1943 |
      | George Clooney | 1961 |
      | Morgan Freeman | 1937 |

  Scenario: Clooney dies
    Given George Clooney dies
    Then the sexiest actor should be nobody
    And the youngest actor should be Robert De Niro
    And the oldest actor should be Morgan Freeman

  Scenario: Clooney retires
    Given George Clooney retires
    Then the sexiest actor should be nobody
    And the youngest actor should be Robert De Niro
    And the oldest actor should be Morgan Freeman

  Scenario: Morgan retires
    Given Morgan Freeman retires
    Then the oldest actor should be Al Pacino
    And the sexiest actor should be George Clooney
    And the youngest actor should be George Clooney

  Scenario: Morgan dies
    Given Morgan Freeman dies
    Then the oldest actor should be Al Pacino
    And the sexiest actor should be George Clooney
    And the youngest actor should be George Clooney
