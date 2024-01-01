find_package(GTest REQUIRED)

add_library(utils INTERFACE
    utils/HookRegistrationFixture.hpp
    utils/ContextManagerTestDouble.hpp
    utils/DriverTestRunner.hpp
    utils/CukeCommandsFixture.hpp
    utils/StepManagerTestDouble.hpp
)
target_include_directories(utils INTERFACE
    .
)

function(cuke_add_driver_test TEST_FILE)
    get_filename_component(TEST_NAME ${TEST_FILE} NAME)
    message(STATUS "Adding " ${TEST_NAME})
    add_executable(${TEST_NAME} ${TEST_FILE}.cpp)
    target_link_libraries(${TEST_NAME} PRIVATE cucumber-cpp-internal utils ${ARGN})
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endfunction()

if(TARGET GTest::gmock_main)
    function(cuke_add_test TEST_FILE)
        get_filename_component(TEST_NAME ${TEST_FILE} NAME)
        message(STATUS "Adding " ${TEST_NAME})
        add_executable(${TEST_NAME} ${TEST_FILE}.cpp)
        target_link_libraries(${TEST_NAME} PRIVATE cucumber-cpp-internal utils ${ARGN} GTest::gmock_main)
        gtest_add_tests(${TEST_NAME} "" ${TEST_FILE}.cpp)
        # Run all tests in executable at once too. This ensures that the used fixtures get tested
        # properly too. Additionally gather the output in jUnit compatible output for CI.
        add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME} "--gtest_output=xml:TEST-${TEST_NAME}.xml")
    endfunction()

    # TODO Compile tests with the least possible code, not with the entire library
    cuke_add_test(integration/ContextHandlingTest)
    cuke_add_test(integration/HookRegistrationTest)
    cuke_add_test(integration/StepRegistrationTest)
    cuke_add_test(integration/TaggedHookRegistrationTest)
    if(NOT WIN32)
        cuke_add_test(integration/WireServerTest)
    endif()
    cuke_add_test(integration/WireProtocolTest)
    cuke_add_test(unit/BasicStepTest)
    cuke_add_test(unit/ContextManagerTest)
    cuke_add_test(unit/CukeCommandsTest)
    cuke_add_test(unit/RegexTest)
    cuke_add_test(unit/StepCallChainTest)
    cuke_add_test(unit/StepManagerTest)
    cuke_add_test(unit/TableTest)
    cuke_add_test(unit/TagTest)
endif()

if(TARGET GTest::gtest_main)
    cuke_add_driver_test(integration/drivers/GTestDriverTest GTest::gtest_main)
endif()

if(TARGET Boost::unit_test_framework)
    cuke_add_driver_test(integration/drivers/BoostDriverTest Boost::unit_test_framework)
endif()

if((TARGET Qt::Test)
    # FIXME: not including this in the test suite due to memory leak #190
    AND (NOT VALGRIND_TESTS)
    )
    cuke_add_driver_test(integration/drivers/QtTestDriverTest Qt::Test)
endif()

cuke_add_driver_test(integration/drivers/GenericDriverTest)
