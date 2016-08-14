Feature: Escaping

In order to copy and paste the undefined steps snippets
As a developer
I want the regex string to be correctly escaped

Scenario Outline: <characters> in step definition
  Given a scenario with:
    """
    Given the step contains '<scenario step string>'
    """
  And the steps have no mappings
  When Cucumber executes the scenario
  Then a step definition snippet with "^the step contains '<step definition string>'$" is suggested

    # Remember that in Gherkin's data tables:
    #   \n             -> newline
    #   \|             -> |
    #   \\             -> \
    #   \<other>       -> <other>
    #
    # Unfortunately the behaviour is just odd when chaining backslashes:
    #   \\\\\\.        -> \\. (most common case)
    #   \\\\\\\\+      -> \\+
    #   \\\\\\\\\\\\\\ -> \\\\

  Examples:
    | characters      | scenario step string | step definition string |
    | Double quotes   | "                    | \\"                    |
    | Backslash       | \\                   | \\\\\\\\\\\\\\         |
    | Dot             | .                    | \\\\\\.                |
    | Caret           | ^                    | \\\\\\^                |
    | Dollar          | $                    | \\\\\\$                |
    | Asterisk        | *                    | \\\\\\*                |
    | Plus            | +                    | \\\\\\\\+              |
    | Question mark   | ?                    | \\\\\\?                |
    | Brackets        | ( )                  | \\\\\\( \\\\\\)        |
    | Square brackets | [ ]                  | \\\\\\[ \\\\\\]        |
    | Curly brackets  | { }                  | \\\\\\{ \\\\\\}        |
    | Pipe            | \|                   | \\\\\\\|               |
