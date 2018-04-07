# language: en

Feature: fizz_buzz_reporter
  In order to practice for programming interviews
  As someone who is bad at the FIZZ, BUZZ exercise
  I want to be told whether an unsigned integer corresponds to FIZZ, BUZZ, or FIZZBUZZ

  Scenario Outline: Report the FIZZBUZZ string associated with an integer
    Given I have passed <input> into fizzBuzzReporter
    Then the result should be <output>

  Examples:
  | input | output   |
  | 3     | FIZZ     |
  | 5     | BUZZ     |
  | 15    | FIZZBUZZ |
  | 11    | 11       |
