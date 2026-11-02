# language: en
Feature: Doc string
  In order to explain how to use doc strings
  I have to make this silly example

  Scenario: Doc string
    Given the following quote:
      """
      I'm gonna make him an offer
        he can't refuse.

      "The Godfather", 1972
      """
    Then the quote should keep its line breaks and indentation

  Scenario: Doc string with content type
    Given the following quote:
      """json
      {
        "actor": "Marlon Brando",
        "quote": "I'm gonna make him an offer he can't refuse."
      }
      """
    Then the quote should be the plain JSON text

  Scenario: Doc string after a regex parameter
    Given Morgan Freeman said:
      """
      Get busy living,
      or get busy dying.
      """
    Then Morgan Freeman should be quoted on two lines
