#ifndef CUKEBINS_HPP_
#define CUKEBINS_HPP_

#include "internal/StepManager.hpp"
#include "internal/ContextManager.hpp"
#include "internal/CukeCommands.hpp"

#define CUKE_STEP_CLASS_NAME_(feature_class, step_name) feature_class##_##step_name##_Step

#ifdef GTEST_INCLUDE_GTEST_GTEST_H_
#include "internal/drivers/GTestDriver.hpp"
#else // No test framework
#include "internal/drivers/FakeDriver.hpp"
#endif

#define CUKE_TEST_REAL_NAME_(step_name) CUKE_TEST_NAME_PREFIX_ #step_name

// TODO: parent_class and parent_id should not be needed
#define CUKE_STEP_(feature_class, step_name, step_matcher, parent_class, parent_id) \
    class CUKE_STEP_CLASS_NAME_(feature_class, step_name) CUKE_STEPCLASS_INHERITANCE_(parent_class) { \
    public: \
        CUKE_STEP_CLASS_NAME_(feature_class, step_name)() CUKE_INHERITED_CONSTRUCTOR_(parent_class) {} \
    private: \
        virtual void CUKE_STEPCLASS_TESTBODY_NAME_ (); \
        static const int cukeRegId; \
        CUKE_STEPCLASS_OTHER_DECLARATIONS_(feature_class, step_name) \
    }; \
    const int CUKE_STEP_CLASS_NAME_(feature_class, step_name)::cukeRegId = \
        ::cukebins::internal::registerStep(step_matcher, CUKE_TEST_FULLNAME_(feature_class, step_name)); \
    CUKE_STEPCLASS_OTHER_DEFINITION_(feature_class, step_name, parent_class, parent_id) \
    void CUKE_STEP_CLASS_NAME_(feature_class, step_name):: CUKE_STEPCLASS_TESTBODY_NAME_ ()

#define CUKE_STEP_F_(feature_class, step_name, step_matcher) \
    CUKE_STEP_(feature_class, step_name, step_matcher, \
        feature_class, ::testing::internal::GetTypeId<feature_class>())
#define CUKE_STEP_C_(context, step_name, step_matcher) CUKE_STEP_F_(context##_DefaultFixture, step_name, step_matcher)

#define GIVEN_F(fixture, step_name, step_matcher) CUKE_STEP_F_(fixture, step_name, step_matcher)
#define WHEN_F(fixture, step_name, step_matcher) CUKE_STEP_F_(fixture, step_name, step_matcher)
#define THEN_F(fixture, step_name, step_matcher) CUKE_STEP_F_(fixture, step_name, step_matcher)

#define GIVEN_C(context, step_name, step_matcher) CUKE_STEP_C_(context, step_name, step_matcher)
#define WHEN_C(context, step_name, step_matcher) CUKE_STEP_C_(context, step_name, step_matcher)
#define THEN_C(context, step_name, step_matcher) CUKE_STEP_C_(context, step_name, step_matcher)

#define GIVEN(context, step_name, step_matcher) GIVEN_C(context, step_name, step_matcher)
#define WHEN(context, step_name, step_matcher) WHEN_C(context, step_name, step_matcher)
#define THEN(context, step_name, step_matcher) THEN_C(context, step_name, step_matcher)

#define CUKE_FIXTURE(FixtureName, ...) \
    class FixtureName : public ::cukebins::internal::CukeFixture<__VA_ARGS__>

#define CUKE_CONTEXT(ContextName) \
    struct ContextName; \
    CUKE_FIXTURE(ContextName##_DefaultFixture, ContextName) {}; \
    struct ContextName

#endif /* CUKEBINS_HPP_ */
