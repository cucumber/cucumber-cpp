#include <gtest/gtest.h>
#include <cucumber-cpp/autodetect.hpp>

#include <string>

using cucumber::ScenarioScope;

struct Quote {
    std::string author;
    std::string text;
};

/*
 * The doc string is passed as the last argument, after the regex parameters.
 */

GIVEN("^the following quote:$") {
    REGEX_PARAM(std::string, text);
    ScenarioScope<Quote> context;

    context->text = text;
}

GIVEN("^(.+) said:$") {
    REGEX_PARAM(std::string, author);
    REGEX_PARAM(std::string, text);
    ScenarioScope<Quote> context;

    context->author = author;
    context->text = text;
}

THEN("^the quote should keep its line breaks and indentation$") {
    ScenarioScope<Quote> context;

    ASSERT_EQ(
        "I'm gonna make him an offer\n"
        "  he can't refuse.\n"
        "\n"
        "\"The Godfather\", 1972",
        context->text
    );
}

/*
 * The content type of a doc string is not sent over the wire, only its content.
 */

THEN("^the quote should be the plain JSON text$") {
    ScenarioScope<Quote> context;

    ASSERT_EQ(
        "{\n"
        "  \"actor\": \"Marlon Brando\",\n"
        "  \"quote\": \"I'm gonna make him an offer he can't refuse.\"\n"
        "}",
        context->text
    );
}

THEN("^(.+) should be quoted on two lines$") {
    REGEX_PARAM(std::string, author);
    ScenarioScope<Quote> context;

    ASSERT_EQ(author, context->author);
    ASSERT_EQ("Get busy living,\nor get busy dying.", context->text);
}
