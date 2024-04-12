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

function(cuke_set_environment environment)
    set(options)
    set(oneValueArgs )
    set(multiValueArgs RUNPATH BINPATH)
    
    cmake_parse_arguments(CUKE_ENV "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    
    if(NOT "${CUKE_ENV_UNPARSED_ARGUMENTS}" STREQUAL "")
        message(
            FATAL_ERROR
                "unparsed arguments in call to cuke_set_environment: ${CUKE_ENV_UNPARSED_ARGUMENTS} from ${CMAKE_CURRENT_LIST_FILE}"
        )
    endif()

    if( NOT "${CUKE_ENV_RUNPATH}" STREQUAL "")
        if(WIN32)
            string(REPLACE ";" "\;" CUKE_ENV_RUNPATH "${CUKE_ENV_RUNPATH}")
        endif()
        set(RUNPATH "$<IF:$<PLATFORM_ID:Windows>,PATH,LD_LIBRARY_PATH>")
        list(APPEND environment "$<IF:$<PLATFORM_ID:Windows>,PATH,LD_LIBRARY_PATH>=path_list_prepend:$<SHELL_PATH:${CUKE_ENV_RUNPATH}>")
    endif()
    if( NOT "${CUKE_ENV_BINPATH}" STREQUAL "")
        if(WIN32)
            string(REPLACE ";" "\;" CUKE_ENV_BINPATH "${CUKE_ENV_BINPATH}")
        endif()
        list(APPEND environment "PATH=path_list_prepend:$<SHELL_PATH:${CUKE_ENV_BINPATH}>")
    endif()
    
    set(${environment} ${${environment}} PARENT_SCOPE)
endfunction()

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
    cuke_set_environment( QTTEST_ENVIRONMENT RUNPATH "${CUKE_QT_RUNTIME_PATH}" )
    set_tests_properties(QtTestDriverTest PROPERTIES ENVIRONMENT_MODIFICATION ${QTTEST_ENVIRONMENT})
endif()

cuke_add_driver_test(integration/drivers/GenericDriverTest)
