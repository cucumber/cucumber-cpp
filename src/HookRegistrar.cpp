#include <cukebins/internal/hook/HookRegistrar.hpp>

namespace cukebins {
namespace internal {

void Hook::invokeHook() {
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

}
}
