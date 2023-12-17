#include <gtest/gtest.h>

#include <cucumber-cpp/internal/hook/HookRegistrar.hpp>

using namespace cucumber::internal;

class FakeStepInfo : public StepInfo {
public:
    FakeStepInfo(std::stringstream* markersPtr, const InvokeResult& result) :
        StepInfo("FAKE", ""),
        latestArgsPtr(0),
        markersPtr(markersPtr),
        result(result) {
    }

    InvokeResult invokeStep(const InvokeArgs* pArgs) const override {
        latestArgsPtr = pArgs;
        (*markersPtr) << "S";
        return result;
    }

    const InvokeArgs* getLatestArgsPassed() const {
        return latestArgsPtr;
    }

private:
    mutable const InvokeArgs* latestArgsPtr;
    std::stringstream* markersPtr;
    const InvokeResult result;
};

class MarkingAroundStepHook : public AroundStepHook {
public:
    MarkingAroundStepHook(std::string id, std::stringstream* markersPtr) :
        id(id),
        markersPtr(markersPtr){};

    MarkingAroundStepHook() :
        id(""),
        markersPtr(0){};

    void body() override {
        if (markersPtr) {
            (*markersPtr) << "B" << id;
        }
        doCall();
        if (markersPtr) {
            (*markersPtr) << "A" << id;
        }
    }

protected:
    virtual void doCall() {
        step->call();
    }

private:
    std::string id;
    std::stringstream* markersPtr;
};

class BlockingAroundStepHook : public MarkingAroundStepHook {
public:
    BlockingAroundStepHook(std::string id, std::stringstream* markersPtr) :
        MarkingAroundStepHook(id, markersPtr){};

protected:
    void doCall() override {
    }
};

static const InvokeArgs NO_INVOKE_ARGS;

class StepCallChainTest : public ::testing::Test {
protected:
    HookRegistrar::aroundhook_list_type aroundHooks;
    std::stringstream markers;

    InvokeResult execStep(const InvokeResult& result) {
        const FakeStepInfo step(&markers, result);
        StepCallChain scc(0, &step, &NO_INVOKE_ARGS, aroundHooks);
        return scc.exec();
    }

    void execStepAndCheckSuccess() {
        InvokeResult result;
        result = execStep(InvokeResult::success());
        EXPECT_TRUE(result.isSuccess());
        result = execStep(InvokeResult::failure("Failed"));
        EXPECT_FALSE(result.isSuccess());
    }
};

TEST_F(StepCallChainTest, failsIfNoStep) {
    StepCallChain scc(0, NULL, &NO_INVOKE_ARGS, aroundHooks);
    EXPECT_FALSE(scc.exec().isSuccess());
    EXPECT_EQ("", markers.str());
}

TEST_F(StepCallChainTest, stepExecutionReturnsTheExpectedResult) {
    std::shared_ptr<MarkingAroundStepHook> hook(std::make_shared<MarkingAroundStepHook>());

    execStepAndCheckSuccess();

    aroundHooks.push_back(hook);

    execStepAndCheckSuccess();

    aroundHooks.push_back(hook);
    aroundHooks.push_back(hook);

    execStepAndCheckSuccess();
}

TEST_F(StepCallChainTest, aroundHooksAreInvokedInTheCorrectOrder) {
    std::shared_ptr<MarkingAroundStepHook> hook1(
        std::make_shared<MarkingAroundStepHook>("1", &markers)
    ),
        hook2(std::make_shared<MarkingAroundStepHook>("2", &markers)),
        hook3(std::make_shared<MarkingAroundStepHook>("3", &markers));

    aroundHooks.push_back(hook1);
    aroundHooks.push_back(hook2);
    aroundHooks.push_back(hook3);

    execStep(InvokeResult::success());

    EXPECT_EQ("B1B2B3SA3A2A1", markers.str());
}

TEST_F(StepCallChainTest, argsArePassedToTheStep) {
    const FakeStepInfo step(&markers, InvokeResult::success());
    StepCallChain scc(0, &step, &NO_INVOKE_ARGS, aroundHooks);

    EXPECT_NE(&NO_INVOKE_ARGS, step.getLatestArgsPassed());
    scc.exec();
    EXPECT_EQ(&NO_INVOKE_ARGS, step.getLatestArgsPassed());
}

TEST_F(StepCallChainTest, aroundHooksCanStopTheCallChain) {
    std::shared_ptr<MarkingAroundStepHook> hook1(
        std::make_shared<MarkingAroundStepHook>("1", &markers)
    ),
        hook2(std::make_shared<BlockingAroundStepHook>("2", &markers)),
        hook3(std::make_shared<MarkingAroundStepHook>("3", &markers));

    aroundHooks.push_back(hook1);
    aroundHooks.push_back(hook2);
    aroundHooks.push_back(hook3);

    execStep(InvokeResult::success());

    EXPECT_EQ("B1B2A2A1", markers.str());
}
