#include <gtest/gtest.h>

#include "../utils/StepManagerTestDouble.hpp"

#include <boost/assign/list_of.hpp>
#include <map>

using namespace std;
using namespace boost::assign;
using namespace cukebins::internal;

class StepManagerTest : public ::testing::Test {
public:
    StepManagerTestDouble stepManager;

    StepManagerTest() :
        stepManager() {
    }
protected:
    const static char *a_matcher;
    const static char *another_matcher;
    const static char *no_match;
    const static char *a_third_matcher;
    const static char *a_step_name;
    const static char *another_step_name;
    const static char *no_step_name;
    const map<int, string> no_params;

    int getUniqueMatchIdOrZeroFor(const string &stepMatch) {
        MatchResult::results_type resultSet = getResultSetFor(stepMatch);
        if (resultSet.size() != 1) {
            return 0;
        } else {
            return resultSet.begin()->id;
        }
    }

    const string getStepNameFromId(StepInfo::id_type id) {
        return stepManager.getStep(id).testName;
    }

    int countMatches(const string &stepMatch) {
        return getResultSetFor(stepMatch).size();
    }

    bool matchesOnce(const string &stepMatch) {
        return (countMatches(stepMatch) == 1);
    }

    bool matchesAtLeastOnce(const string &stepMatch) {
        return (countMatches(stepMatch) > 0);
    }

    bool extractedParamsAre(const string stepMatch, map<int, string> params) {
        MatchResult::results_type resultSet = getResultSetFor(stepMatch);
        if (resultSet.size() != 1) {
            return false; // more than one match
        }
        Match match = resultSet.front();
        if (params.size() != match.subExpressions.size()) {
            return false;
        }
        std::vector<SubExpression>::const_iterator rsi;
        for (rsi = match.subExpressions.begin(); rsi != match.subExpressions.end(); ++rsi) {
            if (params.find(rsi->position) == params.end())
                return false;
            if (rsi->value != params[rsi->position]) {
                return false;
            }
        }
        return true;
    }
private:
    MatchResult::results_type getResultSetFor(const string &stepMatch) {
        return stepManager.stepMatches(stepMatch).getResultSet();
    }
    void TearDown() {
        stepManager.clearSteps();
    }
};

const char *StepManagerTest::a_matcher = "a matcher";
const char *StepManagerTest::another_matcher = "another matcher";
const char *StepManagerTest::a_third_matcher = "a third matcher";
const char *StepManagerTest::no_match = "no match";
const char *StepManagerTest::a_step_name = "a step name";
const char *StepManagerTest::another_step_name = "another step name";
const char *StepManagerTest::no_step_name = "";

TEST_F(StepManagerTest, holdsNonConflictingSteps) {
    ASSERT_EQ(0, stepManager.count());
    stepManager.addStepDefinition(a_matcher, no_step_name);
    stepManager.addStepDefinition(another_matcher, no_step_name);
    stepManager.addStepDefinition(a_third_matcher, no_step_name);
    ASSERT_EQ(3, stepManager.count());
}

TEST_F(StepManagerTest, holdsConflictingSteps) {
    ASSERT_EQ(0, stepManager.count());
    stepManager.addStepDefinition(a_matcher, no_step_name);
    stepManager.addStepDefinition(a_matcher, no_step_name);
    stepManager.addStepDefinition(a_matcher, no_step_name);
    ASSERT_EQ(3, stepManager.count());
}

TEST_F(StepManagerTest, matchesStepsWithNonRegExMatchers) {
    EXPECT_FALSE(matchesAtLeastOnce(no_match));
    StepInfo::id_type aMatcherIndex = stepManager.addStepDefinition(a_matcher, a_step_name);
    EXPECT_EQ(aMatcherIndex, getUniqueMatchIdOrZeroFor(a_matcher));
    EXPECT_STREQ(a_step_name, getStepNameFromId(aMatcherIndex).c_str());
    StepInfo::id_type anotherMatcherIndex = stepManager.addStepDefinition(another_matcher, another_step_name);
    EXPECT_EQ(anotherMatcherIndex, getUniqueMatchIdOrZeroFor(another_matcher));
    EXPECT_STREQ(another_step_name, getStepNameFromId(anotherMatcherIndex).c_str());
    EXPECT_EQ(aMatcherIndex, getUniqueMatchIdOrZeroFor(a_matcher));
}

TEST_F(StepManagerTest, matchesStepsWithRegExMatchers) {
    stepManager.addStepDefinition("match the number (\\d+)", no_step_name);
    EXPECT_TRUE(matchesOnce("match the number 42"));
    EXPECT_FALSE(matchesOnce("match the number (\\d+)"));
    EXPECT_FALSE(matchesOnce("match the number one"));
}

TEST_F(StepManagerTest, extractsParamsFromRegExMatchers) {
    stepManager.addStepDefinition("match no params", no_step_name);
    EXPECT_TRUE(extractedParamsAre("match no params", no_params));
    stepManager.addStepDefinition("match the (\\w+) param", no_step_name);
    EXPECT_TRUE(extractedParamsAre("match the first param", map_list_of(10, "first")));
    stepManager.addStepDefinition("match params (\\w+), (\\w+) and (\\w+)", no_step_name);
    EXPECT_TRUE(extractedParamsAre("match params A, B and C", map_list_of(13, "A")(16, "B")(22, "C")));
}

TEST_F(StepManagerTest, handlesMultipleMatches) {
    stepManager.addStepDefinition(a_matcher, no_step_name);
    stepManager.addStepDefinition(another_matcher, no_step_name);
    stepManager.addStepDefinition(a_matcher, no_step_name);
    EXPECT_EQ(2, countMatches(a_matcher));
}

TEST_F(StepManagerTest, matchesStepsWithNonAsciiMatchers) {
    StepInfo::id_type aMatcherIndex = stepManager.addStepDefinition("خيار", no_step_name);
    EXPECT_EQ(aMatcherIndex, getUniqueMatchIdOrZeroFor("خيار"));
    EXPECT_FALSE(matchesAtLeastOnce("cetriolo"));
    EXPECT_FALSE(matchesAtLeastOnce("огурец"));
    EXPECT_FALSE(matchesAtLeastOnce("黄瓜"));
}

