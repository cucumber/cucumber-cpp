#ifndef CUKE_STEPMACROS_HPP_
#define CUKE_STEPMACROS_HPP_

#include "../RegistrationMacros.hpp"
#include "BreakpointSupport.hpp"

// ************************************************************************** //
// **************                     STEP                     ************** //
// ************************************************************************** //

#define CUKE_STEP_(step_matcher)                        \
CUKE_STEP_WITH_NAME_(CUKE_GEN_OBJECT_NAME_, step_matcher) \
/**/

#define CUKE_STEP_WITH_NAME_(step_name, step_matcher) \
CUKE_OBJECT_(                                         \
    step_name,                                        \
    STEP_INHERITANCE(step_name),                      \
    CUKE_STEP_REGISTRATION_(step_name, step_matcher)  \
)                                                     \
/**/

#define CUKE_STEP_REGISTRATION_(step_name, step_matcher) \
::cucumber::internal::registerStep< step_name >(         \
    step_matcher, __FILE__, __LINE__                     \
)                                                        \
/**/

#if defined(CUKE_ENABLE_BREAKPOINT_SUPPORT)
#define BODY_WITH_BREAKPOINT_IMPLEMENTATION_(step_class_name)                 \
  {                                                                           \
    if (BreakpointSupport::checkAndClearShouldBreakIntoDebugger()) {          \
      CUKE_BREAK_INTO_DEBUGGER;                                               \
    }                                                                         \
    bodyIfCompiledWithBreakpointSupport();                                    \
  }                                                                           \
  void step_class_name ::bodyIfCompiledWithBreakpointSupport()
/**/

#define CUKE_STEP_WITH_BREAKPOINT_WITH_NAME_(step_name, step_matcher)         \
  CUKE_STEP_WITH_NAME_(step_name, step_matcher)                               \
  BODY_WITH_BREAKPOINT_IMPLEMENTATION_(step_name)
/**/

#define CUKE_STEP_WITH_BREAKPOINT_(step_matcher)                              \
  CUKE_STEP_WITH_BREAKPOINT_WITH_NAME_(CUKE_GEN_OBJECT_NAME_, step_matcher)
/**/

#endif

// ************************************************************************** //
// **************               GIVEN/WHEN/THEN                ************** //
// ************************************************************************** //

#if defined(CUKE_ENABLE_BREAKPOINT_SUPPORT)
#define GIVEN CUKE_STEP_WITH_BREAKPOINT_
#define WHEN CUKE_STEP_WITH_BREAKPOINT_
#define THEN CUKE_STEP_WITH_BREAKPOINT_

WHEN("^BREAK!$") {
  BreakpointSupport::shouldBreakIntoDebuggerInNextStep();
}
#else
#define GIVEN CUKE_STEP_
#define WHEN CUKE_STEP_
#define THEN CUKE_STEP_
#endif

// ************************************************************************** //
// **************                 REGEX_PARAM                  ************** //
// ************************************************************************** //

#define REGEX_PARAM(type, name) const type name(getInvokeArg<type>())
#define TABLE_PARAM(name) const ::cucumber::internal::Table & name = \
    getArgs()->getTableArg()

#endif /* CUKE_STEPMACROS_HPP_ */
