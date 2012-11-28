#ifndef CUKE_HOOKMACROS_HPP_
#define CUKE_HOOKMACROS_HPP_

#include "../RegistrationMacros.hpp"

// ************************************************************************** //
// **************                 BEFORE HOOK                  ************** //
// ************************************************************************** //

#define BEFORE(...)                                    \
BEFORE_WITH_NAME_(CUKE_GEN_OBJECT_NAME_, "" #__VA_ARGS__) \
/**/

#define BEFORE_WITH_NAME_(step_name, tag_expression)     \
CUKE_OBJECT_(                                            \
    step_name,                                           \
    ::cucumber::internal::BeforeHook,                    \
    BEFORE_HOOK_REGISTRATION_(step_name, tag_expression) \
)                                                        \
/**/

#define BEFORE_HOOK_REGISTRATION_(step_name, tag_expression)        \
::cucumber::internal::registerBeforeHook<step_name>(tag_expression) \
/**/

// ************************************************************************** //
// **************              AROUND_STEP HOOK                ************** //
// ************************************************************************** //

#define AROUND_STEP(...)                                    \
AROUND_STEP_WITH_NAME_(CUKE_GEN_OBJECT_NAME_, "" #__VA_ARGS__) \
/**/

#define AROUND_STEP_WITH_NAME_(step_name, tag_expression)     \
CUKE_OBJECT_(                                                 \
    step_name,                                                \
    ::cucumber::internal::AroundStepHook,                     \
    AROUND_STEP_HOOK_REGISTRATION_(step_name, tag_expression) \
)                                                             \
/**/

#define AROUND_STEP_HOOK_REGISTRATION_(step_name, tag_expression)       \
::cucumber::internal::registerAroundStepHook<step_name>(tag_expression) \
/**/

// ************************************************************************** //
// **************               AFTER_STEP HOOK                ************** //
// ************************************************************************** //

#define AFTER_STEP(...)                                    \
AFTER_STEP_WITH_NAME_(CUKE_GEN_OBJECT_NAME_, "" #__VA_ARGS__) \
/**/

#define AFTER_STEP_WITH_NAME_(step_name, tag_expression)     \
CUKE_OBJECT_(                                                \
    step_name,                                               \
    ::cucumber::internal::AfterStepHook,                     \
    AFTER_STEP_HOOK_REGISTRATION_(step_name, tag_expression) \
)                                                            \
/**/

#define AFTER_STEP_HOOK_REGISTRATION_(step_name, tag_expression)       \
::cucumber::internal::registerAfterStepHook<step_name>(tag_expression) \
/**/


// ************************************************************************** //
// **************                  AFTER HOOK                  ************** //
// ************************************************************************** //

#define AFTER(...)                                    \
AFTER_WITH_NAME_(CUKE_GEN_OBJECT_NAME_, "" #__VA_ARGS__) \
/**/

#define AFTER_WITH_NAME_(step_name, tag_expression)     \
CUKE_OBJECT_(                                           \
    step_name,                                          \
    ::cucumber::internal::AfterHook,                    \
    AFTER_HOOK_REGISTRATION_(step_name, tag_expression) \
)                                                       \
/**/

#define AFTER_HOOK_REGISTRATION_(step_name, tag_expression)        \
::cucumber::internal::registerAfterHook<step_name>(tag_expression) \
/**/

#endif /* CUKE_HOOKMACROS_HPP_ */
