#include <gtest/gtest.h>

#include "utils/StepManagerTestDouble.hpp"

#include <map>

using namespace std;
using namespace cucumber::internal;

class StepManagerTest : public ::testing::Test {
public:
    typedef StepManagerTestDouble StepManager;

protected:
    StepManagerTest() {
    }
    const static char* a_matcher;
    const static char* another_matcher;
    const static char* no_match;
    const static char* a_third_matcher;
    const map<std::ptrdiff_t, string> no_params;

    int getUniqueMatchIdOrZeroFor(const string& stepMatch) {
        MatchResult::match_results_type resultSet = getResultSetFor(stepMatch);
        if (resultSet.size() != 1) {
            return 0;
        } else {
            return resultSet.begin()->stepInfo->id;
        }
    }

    size_t countMatches(const string& stepMatch) {
        return getResultSetFor(stepMatch).size();
    }

    bool matchesOnce(const string& stepMatch) {
        return (countMatches(stepMatch) == 1);
    }

    bool matchesAtLeastOnce(const string& stepMatch) {
        return (countMatches(stepMatch) > 0);
    }

    bool extractedParamsAre(const string stepMatch, map<std::ptrdiff_t, string> params) {
        MatchResult::match_results_type resultSet = getResultSetFor(stepMatch);
        if (resultSet.size() != 1) {
            return false; // more than one match
        }
        SingleStepMatch match = resultSet.front();
        if (params.size() != match.submatches.size()) {
            return false;
        }
        SingleStepMatch::submatches_type::const_iterator rsi;
        for (rsi = match.submatches.begin(); rsi != match.submatches.end(); ++rsi) {
            if (params.find(rsi->position) == params.end())
                return false;
            if (rsi->value != params[rsi->position]) {
                return false;
            }
        }
        return true;
    }

private:
    MatchResult::match_results_type getResultSetFor(const string& stepMatch) {
        return StepManager::stepMatches(stepMatch).getResultSet();
    }
    void TearDown() override {
        StepManager::clearSteps();
    }
};

const char* StepManagerTest::a_matcher = "a matcher";
const char* StepManagerTest::another_matcher = "another matcher";
const char* StepManagerTest::a_third_matcher = "a third matcher";
const char* StepManagerTest::no_match = "no match";

TEST_F(StepManagerTest, holdsNonConflictingSteps) {
    ASSERT_EQ(0, StepManager::count());
    StepManager::addStepDefinition(a_matcher);
    StepManager::addStepDefinition(another_matcher);
    StepManager::addStepDefinition(a_third_matcher);
    ASSERT_EQ(3, StepManager::count());
}

TEST_F(StepManagerTest, holdsConflictingSteps) {
    ASSERT_EQ(0, StepManager::count());
    StepManager::addStepDefinition(a_matcher);
    StepManager::addStepDefinition(a_matcher);
    StepManager::addStepDefinition(a_matcher);
    ASSERT_EQ(3, StepManager::count());
}

TEST_F(StepManagerTest, matchesStepsWithNonRegExMatchers) {
    EXPECT_FALSE(matchesAtLeastOnce(no_match));
    step_id_type aMatcherIndex = StepManager::addStepDefinition(a_matcher);
    EXPECT_EQ(aMatcherIndex, getUniqueMatchIdOrZeroFor(a_matcher));
    step_id_type anotherMatcherIndex = StepManager::addStepDefinition(another_matcher);
    EXPECT_EQ(anotherMatcherIndex, getUniqueMatchIdOrZeroFor(another_matcher));
    EXPECT_EQ(aMatcherIndex, getUniqueMatchIdOrZeroFor(a_matcher));
}

TEST_F(StepManagerTest, matchesStepsWithRegExMatchers) {
    StepManager::addStepDefinition("match the number (\\d+)");
    EXPECT_TRUE(matchesOnce("match the number 42"));
    EXPECT_FALSE(matchesOnce("match the number (\\d+)"));
    EXPECT_FALSE(matchesOnce("match the number one"));
}

TEST_F(StepManagerTest, extractsParamsFromRegExMatchers) {
    StepManager::addStepDefinition("match no params");
    EXPECT_TRUE(extractedParamsAre("match no params", no_params));
    StepManager::addStepDefinition("match the (\\w+) param");
    EXPECT_TRUE(extractedParamsAre("match the first param", {{10, "first"}}));
    StepManager::addStepDefinition("match a (.+)$");
    EXPECT_TRUE(
        extractedParamsAre("match a  string  with  spaces  ", {{8, " string  with  spaces  "}})
    );
    StepManager::addStepDefinition("match params (\\w+), (\\w+) and (\\w+)");
    EXPECT_TRUE(extractedParamsAre("match params A, B and C", {{13, "A"}, {16, "B"}, {22, "C"}}));
}

TEST_F(StepManagerTest, handlesMultipleMatches) {
    StepManager::addStepDefinition(a_matcher);
    StepManager::addStepDefinition(another_matcher);
    StepManager::addStepDefinition(a_matcher);
    EXPECT_EQ(2, countMatches(a_matcher));
}

TEST_F(StepManagerTest, matchesStepsWithNonAsciiMatchers) {
    step_id_type aMatcherIndex = StepManager::addStepDefinition("خيار");
    EXPECT_EQ(aMatcherIndex, getUniqueMatchIdOrZeroFor("خيار"));
    EXPECT_FALSE(matchesAtLeastOnce("cetriolo"));
    EXPECT_FALSE(matchesAtLeastOnce("огурец"));
    EXPECT_FALSE(matchesAtLeastOnce("黄瓜"));
}
