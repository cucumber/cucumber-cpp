#include <gtest/gtest.h>

#include <cukebins/internal/hook/HookRegistrar.hpp>
#include <cukebins/internal/hook/HookMacros.hpp>

#include "../utils/CukeCommandsFixture.hpp"

#include <sstream>

using namespace cukebins::internal;

namespace {
    std::stringstream beforeHookCallMarker;
    std::stringstream stepHookCallMarker;
    std::stringstream afterHookCallMarker;

    void clearHookCallMarkers() {
        beforeHookCallMarker.str("");
        stepHookCallMarker.str("");
        afterHookCallMarker.str("");
    }

    std::string getHookCallMarkers() {
       return beforeHookCallMarker.str() + afterHookCallMarker.str() + stepHookCallMarker.str();
    }
}

class HookRegistrationTest : public CukeCommandsFixture {
protected:
    HookRegistrar hookRegistrar;

    std::string beforeHookOrder() {
        clearHookCallMarkers();
        hookRegistrar.execBeforeHooks();
        return getHookCallMarkers();
    }

    std::string afterStepHookOrder() {
        clearHookCallMarkers();
        hookRegistrar.execAfterStepHooks();
        return getHookCallMarkers();
    }

    std::string afterHookOrder() {
        clearHookCallMarkers();
        hookRegistrar.execAfterHooks();
        return getHookCallMarkers();
    }

    void beginScenario() {
        cukeCommands.beginScenario();
    }

    void invokeStep() {
        shared_ptr<command_args_type> no_args;
        cukeCommands.invoke(stepInfoPtr->id, no_args.get());
    }

    void endScenario() {
        cukeCommands.endScenario();
    }

    void SetUp() {
        CukeCommandsFixture::SetUp();
        addStepToManager<EmptyStep>(STATIC_MATCHER);
    }

    void TearDown() {
        clearHookCallMarkers();
        CukeCommandsFixture::TearDown();
    }
};

#define BEFORE_MARKER_1 "b1"
#define BEFORE_MARKER_2 "b2"
#define BEFORE_MARKER_3 "b3"
#define BEFORE_MARKER_ORDER BEFORE_MARKER_1 BEFORE_MARKER_2 BEFORE_MARKER_3
BEFORE { beforeHookCallMarker << BEFORE_MARKER_1; }
BEFORE { beforeHookCallMarker << BEFORE_MARKER_2; }
BEFORE { beforeHookCallMarker << BEFORE_MARKER_3; }

#define AROUND_STEP_MARKER_BEFORE_1 "X"
#define AROUND_STEP_MARKER_BEFORE_2 "Y"
#define AROUND_STEP_MARKER_BEFORE_3 "Z"
#define AROUND_STEP_MARKER_AFTER_3  "z"
#define AROUND_STEP_MARKER_AFTER_2  "y"
#define AROUND_STEP_MARKER_AFTER_1  "x"
#define AROUND_STEP_MARKER_ORDER                                                        \
    AROUND_STEP_MARKER_BEFORE_1 AROUND_STEP_MARKER_BEFORE_2 AROUND_STEP_MARKER_BEFORE_3 \
    AROUND_STEP_MARKER_AFTER_3 AROUND_STEP_MARKER_AFTER_2 AROUND_STEP_MARKER_AFTER_1
AROUND_STEP {
    stepHookCallMarker << AROUND_STEP_MARKER_BEFORE_1;
    step->call();
    stepHookCallMarker << AROUND_STEP_MARKER_AFTER_1;
}
AROUND_STEP {
    stepHookCallMarker << AROUND_STEP_MARKER_BEFORE_2;
    step->call();
    stepHookCallMarker << AROUND_STEP_MARKER_AFTER_2;
}
AROUND_STEP {
    stepHookCallMarker << AROUND_STEP_MARKER_BEFORE_3;
    step->call();
    stepHookCallMarker << AROUND_STEP_MARKER_AFTER_3;
}

#define AFTER_STEP_MARKER_1 "as1"
#define AFTER_STEP_MARKER_2 "as2"
#define AFTER_STEP_MARKER_3 "as3"
#define AFTER_STEP_MARKER_ORDER AFTER_STEP_MARKER_3 AFTER_STEP_MARKER_2 AFTER_STEP_MARKER_1
AFTER_STEP { stepHookCallMarker << AFTER_STEP_MARKER_1; }
AFTER_STEP { stepHookCallMarker << AFTER_STEP_MARKER_2; }
AFTER_STEP { stepHookCallMarker << AFTER_STEP_MARKER_3; }

#define STEP_MARKER_ORDER AROUND_STEP_MARKER_ORDER AFTER_STEP_MARKER_ORDER

#define AFTER_MARKER_1 "a1"
#define AFTER_MARKER_2 "a2"
#define AFTER_MARKER_3 "a3"
#define AFTER_MARKER_ORDER AFTER_MARKER_3 AFTER_MARKER_2 AFTER_MARKER_1
AFTER { afterHookCallMarker << AFTER_MARKER_1; }
AFTER { afterHookCallMarker << AFTER_MARKER_2; }
AFTER { afterHookCallMarker << AFTER_MARKER_3; }


TEST_F(HookRegistrationTest, hooksAreRegisteredInTheDeclaredOrder) {
    EXPECT_EQ(BEFORE_MARKER_ORDER, beforeHookOrder());
    EXPECT_EQ(AFTER_STEP_MARKER_ORDER, afterStepHookOrder());
    EXPECT_EQ(AFTER_MARKER_ORDER, afterHookOrder());
}

TEST_F(HookRegistrationTest, hooksAreInvokedInTheOrderInWhichTheyWereRegistered) {
    EXPECT_EQ("", beforeHookCallMarker.str());
    EXPECT_EQ("", stepHookCallMarker.str());
    EXPECT_EQ("", afterHookCallMarker.str());

    beginScenario();

    EXPECT_EQ(BEFORE_MARKER_ORDER, beforeHookCallMarker.str());
    EXPECT_EQ("", stepHookCallMarker.str());
    EXPECT_EQ("", afterHookCallMarker.str());

    invokeStep();

    EXPECT_EQ(BEFORE_MARKER_ORDER, beforeHookCallMarker.str());
    EXPECT_EQ(STEP_MARKER_ORDER, stepHookCallMarker.str());
    EXPECT_EQ("", afterHookCallMarker.str());

    invokeStep();

    EXPECT_EQ(BEFORE_MARKER_ORDER, beforeHookCallMarker.str());
    EXPECT_EQ(STEP_MARKER_ORDER STEP_MARKER_ORDER, stepHookCallMarker.str());
    EXPECT_EQ("", afterHookCallMarker.str());

    endScenario();

    EXPECT_EQ(BEFORE_MARKER_ORDER, beforeHookCallMarker.str());
    EXPECT_EQ(STEP_MARKER_ORDER STEP_MARKER_ORDER, stepHookCallMarker.str());
    EXPECT_EQ(AFTER_MARKER_ORDER, afterHookCallMarker.str());
}
