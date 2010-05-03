#include <cukebins/cukebins.hpp>

#include <gtest/gtest.h>

#include "../utils/ContextManagerTestDouble.hpp"

using namespace std;
using namespace cukebins::internal;

class ContextHandlingTest : public ::testing::Test {
public:
    ContextManagerTestDouble contextManager;

    ContextHandlingTest() :
        contextManager() {
    }
protected:
private:
    void TearDown() {
        contextManager.purgeContexts();
    }
};
CUKE_CONTEXT(Context1)
{
};
CUKE_CONTEXT(Context2)
{
};

CUKE_FIXTURE(Context1Fixture1, Context1) {
public:
    bool hasValidContext() {
        return !contextReference.expired();
    }
    void freeTheContextAsItWasDestroyed() {
        context.reset();
    }
};
CUKE_FIXTURE(Context1Fixture2, Context1) {
};
CUKE_FIXTURE(Context2Fixture3, Context2) {
};

TEST_F(ContextHandlingTest, fixturesHaveValidContextsTillPurged) {
    ASSERT_EQ(0, contextManager.countContexts());
    Context1Fixture1 test1;
    ASSERT_EQ(1, contextManager.countContexts());
    EXPECT_TRUE(test1.hasValidContext());
    test1.freeTheContextAsItWasDestroyed();
    contextManager.purgeContexts();
    EXPECT_FALSE(test1.hasValidContext());
}

TEST_F(ContextHandlingTest, theSameContextIsNotCreatedTwice) {
    ASSERT_EQ(0, contextManager.countContexts());
    Context1Fixture1 test1_a;
    Context1Fixture1 test1_b;
    ASSERT_EQ(1, contextManager.countContexts());
    Context1Fixture2 test2;
    ASSERT_EQ(1, contextManager.countContexts());
    Context2Fixture3 test3;
    ASSERT_EQ(2, contextManager.countContexts());
}
