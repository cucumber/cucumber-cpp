#include <gtest/gtest.h>

#include "utils/ContextManagerTestDouble.hpp"

#include <memory>

using namespace cucumber::internal;

class ContextManagerTest : public ::testing::Test {
public:
    ContextManagerTestDouble contextManager;

    ContextManagerTest() :
        contextManager() {
    }

protected:
private:
    void TearDown() override {
        contextManager.purgeContexts();
    }
};

class Context1 {};
class Context2 {};

TEST_F(ContextManagerTest, createsValidContextPointers) {
    std::weak_ptr<Context1> ctx1 = contextManager.addContext<Context1>();
    ASSERT_EQ(1, contextManager.countContexts());
    ASSERT_FALSE(ctx1.expired());
    std::weak_ptr<Context2> ctx2 = contextManager.addContext<Context2>();
    ASSERT_EQ(2, contextManager.countContexts());
    ASSERT_FALSE(ctx2.expired());
}

TEST_F(ContextManagerTest, allowsCreatingTheSameContextTypeTwice) {
    std::weak_ptr<Context1> ctx1 = contextManager.addContext<Context1>();
    ASSERT_EQ(1, contextManager.countContexts());
    ASSERT_FALSE(ctx1.expired());
    std::weak_ptr<Context1> ctx2 = contextManager.addContext<Context1>();
    ASSERT_EQ(2, contextManager.countContexts());
    ASSERT_FALSE(ctx2.expired());
    ASSERT_NE(ctx1.lock(), ctx2.lock());
}

TEST_F(ContextManagerTest, purgesContexts) {
    std::weak_ptr<Context1> ctx1 = contextManager.addContext<Context1>();
    ASSERT_EQ(1, contextManager.countContexts());
    ASSERT_FALSE(ctx1.expired());
    contextManager.purgeContexts();
    ASSERT_EQ(0, contextManager.countContexts());
    ASSERT_TRUE(ctx1.expired());
}
