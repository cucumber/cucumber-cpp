#ifndef CUKEBINS_PP_MACROS_HPP_
#define CUKEBINS_PP_MACROS_HPP_

#include <boost/config/suffix.hpp>

// ************************************************************************** //
// **************                NAMING MACROS                 ************** //
// ************************************************************************** //

#ifdef __COUNTER__
#define CUKE_GEN_STEP_NAME_ BOOST_JOIN(CukeStep, __COUNTER__)
#else
#define CUKE_GEN_STEP_NAME_ BOOST_JOIN(CukeStep, __LINE__)
#endif

#define CUKE_TEST_CLASS_NAME_(step_name) step_name##Test

// ************************************************************************** //
// **************                  CUKE_STEP_                  ************** //
// ************************************************************************** //

#define CUKE_STEP_(context, step_matcher)                        \
CUKE_STEP_WITH_NAME_(context, CUKE_GEN_STEP_NAME_, step_matcher) \
/**/

#define CUKE_STEP_WITH_NAME_(context, step_name, step_matcher)                             \
class BOOST_JOIN(step_name, Fixture) : public ::cukebins::internal::CukeFixture<context> { \
};                                                                                         \
CUKE_DEFINE_TEST_(BOOST_JOIN(step_name, Fixture), step_name, step_matcher)                 \
/**/

#define CUKE_DEFINE_TEST_(feature_class, step_name, step_matcher)                           \
class CUKE_TEST_CLASS_NAME_(step_name) CUKE_STEPCLASS_INHERITANCE_(feature_class) {         \
public:                                                                                     \
    CUKE_TEST_CLASS_NAME_(step_name)() CUKE_INHERITED_CONSTRUCTOR_(feature_class) {}        \
private:                                                                                    \
    virtual void CUKE_STEPCLASS_TESTBODY_NAME_ ();                                          \
    static const int cukeRegId;                                                             \
    CUKE_STEPCLASS_OTHER_DECLARATIONS_(feature_class, step_name)                            \
};                                                                                          \
const int CUKE_TEST_CLASS_NAME_(step_name)::cukeRegId = ::cukebins::internal::registerStep( \
    step_matcher, CUKE_TEST_FULLNAME_(feature_class, step_name)                             \
);                                                                                          \
CUKE_STEPCLASS_OTHER_DEFINITION_(feature_class, step_name)                                  \
void CUKE_TEST_CLASS_NAME_(step_name):: CUKE_STEPCLASS_TESTBODY_NAME_ ()                    \
/**/

// ************************************************************************** //
// **************               GIVEN/WHEN/THEN                ************** //
// ************************************************************************** //

#define GIVEN(context, step_matcher) CUKE_STEP_(context, step_matcher)
#define WHEN(context, step_matcher) CUKE_STEP_(context, step_matcher)
#define THEN(context, step_matcher) CUKE_STEP_(context, step_matcher)

// ************************************************************************** //
// **************                 REGEX_PARAM                  ************** //
// ************************************************************************** //

#define REGEX_PARAM(type, name) type name = ::cukebins::internal::getInvokeArg<type>()


#endif /* CUKEBINS_PP_MACROS_HPP_ */
