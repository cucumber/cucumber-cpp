#include <cukebins/internal/hook/HookRegistrar.hpp>

namespace cukebins {
namespace internal {

void Hook::invokeHook() {
    this->body();
}


void AroundStepHook::invokeHook(CallableStep *step) {
    this->step = step;
    this->body();
}


HookRegistrar::~HookRegistrar() {
}

void HookRegistrar::addBeforeHook(BeforeHook *beforeHook) {
    beforeHooks().push_back(beforeHook);
}

HookRegistrar::hook_list_type& HookRegistrar::beforeHooks() {
    static hook_list_type *beforeHooks = new hook_list_type();
    return *beforeHooks;
}

void HookRegistrar::execBeforeHooks() {
    execHooks(beforeHooks());
}


void HookRegistrar::addAroundStepHook(AroundStepHook *aroundStepHook) {
    aroundStepHooks().push_front(aroundStepHook);
}

HookRegistrar::aroundhook_list_type& HookRegistrar::aroundStepHooks() {
    static aroundhook_list_type *aroundStepHooks = new aroundhook_list_type();
    return *aroundStepHooks;
}

InvokeResult HookRegistrar::execStepChain(StepInfo *stepInfo, command_args_type *args) {
    StepCallChain scc(stepInfo, args, aroundStepHooks());
    return scc.exec();
}


void HookRegistrar::addAfterStepHook(AfterStepHook *afterStepHook) {
    afterStepHooks().push_front(afterStepHook);
}

HookRegistrar::hook_list_type& HookRegistrar::afterStepHooks() {
    static hook_list_type *afterStepHooks = new hook_list_type();
    return *afterStepHooks;
}

void HookRegistrar::execAfterStepHooks() {
    execHooks(afterStepHooks());
}


void HookRegistrar::addAfterHook(AfterHook *afterHook) {
    afterHooks().push_front(afterHook);
}

HookRegistrar::hook_list_type& HookRegistrar::afterHooks() {
    static hook_list_type *afterHooks = new hook_list_type();
    return *afterHooks;
}

void HookRegistrar::execAfterHooks() {
    execHooks(afterHooks());
}


void HookRegistrar::execHooks(HookRegistrar::hook_list_type &hookList) {
    for (HookRegistrar::hook_list_type::iterator hook = hookList.begin(); hook != hookList.end(); ++hook) {
        (*hook)->invokeHook();
    }
}


StepCallChain::StepCallChain(
    StepInfo *stepInfo,
    command_args_type *stepArgs,
    HookRegistrar::aroundhook_list_type &aroundHooks
) :
    stepInfo(stepInfo),
    stepArgs(stepArgs)
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
        (*currentHook)->invokeHook(&callableStepChain);
    }
}

void StepCallChain::execStep() {
    if (stepInfo) {
        result = stepInfo->invokeStep(stepArgs);
    }
}


CallableStepChain::CallableStepChain(StepCallChain *scc) : scc(scc) {};

void CallableStepChain::call() {
    scc->execNext();
}


}
}
