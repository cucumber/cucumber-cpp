#include <gtest/gtest.h>

#include <cukebins/internal/hook/HookRegistrar.hpp>
#include <cukebins/internal/hook/HookMacros.hpp>

#include "../utils/CukeCommandsFixture.hpp"

#include <sstream>

using namespace cukebins::internal;

namespace {
    std::stringstream beforeHookCallMarker;
    std::stringstream afterStepHookCallMarker;
    std::stringstream afterHookCallMarker;

    void clearHookCallMarkers() {
        beforeHookCallMarker.str("");
        afterStepHookCallMarker.str("");
        afterHookCallMarker.str("");
    }

    std::string getHookCallMarkers() {
       return beforeHookCallMarker.str() + afterHookCallMarker.str() + afterStepHookCallMarker.str();
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
        cukeCommands.invoke(stepInfoPtr->id, no_args);
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

#define AFTER_STEP_MARKER_1 "as1"
#define AFTER_STEP_MARKER_2 "as2"
#define AFTER_STEP_MARKER_3 "as3"
#define AFTER_STEP_MARKER_ORDER AFTER_STEP_MARKER_3 AFTER_STEP_MARKER_2 AFTER_STEP_MARKER_1
AFTER_STEP { afterStepHookCallMarker << AFTER_STEP_MARKER_1; }
AFTER_STEP { afterStepHookCallMarker << AFTER_STEP_MARKER_2; }
AFTER_STEP { afterStepHookCallMarker << AFTER_STEP_MARKER_3; }

#define AFTER_MARKER_1 "a1"
#define AFTER_MARKER_2 "a2"
#define AFTER_MARKER_3 "a3"
#define AFTER_MARKER_ORDER AFTER_MARKER_3 AFTER_MARKER_2 AFTER_MARKER_1
AFTER { afterHookCallMarker << AFTER_MARKER_1; }
AFTER { afterHookCallMarker << AFTER_MARKER_2; }
AFTER { afterHookCallMarker << AFTER_MARKER_3; }


TEST_F(HookRegistrationTest, hooksAreRegisteredInTheCorrectOrder) {
    EXPECT_EQ(BEFORE_MARKER_ORDER, beforeHookOrder());
    EXPECT_EQ(AFTER_STEP_MARKER_ORDER, afterStepHookOrder());
    EXPECT_EQ(AFTER_MARKER_ORDER, afterHookOrder());
}

TEST_F(HookRegistrationTest, hooksAreInvokedCorrectly) {

    EXPECT_EQ("", beforeHookCallMarker.str());
    EXPECT_EQ("", afterStepHookCallMarker.str());
    EXPECT_EQ("", afterHookCallMarker.str());

    beginScenario();

    EXPECT_EQ(BEFORE_MARKER_ORDER, beforeHookCallMarker.str());
    EXPECT_EQ("", afterStepHookCallMarker.str());
    EXPECT_EQ("", afterHookCallMarker.str());

    invokeStep();

    EXPECT_EQ(BEFORE_MARKER_ORDER, beforeHookCallMarker.str());
    EXPECT_EQ(AFTER_STEP_MARKER_ORDER, afterStepHookCallMarker.str());
    EXPECT_EQ("", afterHookCallMarker.str());

    invokeStep();

    EXPECT_EQ(BEFORE_MARKER_ORDER, beforeHookCallMarker.str());
    EXPECT_EQ(AFTER_STEP_MARKER_ORDER AFTER_STEP_MARKER_ORDER, afterStepHookCallMarker.str());
    EXPECT_EQ("", afterHookCallMarker.str());

    endScenario();

    EXPECT_EQ(BEFORE_MARKER_ORDER, beforeHookCallMarker.str());
    EXPECT_EQ(AFTER_STEP_MARKER_ORDER AFTER_STEP_MARKER_ORDER, afterStepHookCallMarker.str());
    EXPECT_EQ(AFTER_MARKER_ORDER, afterHookCallMarker.str());
}
