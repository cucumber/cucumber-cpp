#include <gtest/gtest.h>

#include <boost/assign/list_of.hpp>
using namespace boost::assign;

#include <cukebins/internal/utils/Regex.hpp>

using namespace cukebins::internal;

TEST(RegexTest, matchesSimpleRegex) {
    Regex exact("^cde$");

    RegexMatch match = exact.find("cde");
    EXPECT_TRUE(match.matches());
    EXPECT_TRUE(match.getSubmatches().empty());

    match = exact.find("abcdefg");
    EXPECT_FALSE(match.matches());
    EXPECT_TRUE(match.getSubmatches().empty());
}

TEST(RegexTest, matchesRegexWithoutSubmatches) {
    Regex variable("x\\d+x");

    RegexMatch match = variable.find("xxxx123xxx");
    EXPECT_TRUE(match.matches());

    match = variable.find("xxx");
    EXPECT_FALSE(match.matches());
}

TEST(RegexTest, matchesRegexWithSubmatches) {
    Regex sum("^(\\d+)\\+\\d+=(\\d+)$");

    RegexMatch match = sum.find("42+27=69");
    ASSERT_TRUE(match.matches());
    ASSERT_EQ(2, match.getSubmatches().size());
    EXPECT_EQ("42", match.getSubmatches()[0].value);
    EXPECT_EQ("69", match.getSubmatches()[1].value);

    match = sum.find("1+2=3 ");
    ASSERT_FALSE(match.matches());
    ASSERT_TRUE(match.getSubmatches().empty());
}
