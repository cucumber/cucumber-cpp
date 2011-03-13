#ifndef CUKEBINS_HOOKREGISTRAR_HPP_
#define CUKEBINS_HOOKREGISTRAR_HPP_

#include <list>

namespace cukebins {
namespace internal {

class Hook {
public:
    virtual void invokeHook();
    virtual void body() = 0;
};

class BeforeHook : public Hook {
};

class AfterHook : public Hook {
};

class AfterStepHook : public Hook {
};


class HookRegistrar {
public:
    typedef std::list<Hook *> hook_list_type;

    virtual ~HookRegistrar();

    void addBeforeHook(BeforeHook *afterHook);
    void execBeforeHooks();

    void addAfterStepHook(AfterStepHook *afterStepHook);
    void execAfterStepHooks();

    void addAfterHook(AfterHook *afterHook);
    void execAfterHooks();

private:
    void execHooks(HookRegistrar::hook_list_type &hookList);

    hook_list_type& beforeHooks();
    hook_list_type& afterStepHooks();
    hook_list_type& afterHooks();
};


template<class T>
static int registerBeforeHook() {
   HookRegistrar reg;
   reg.addBeforeHook(new T);
   return 0; // We are not interested in the ID at this time
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
