#include <gtest/gtest.h>

#include <cucumber-cpp/internal/utils/Regex.hpp>
using namespace cucumber::internal;

#include <memory>

TEST(RegexTest, matchesSimpleRegex) {
    Regex exact("^cde$");

    std::shared_ptr<RegexMatch> match(exact.find("cde"));
    EXPECT_TRUE(match->matches());
    EXPECT_TRUE(match->getSubmatches().empty());

    match = std::shared_ptr<RegexMatch>(exact.find("abcdefg"));
    EXPECT_FALSE(match->matches());
    EXPECT_TRUE(match->getSubmatches().empty());
}

TEST(RegexTest, matchesRegexWithoutSubmatches) {
    Regex variable("x\\d+x");

    std::shared_ptr<RegexMatch> match(variable.find("xxxx123xxx"));
    EXPECT_TRUE(match->matches());

    match = std::shared_ptr<RegexMatch>(variable.find("xxx"));
    EXPECT_FALSE(match->matches());
}

TEST(RegexTest, matchesRegexWithSubmatches) {
    Regex sum("^(\\d+)\\+\\d+=(\\d+)$");

    std::shared_ptr<RegexMatch> match(sum.find("1+2=3 "));
    EXPECT_FALSE(match->matches());
    EXPECT_TRUE(match->getSubmatches().empty());

    match = std::shared_ptr<RegexMatch>(sum.find("42+27=69"));
    EXPECT_TRUE(match->matches());
    ASSERT_EQ(2, match->getSubmatches().size());
    EXPECT_EQ("42", match->getSubmatches()[0].value);
    EXPECT_EQ("69", match->getSubmatches()[1].value);
}

TEST(RegexTest, matchesRegexWithOptionalSubmatches) {
    Regex sum("^(\\d+)\\+(\\d+)(?:\\+(\\d+))?=(\\d+)$");

    std::shared_ptr<RegexMatch> match(sum.find("1+2+3=6"));
    EXPECT_TRUE(match->matches());
    ASSERT_EQ(4, match->getSubmatches().size());

    match = std::shared_ptr<RegexMatch>(sum.find("42+27=69"));
    EXPECT_TRUE(match->matches());
    ASSERT_EQ(4, match->getSubmatches().size());
    EXPECT_EQ("42", match->getSubmatches()[0].value);
    EXPECT_EQ("27", match->getSubmatches()[1].value);
    EXPECT_EQ("", match->getSubmatches()[2].value);
    EXPECT_EQ("69", match->getSubmatches()[3].value);
}

TEST(RegexTest, findAllDoesNotMatchIfNoTokens) {
    Regex sum("([^,]+)(?:,|$)");
    std::shared_ptr<RegexMatch> match(sum.findAll(""));

    EXPECT_FALSE(match->matches());
    EXPECT_EQ(0, match->getSubmatches().size());
}

TEST(RegexTest, findReportsCodepointPositions) {
    Regex twoArgs("Some (.+) regexp (.+)");
    std::shared_ptr<RegexMatch> match(twoArgs.find("Some カラオケ機 regexp ASCII"));

    EXPECT_TRUE(match->matches());
    ASSERT_EQ(2, match->getSubmatches().size());
    EXPECT_EQ(5, match->getSubmatches()[0].position);
    EXPECT_EQ(18, match->getSubmatches()[1].position);
}

TEST(RegexTest, findAllExtractsTheFirstGroupOfEveryToken) {
    Regex sum("([^,]+)(?:,|$)");
    std::shared_ptr<RegexMatch> match(sum.findAll("a,b,cc"));

    EXPECT_TRUE(match->matches());
    EXPECT_EQ(3, match->getSubmatches().size());
    // EXPECT_THAT(match.getSubmatches(), ElementsAre("a", "b", "cc"));
}

/*
TEST(RegexTest, findAllHasToMatchTheEntireInput) {
    Regex sum("([^,]+)(?:,|$)");
    std::shared_ptr<RegexMatch> match(sum.findAll("1 a,b,cc"));

    EXPECT_FALSE(match->matches());
    EXPECT_EQ(0, match->getSubmatches().size());
}
*/
