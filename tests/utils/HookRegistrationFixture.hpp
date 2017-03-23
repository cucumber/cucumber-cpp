#ifndef CUKE_HOOKREGISTRATIONFIXTURE_HPP_
#define CUKE_HOOKREGISTRATIONFIXTURE_HPP_

#include <cucumber-cpp/internal/hook/HookRegistrar.hpp>

#include "CukeCommandsFixture.hpp"

#include <boost/make_shared.hpp>
#include <sstream>

using namespace cucumber::internal;

namespace {

std::stringstream beforeHookCallMarker;
std::stringstream aroundStepHookCallMarker;
std::stringstream beforeAroundStepHookCallMarker;
std::stringstream afterAroundStepHookCallMarker;
std::stringstream afterStepHookCallMarker;
std::stringstream afterHookCallMarker;
std::stringstream globalStepHookCallMarker;
std::stringstream beforeAllHookCallMarker;
std::stringstream afterAllHookCallMarker;
std::string contextContents;

void clearHookCallMarkers() {
    beforeHookCallMarker.str("");
    beforeAroundStepHookCallMarker.str("");
    afterAroundStepHookCallMarker.str("");
    afterStepHookCallMarker.str("");
    afterHookCallMarker.str("");
    globalStepHookCallMarker.str("");
    beforeAllHookCallMarker.str("");
    afterAllHookCallMarker.str("");
    contextContents.clear();
}

std::string getHookCallMarkers() {
   return beforeAllHookCallMarker.str() +
           beforeHookCallMarker.str() +
           beforeAroundStepHookCallMarker.str() +
           afterStepHookCallMarker.str() +
           afterHookCallMarker.str() +
           afterAllHookCallMarker.str();
}

class EmptyCallableStep : public CallableStep {
public:
    void call() {};
};

class HookRegistrarDouble : public HookRegistrar {
public:
    void execAroundStepHooks(Scenario *scenario) {
        EmptyCallableStep emptyStep;
        aroundhook_list_type &ash = aroundStepHooks();
        for (HookRegistrar::aroundhook_list_type::const_iterator h = ash.begin(); h != ash.end(); ++h) {
            (*h)->invokeHook(scenario, &emptyStep);
        }
    }
};

static const InvokeArgs NO_INVOKE_ARGS;

class HookRegistrationTest : public CukeCommandsFixture {
protected:
    HookRegistrarDouble hookRegistrar;
    shared_ptr<Scenario> emptyScenario;

    HookRegistrationTest() {
        emptyScenario = boost::make_shared<Scenario>();
    }

    Scenario *getEmptyScenario() {
        return emptyScenario.get();
    }

    std::string beforeHookOrder() {
        clearHookCallMarkers();
        hookRegistrar.execBeforeHooks(getEmptyScenario());
        return getHookCallMarkers();
    }

    std::string aroundStepHookOrder() {
        clearHookCallMarkers();
        hookRegistrar.execAroundStepHooks(getEmptyScenario());
        return getHookCallMarkers();
    }

    std::string afterStepHookOrder() {
        clearHookCallMarkers();
        hookRegistrar.execAfterStepHooks(getEmptyScenario());
        return getHookCallMarkers();
    }

    std::string afterHookOrder() {
        clearHookCallMarkers();
        hookRegistrar.execAfterHooks(getEmptyScenario());
        return getHookCallMarkers();
    }

    void beginScenario(const TagExpression::tag_list & tags = TagExpression::tag_list()) {
        CukeCommandsFixture::beginScenario(tags);
    }

    void invokeStep() {
        invoke(stepInfoPtr->id, &NO_INVOKE_ARGS);
    }

    void endScenario() {
        CukeCommandsFixture::endScenario();
    }

    std::string sort(std::string str) {
        std::sort(str.begin(), str.end());
        return str;
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

}

#endif /* CUKE_HOOKREGISTRATIONFIXTURE_HPP_ */
