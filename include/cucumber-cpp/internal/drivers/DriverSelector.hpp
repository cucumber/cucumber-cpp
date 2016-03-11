#if defined(GTEST_INCLUDE_GTEST_GTEST_H_)
    #include "GTestDriver.hpp"
#elif defined(BOOST_TEST_CASE)
    #include "BoostDriver.hpp"
#elif defined(CPPSPEC_H_)
    #include "CppSpecDriver.hpp"
#else // No test framework
    #ifdef CUKE_ENABLE_GENERICDRIVER
        #include "GenericDriver.hpp"
    #else
        #error Please #inlude testing framework before cucumber-cpp or #define CUKE_ENABLE_GENERICDRIVER explicitly 
    #endif
#endif
