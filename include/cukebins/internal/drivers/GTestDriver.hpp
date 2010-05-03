#ifndef CUKEBINS_GTESTDRIVER_HPP_
#define CUKEBINS_GTESTDRIVER_HPP_

#include <cukebins/internal/CukeCommands.hpp>
#include <cukebins/internal/CukeFixture.hpp>

#define CUKE_TEST_NAME_PREFIX_ "DISABLED_"
#define CUKE_TEST_FULLNAME_(feature_name, step_name) #feature_name "." CUKE_TEST_NAME_PREFIX_ #step_name
#define CUKE_STEPCLASS_INHERITANCE_(parent_class) : public parent_class
#define CUKE_INHERITED_CONSTRUCTOR_(parent_class) : parent_class ()
#define CUKE_STEPCLASS_TESTBODY_NAME_ TestBody
#define CUKE_STEPCLASS_OTHER_DECLARATIONS_(feature_class, step_name) \
    static ::testing::TestInfo* const test_info_; \
    GTEST_DISALLOW_COPY_AND_ASSIGN_(CUKE_STEP_CLASS_NAME_(feature_class, step_name));
#define CUKE_STEPCLASS_OTHER_DEFINITION_(feature_class, step_name, parent_class, parent_id) \
    ::testing::TestInfo* const CUKE_STEP_CLASS_NAME_(feature_class, step_name) ::test_info_ = \
        ::testing::internal::MakeAndRegisterTestInfo( \
            #feature_class, CUKE_TEST_NAME_PREFIX_ #step_name, "", "", \
            (parent_id), parent_class::SetUpTestCase, parent_class::TearDownTestCase, \
            new ::testing::internal::TestFactoryImpl<CUKE_STEP_CLASS_NAME_(feature_class, step_name)>);

namespace cukebins {
namespace internal {

template<class T>
class CukeFixture : public BaseCukeFixture<T>, public ::testing::Test {
};

class GTestCommands : public AbstractCommands {
protected:
    static bool initialized;

private:
    void initGTest() {
        int fake_argc = 1;
        char *fake_argv[1];
        fake_argv[0] = (char *) "cukebins";
        ::testing::InitGoogleTest(&fake_argc, fake_argv);
        initialized = true;
    }

public:
    void beginScenario() {
        if (!initialized) {
            initGTest();
        }
    }

protected:
    InvokeResult invokeNoArgs(StepInfo::id_type id) {
        InvokeResult result;
        ::testing::GTEST_FLAG( also_run_disabled_tests) = true;
        ::testing::GTEST_FLAG( throw_on_failure) = true;
        ::testing::GTEST_FLAG( break_on_failure) = false;
        ::testing::GTEST_FLAG( catch_exceptions) = true;
        const StepInfo &stepInfo = stepManager.getStep(id);
        if (stepInfo.id == id) {
            ::testing::GTEST_FLAG( filter) = stepInfo.testName.c_str();
            try {
                result.success = (::testing::UnitTest::GetInstance()->Run() == 0);
            } catch (...) {
                result.success = false;
            }
        } else {
            result.success = false;
        }
        return result;
    }
};

bool GTestCommands::initialized = false;

typedef GTestCommands CukeCommands;

}
}

#endif /* CUKEBINS_GTESTDRIVER_HPP_ */
