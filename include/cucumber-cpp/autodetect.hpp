#if defined(GTEST_INCLUDE_GTEST_GTEST_H_)
    #include "internal/drivers/GTestDriver.hpp"
#elif defined(BOOST_TEST_CASE)
    #include "internal/drivers/BoostDriver.hpp"
#elif defined(CPPSPEC_H_)
    #include "internal/drivers/CppSpecDriver.hpp"
#else
    #error No test framework found: Please #include testing framework before cucumber-cpp or #include defs.hpp for Generic Driver
#endif

#include "internal/step/StepManager.hpp"
#include "internal/hook/HookRegistrar.hpp"
#include "internal/ContextManager.hpp"
#include "internal/Macros.hpp"
