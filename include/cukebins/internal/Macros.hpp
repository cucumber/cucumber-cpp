#ifndef CUKEBINS_MACROS_HPP_
#define CUKEBINS_MACROS_HPP_

// ************************************************************************** //
// **************                NAMING MACROS                 ************** //
// ************************************************************************** //

#ifdef __COUNTER__
#define CUKE_GEN_STEP_NAME_ BOOST_JOIN(CukeStep, __COUNTER__)
#else
// Use a counter to be incremented every time cukebins is included
// in case this does not suffice (possible with multiple files only)
#define CUKE_GEN_STEP_NAME_ BOOST_JOIN(CukeStep, __LINE__)
#endif

// ************************************************************************** //
// **************                  CUKE_STEP_                  ************** //
// ************************************************************************** //

#define CUKE_STEP_(step_matcher)                        \
CUKE_STEP_WITH_NAME_(CUKE_GEN_STEP_NAME_, step_matcher) \
/**/

#define CUKE_STEP_WITH_NAME_(step_name, step_matcher)              \
class step_name : public STEP_INHERITANCE(step_name) {             \
public:                                                            \
    void stepBody();                                               \
private:                                                           \
    static const int cukeRegId;                                    \
};                                                                 \
const int step_name ::cukeRegId =                                  \
    ::cukebins::internal::registerStep< step_name >(step_matcher); \
void step_name ::stepBody()                                        \
/**/

// ************************************************************************** //
// **************               GIVEN/WHEN/THEN                ************** //
// ************************************************************************** //

#define GIVEN CUKE_STEP_
#define WHEN CUKE_STEP_
#define THEN CUKE_STEP_

// ************************************************************************** //
// **************                 REGEX_PARAM                  ************** //
// ************************************************************************** //

#define REGEX_PARAM(type, name) type name(getInvokeArg<type>())

// ************************************************************************** //
// **************                USING_CONTEXT                 ************** //
// ************************************************************************** //

#define USING_CONTEXT(type, name) ::cukebins::internal::SessionContextPtr<type> name

#endif /* CUKEBINS_MACROS_HPP_ */
