#ifndef CUKEBINS_HOOKMACROS_HPP_
#define CUKEBINS_HOOKMACROS_HPP_

#include "../RegistrationMacros.hpp"

// ************************************************************************** //
// **************                 BEFORE HOOK                  ************** //
// ************************************************************************** //

#define BEFORE                           \
BEFORE_WITH_NAME_(CUKE_GEN_OBJECT_NAME_) \
/**/

#define BEFORE_WITH_NAME_(step_name)     \
CUKE_OBJECT_(                            \
    step_name,                           \
    BeforeHook,                          \
    BEFORE_HOOK_REGISTRATION_(step_name) \
)                                        \
/**/

#define BEFORE_HOOK_REGISTRATION_(step_name)          \
::cukebins::internal::registerBeforeHook<step_name>() \
/**/

// ************************************************************************** //
// **************               AFTER_STEP HOOK                ************** //
// ************************************************************************** //

#define AFTER_STEP                           \
AFTER_STEP_WITH_NAME_(CUKE_GEN_OBJECT_NAME_) \
/**/

#define AFTER_STEP_WITH_NAME_(step_name)     \
CUKE_OBJECT_(                                \
    step_name,                               \
    AfterStepHook,                           \
    AFTER_STEP_HOOK_REGISTRATION_(step_name) \
)                                            \
/**/

#define AFTER_STEP_HOOK_REGISTRATION_(step_name)         \
::cukebins::internal::registerAfterStepHook<step_name>() \
/**/


// ************************************************************************** //
// **************                  AFTER HOOK                  ************** //
// ************************************************************************** //

#define AFTER                           \
AFTER_WITH_NAME_(CUKE_GEN_OBJECT_NAME_) \
/**/

#define AFTER_WITH_NAME_(step_name)     \
CUKE_OBJECT_(                           \
    step_name,                          \
    AfterHook,                          \
    AFTER_HOOK_REGISTRATION_(step_name) \
)                                       \
/**/

#define AFTER_HOOK_REGISTRATION_(step_name)          \
::cukebins::internal::registerAfterHook<step_name>() \
/**/

#endif /* CUKEBINS_HOOKMACROS_HPP_ */
