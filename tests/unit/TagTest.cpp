#include <gtest/gtest.h>

#include <cucumber-cpp/internal/hook/Tag.hpp>

using namespace cucumber::internal;

TEST(TagTest, emptyOrExpressionMatchesNoTag) {
    OrTagExpression tagExpr("");
    EXPECT_FALSE(tagExpr.matches({"x"}));
    EXPECT_FALSE(tagExpr.matches({"a", "b"}));
}

TEST(TagTest, orExpressionsMatchTheTagSpecified) {
    OrTagExpression tagExpr("@a");
    EXPECT_TRUE(tagExpr.matches({"a"}));
    EXPECT_FALSE(tagExpr.matches({"x"}));
}

TEST(TagTest, orExpressionsMatchAnyTagSpecified) {
    OrTagExpression tagExpr("@a,@b,@c");
    EXPECT_TRUE(tagExpr.matches({"a"}));
    EXPECT_TRUE(tagExpr.matches({"b"}));
    EXPECT_TRUE(tagExpr.matches({"a", "b"}));
    EXPECT_TRUE(tagExpr.matches({"a", "b", "c"}));
    EXPECT_TRUE(tagExpr.matches({"x", "a", "b"}));
    EXPECT_TRUE(tagExpr.matches({"a", "y"}));
    EXPECT_TRUE(tagExpr.matches({"x", "b"}));
    EXPECT_FALSE(tagExpr.matches({"x", "y", "z"}));
}

TEST(TagTest, orExpressionsAllowSpaces) {
    OrTagExpression tagExpr("@a, @b,@c");
    EXPECT_TRUE(tagExpr.matches({"b"}));
    EXPECT_FALSE(tagExpr.matches({"x"}));
}

TEST(TagTest, emptyAndExpressionMatchesAnyTag) {
    AndTagExpression tagExpr("");
    EXPECT_TRUE(tagExpr.matches({"x"}));
    EXPECT_TRUE(tagExpr.matches({"a", "b"}));
}

TEST(TagTest, singleAndExpressionMatchesTheTagSpecified) {
    AndTagExpression tagExpr("\"@a\"");
    EXPECT_TRUE(tagExpr.matches({"a"}));
    EXPECT_FALSE(tagExpr.matches({"x"}));
}

TEST(TagTest, andExpressionsMatchEveryTagSpecified) {
    AndTagExpression tagExpr("\"@a\",\"@b\"");
    EXPECT_TRUE(tagExpr.matches({"a", "b"}));
    EXPECT_TRUE(tagExpr.matches({"x", "a", "b"}));
    EXPECT_FALSE(tagExpr.matches({"a"}));
    EXPECT_FALSE(tagExpr.matches({"b"}));
    EXPECT_FALSE(tagExpr.matches({"a", "y"}));
    EXPECT_FALSE(tagExpr.matches({"x", "b"}));
    EXPECT_FALSE(tagExpr.matches({"x", "y"}));
}

TEST(TagTest, andExpressionsAllowSpaces) {
    AndTagExpression tagExpr(" \"@a\" , \"@b\" ");
    EXPECT_TRUE(tagExpr.matches({"a", "b"}));
    EXPECT_FALSE(tagExpr.matches({"a"}));
}

TEST(TagTest, compositeTagExpressionsAreHandled) {
    AndTagExpression tagExpr("\"@a,@b\", \"@c\", \"@d,@e,@f\"");
    EXPECT_TRUE(tagExpr.matches({"a", "c", "d"}));
    EXPECT_FALSE(tagExpr.matches({"x", "c", "f"}));
}
