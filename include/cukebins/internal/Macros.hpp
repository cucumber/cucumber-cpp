#ifndef CUKEBINS_MACROS_HPP_
#define CUKEBINS_MACROS_HPP_

// ************************************************************************** //
// **************                NAMING MACROS                 ************** //
// ************************************************************************** //

#ifdef __COUNTER__
#define CUKE_GEN_STEP_NAME_ BOOST_JOIN(CukeStep, __COUNTER__)
#else
#define CUKE_GEN_STEP_NAME_ BOOST_JOIN(CukeStep, __LINE__)
#endif

// ************************************************************************** //
// **************                  CUKE_STEP_                  ************** //
// ************************************************************************** //

#define CUKE_STEP_(step_matcher)                        \
CUKE_STEP_WITH_NAME_(CUKE_GEN_STEP_NAME_, step_matcher) \
/**/

#define CUKE_STEP_WITH_NAME_(step_name, step_matcher)              \
class step_name : public ::cukebins::internal::Step {              \
public:                                                            \
    void invoke();                                                 \
private:                                                           \
    static const int cukeRegId;                                    \
};                                                                 \
const int step_name ::cukeRegId =                                  \
    ::cukebins::internal::registerStep< step_name >(step_matcher); \
void step_name ::invoke()                                          \
/**/

// ************************************************************************** //
// **************               GIVEN/WHEN/THEN                ************** //
// ************************************************************************** //

#define GIVEN(step_matcher) CUKE_STEP_(step_matcher)
#define WHEN(step_matcher) CUKE_STEP_(step_matcher)
#define THEN(step_matcher) CUKE_STEP_(step_matcher)

// ************************************************************************** //
// **************                 REGEX_PARAM                  ************** //
// ************************************************************************** //

#define REGEX_PARAM(type, name) type name = ::cukebins::internal::getInvokeArg<type>()

// ************************************************************************** //
// **************                USING_CONTEXT                 ************** //
// ************************************************************************** //

#define USING_CONTEXT(type, name) ::cukebins::internal::SessionContextPtr<type> name

#endif /* CUKEBINS_MACROS_HPP_ */
