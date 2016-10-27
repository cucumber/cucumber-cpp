#include <cucumber-cpp/internal/hook/HookRegistrar.hpp>
#include <cucumber-cpp/internal/CukeCommands.hpp>
#include <boost/make_shared.hpp>

namespace cucumber {
namespace internal {

void Hook::invokeHook(Scenario *scenario, CallableStep *) {
    if (tagsMatch(scenario)) {
        body();
    } else {
        skipHook();
    }
}

void Hook::skipHook() {
}

void Hook::setTags(const std::string &csvTagNotation) {
    tagExpression = boost::make_shared<AndTagExpression>(csvTagNotation);
}

bool Hook::tagsMatch(Scenario *scenario) {
    return !scenario || tagExpression->matches(scenario->getTags());
}

void AroundStepHook::invokeHook(Scenario *scenario, CallableStep *step) {
    this->step = step;
    Hook::invokeHook(scenario, NULL);
}

void AroundStepHook::skipHook() {
    step->call();
}

void UnconditionalHook::invokeHook(Scenario*, CallableStep *) {
    body();
}

HookRegistrar::~HookRegistrar() {
}

void HookRegistrar::addBeforeHook(const boost::shared_ptr<BeforeHook>& beforeHook) {
    beforeHooks().push_back(beforeHook);
}

HookRegistrar::hook_list_type& HookRegistrar::beforeHooks() {
    static hook_list_type beforeHooks;
    return beforeHooks;
}

void HookRegistrar::execBeforeHooks(Scenario *scenario) {
    execHooks(beforeHooks(), scenario);
}


void HookRegistrar::addAroundStepHook(const boost::shared_ptr<AroundStepHook>& aroundStepHook) {
    aroundStepHooks().push_front(aroundStepHook);
}

HookRegistrar::aroundhook_list_type& HookRegistrar::aroundStepHooks() {
    static aroundhook_list_type aroundStepHooks;
    return aroundStepHooks;
}

InvokeResult HookRegistrar::execStepChain(Scenario *scenario, const boost::shared_ptr<const StepInfo>& stepInfo, const InvokeArgs *pArgs) {
    StepCallChain scc(scenario, stepInfo, pArgs, aroundStepHooks());
    return scc.exec();
}

void HookRegistrar::addAfterStepHook(const boost::shared_ptr<AfterStepHook>& afterStepHook) {
    afterStepHooks().push_front(afterStepHook);
}

HookRegistrar::hook_list_type& HookRegistrar::afterStepHooks() {
    static hook_list_type afterStepHooks;
    return afterStepHooks;
}

void HookRegistrar::execAfterStepHooks(Scenario *scenario) {
    execHooks(afterStepHooks(), scenario);
}


void HookRegistrar::addAfterHook(const boost::shared_ptr<AfterHook>& afterHook) {
    afterHooks().push_front(afterHook);
}

HookRegistrar::hook_list_type& HookRegistrar::afterHooks() {
    static hook_list_type afterHooks;
    return afterHooks;
}

void HookRegistrar::execAfterHooks(Scenario *scenario) {
    execHooks(afterHooks(), scenario);
}


void HookRegistrar::execHooks(HookRegistrar::hook_list_type &hookList, Scenario *scenario) {
    for (HookRegistrar::hook_list_type::iterator hook = hookList.begin(); hook != hookList.end(); ++hook) {
        (*hook)->invokeHook(scenario, NULL);
    }
}

HookRegistrar::hook_list_type& HookRegistrar::beforeAllHooks() {
    static hook_list_type beforeAllHooks;
    return beforeAllHooks;
}

void HookRegistrar::addBeforeAllHook(const boost::shared_ptr<BeforeAllHook>& beforeAllHook) {
    beforeAllHooks().push_back(beforeAllHook);
}

void HookRegistrar::execBeforeAllHooks() {
    execHooks(beforeAllHooks(), NULL);
}

HookRegistrar::hook_list_type& HookRegistrar::afterAllHooks() {
    static hook_list_type afterAllHooks;
    return afterAllHooks;
}

void HookRegistrar::addAfterAllHook(const boost::shared_ptr<AfterAllHook>& afterAllHook) {
    afterAllHooks().push_back(afterAllHook);
}

void HookRegistrar::execAfterAllHooks() {
    execHooks(afterAllHooks(), NULL);
}


StepCallChain::StepCallChain(
    Scenario *scenario,
    const boost::shared_ptr<const StepInfo>& stepInfo,
    const InvokeArgs *pStepArgs,
    HookRegistrar::aroundhook_list_type &aroundHooks
) :
    scenario(scenario),
    stepInfo(stepInfo),
    pStepArgs(pStepArgs)
{
    nextHook = aroundHooks.begin();
    hookEnd = aroundHooks.end();
}

InvokeResult StepCallChain::exec() {
    execNext();
    return result;
}

void StepCallChain::execNext() {
    if (nextHook == hookEnd) {
        execStep();
    } else {
        HookRegistrar::aroundhook_list_type::iterator currentHook = nextHook++;
        CallableStepChain callableStepChain(this);
        (*currentHook)->invokeHook(scenario, &callableStepChain);
    }
}

void StepCallChain::execStep() {
    if (stepInfo) {
        result = stepInfo->invokeStep(pStepArgs);
    }
}


CallableStepChain::CallableStepChain(StepCallChain *scc) : scc(scc) {};

void CallableStepChain::call() {
    scc->execNext();
}


}
}
