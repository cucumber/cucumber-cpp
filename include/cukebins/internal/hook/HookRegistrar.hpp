#ifndef CUKEBINS_HOOKREGISTRAR_HPP_
#define CUKEBINS_HOOKREGISTRAR_HPP_

#include <list>
#include <cukebins/internal/step/StepManager.hpp>

namespace cukebins {
namespace internal {

class CallableStep {
public:
    virtual void call() = 0;
};

class Hook {
public:
    virtual void invokeHook();
    virtual void body() = 0;
};

class BeforeHook : public Hook {
};

class AroundStepHook {
public:
    virtual void invokeHook(CallableStep *step);
    virtual void body() = 0;
protected:
    CallableStep *step;
};

class AfterStepHook : public Hook {
};

class AfterHook : public Hook {
};


class HookRegistrar {
public:
    typedef std::list<Hook *> hook_list_type;
    typedef std::list<AroundStepHook *> aroundhook_list_type;

    virtual ~HookRegistrar();

    void addBeforeHook(BeforeHook *afterHook);
    void execBeforeHooks();

    void addAroundStepHook(AroundStepHook *aroundStepHook);
    InvokeResult execStepChain(StepInfo *stepInfo, command_args_type *args);

    void addAfterStepHook(AfterStepHook *afterStepHook);
    void execAfterStepHooks();

    void addAfterHook(AfterHook *afterHook);
    void execAfterHooks();

private:
    void execHooks(HookRegistrar::hook_list_type &hookList);

    hook_list_type& beforeHooks();
    aroundhook_list_type& aroundStepHooks();
    hook_list_type& afterStepHooks();
    hook_list_type& afterHooks();
};


class StepCallChain {
public:
    StepCallChain(StepInfo *stepInfo, command_args_type *stepArgs, HookRegistrar::aroundhook_list_type &aroundHooks);
    InvokeResult exec();
    void execNext();
private:
    void execStep();

    StepInfo *stepInfo;
    command_args_type *stepArgs;

    HookRegistrar::aroundhook_list_type::iterator nextHook;
    HookRegistrar::aroundhook_list_type::iterator hookEnd;
    InvokeResult result;
};

class CallableStepChain : public CallableStep {
public:
    CallableStepChain(StepCallChain *scc);
    void call();
private:
    StepCallChain *scc;
};


template<class T>
static int registerBeforeHook() {
   HookRegistrar reg;
   reg.addBeforeHook(new T);
   return 0; // We are not interested in the ID at this time
}

template<class T>
static int registerAroundStepHook() {
   HookRegistrar reg;
   reg.addAroundStepHook(new T);
   return 0;
}

template<class T>
static int registerAfterStepHook() {
   HookRegistrar reg;
   reg.addAfterStepHook(new T);
   return 0;
}

template<class T>
static int registerAfterHook() {
   HookRegistrar reg;
   reg.addAfterHook(new T);
   return 0;
}

}
}

#endif /* CUKEBINS_HOOKREGISTRAR_HPP_ */
