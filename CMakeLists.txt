cmake_minimum_required(VERSION 2.8.12)

project(Cucumber-Cpp)

set(CUKE_USE_STATIC_BOOST ${WIN32} CACHE BOOL "Statically link Boost (except boost::test)")
set(CUKE_USE_STATIC_GTEST ON CACHE BOOL "Statically link Google Test")

set(CUKE_DISABLE_BOOST_TEST OFF CACHE BOOL "Disable boost:test")
set(CUKE_DISABLE_GTEST OFF CACHE BOOL "Disable Google Test framework")
set(CUKE_DISABLE_UNIT_TESTS OFF CACHE BOOL "Disable unit tests")
set(CUKE_DISABLE_E2E_TESTS OFF CACHE BOOL "Disable end-to-end tests")
set(CUKE_ENABLE_EXAMPLES OFF CACHE BOOL "Enable the examples")
set(GMOCK_SRC_DIR "" CACHE STRING "Google Mock framework sources path (otherwise downloaded)")
set(GMOCK_VER "1.7.0" CACHE STRING "Google Mock framework version to be used")
option(VALGRIND_TESTS "Run tests within Valgrind" OFF)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/modules)

#
# Generic Compiler Flags
#

if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Weffc++")
    # TODO: A better fix should handle ld's --as-needed flag
    if(UNIX)
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Xlinker '--no-as-needed'")
    endif()
elseif(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DNOMINMAX") # exclude M$ min/max macros
    #set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /analyze")
endif()

#
# Boost
#

if(MSVC11)
    # Boost 1.51 fixed a bug with MSVC11
    message(STATUS "Forcing Boost 1.51+ on MSVC11")
    set(BOOST_MIN_VERSION "1.51")
else()
    set(BOOST_MIN_VERSION "1.46")
endif()

set(Boost_USE_STATIC_RUNTIME OFF)
set(CUKE_CORE_BOOST_LIBS thread system regex date_time program_options filesystem)
if(NOT CUKE_DISABLE_BOOST_TEST)
    # "An external test runner utility is required to link with dynamic library" (Boost User's Guide)
    set(Boost_USE_STATIC_LIBS OFF)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBOOST_TEST_DYN_LINK")
    find_package(Boost ${BOOST_MIN_VERSION} COMPONENTS unit_test_framework)
endif()

if(CUKE_USE_STATIC_BOOST)
    set(Boost_USE_STATIC_LIBS ON)
    find_package(Boost ${BOOST_MIN_VERSION} COMPONENTS ${CUKE_CORE_BOOST_LIBS} REQUIRED)

    if(NOT MSVC)
        find_package(Threads)
        set(CUKE_EXTRA_LIBRARIES ${CUKE_EXTRA_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
    endif()
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBOOST_ALL_DYN_LINK")
    set(Boost_USE_STATIC_LIBS OFF)
    find_package(Boost ${BOOST_MIN_VERSION} COMPONENTS ${CUKE_CORE_BOOST_LIBS} REQUIRED)
endif()

include_directories(${Boost_INCLUDE_DIRS})
set(CUKE_EXTRA_LIBRARIES ${CUKE_EXTRA_LIBRARIES} ${Boost_THREAD_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${Boost_REGEX_LIBRARY} ${Boost_DATE_TIME_LIBRARY} ${Boost_PROGRAM_OPTIONS_LIBRARY} ${Boost_FILESYSTEM_LIBRARY})

#
# GTest
#

if(NOT CUKE_DISABLE_GTEST)
    set(GTEST_USE_STATIC_LIBS ${CUKE_USE_STATIC_GTEST})
    if(NOT GMOCK_ROOT)
        set(GMOCK_ROOT "${CMAKE_CURRENT_BINARY_DIR}/gmock")
    endif()
    find_package(GMock REQUIRED)

    if(GTEST_FOUND)
        set(CUKE_GTEST_LIBRARIES
            GTest::GTest
            )
    endif()
    if(GMOCK_FOUND)
        set(CUKE_GMOCK_LIBRARIES
            GTest::GTest
            GMock::GMock
            GMock::Main
            )
    endif()
endif()

#
# Valgrind
#

if(VALGRIND_TESTS)
    find_program(VALGRIND_CMD valgrind)
    if(NOT VALGRIND_CMD)
        message(SEND_ERROR "Failed to find 'valgrind', cannot run tests with it as requested")
    endif()
    set(VALGRIND_ARGS --error-exitcode=2 --leak-check=full --show-leak-kinds=all --undef-value-errors=no)
    function(add_test name)
        if(NOT name STREQUAL "NAME")
            _add_test(${VALGRIND_CMD} ${VALGRIND_ARGS} ${ARGV})
            return()
        endif()

        set(TEST_ARGS ${ARGV})
        list(FIND TEST_ARGS COMMAND COMMAND_IDX)
        if(COMMAND_IDX EQUAL -1)
            message(AUTHOR_WARNING "Weird command-line given to add_test(), not injecting valgrind")
            _add_test(${ARGV})
            return()
        endif()

        # We want to operate on the COMMAND, not the 'COMMAND' keyword preceding it
        math(EXPR COMMAND_IDX "${COMMAND_IDX} + 1")

        # Keep add_test() behaviour of replacing COMMANDs, when executable targets, with their output files
        list(GET TEST_ARGS ${COMMAND_IDX} COMMAND)
        if(TARGET ${COMMAND})
            get_target_property(COMMAND_TYPE ${COMMAND} TYPE)
            if(COMMAND_TYPE STREQUAL "EXECUTABLE")
                # Inserting first, removing the original only after that, because inserting to the end of the list doesn't work
                math(EXPR ORIG_COMMAND_IDX "${COMMAND_IDX} + 1")
                list(INSERT TEST_ARGS ${COMMAND_IDX} "$<TARGET_FILE:${COMMAND}>")
                list(REMOVE_AT TEST_ARGS ${ORIG_COMMAND_IDX})
            endif()
        endif()

        # Insert the valgrind command line, before the command to execute
        list(INSERT TEST_ARGS ${COMMAND_IDX} ${VALGRIND_CMD} ${VALGRIND_ARGS})

        _add_test(${TEST_ARGS})
    endfunction()
endif()

#
# Cucumber-Cpp
#

set(CUKE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

include_directories(${CUKE_INCLUDE_DIR})

set(CUKE_LIBRARIES cucumber-cpp ${CUKE_EXTRA_LIBRARIES})

add_subdirectory(src)

#
# Tests
#

if(CUKE_DISABLE_UNIT_TESTS)
    message(STATUS "Skipping unit tests")
else()
    enable_testing()
    add_subdirectory(tests)
endif()

if(CUKE_DISABLE_E2E_TESTS)
    message(STATUS "Skipping end-to-end tests")
else()
    find_program(CUCUMBER_RUBY cucumber)
    if(CUCUMBER_RUBY)
        message(STATUS "Found Cucumber")
        set(CUKE_FEATURES_DIR "${CMAKE_SOURCE_DIR}/features")
        set(CUKE_FEATURES_TMP "${CMAKE_BINARY_DIR}/tmp")
        set(CUKE_TEST_FEATURES_DIR "${CUKE_FEATURES_TMP}/test_features")
        set(CUKE_DYNAMIC_CPP_STEPS "${CUKE_TEST_FEATURES_DIR}/step_definitions/cpp_steps.cpp")
        string(REPLACE "/tmp" "${CMAKE_FILES_DIRECTORY}/e2e-steps.dir/tmp" CUKE_DYNAMIC_CPP_STEPS_OBJ "${CUKE_DYNAMIC_CPP_STEPS}${CMAKE_CXX_OUTPUT_EXTENSION}")

        file(WRITE ${CUKE_DYNAMIC_CPP_STEPS})
        add_executable(e2e-steps EXCLUDE_FROM_ALL ${CUKE_DYNAMIC_CPP_STEPS})
        target_link_libraries(e2e-steps ${CUKE_LIBRARIES})

        set(CUKE_COMPILE_DYNAMIC_CPP_STEPS '"${CMAKE_COMMAND}" --build "${CMAKE_BINARY_DIR}" --target e2e-steps')

        function(add_feature_target TARGET_NAME)
            add_custom_target(${TARGET_NAME}
                COMMAND ${CUCUMBER_RUBY}
                TEST_FEATURES_DIR=${CUKE_TEST_FEATURES_DIR}
                TMP_DIR=${CUKE_FEATURES_TMP}
                DYNAMIC_CPP_STEPS_SRC=${CUKE_DYNAMIC_CPP_STEPS}
                DYNAMIC_CPP_STEPS_EXE=${CMAKE_BINARY_DIR}/${CMAKE_CFG_INTDIR}/e2e-steps
                DYNAMIC_CPP_STEPS_OBJ=${CUKE_DYNAMIC_CPP_STEPS_OBJ}
                COMPILE_DYNAMIC_CPP_STEPS=${CUKE_COMPILE_DYNAMIC_CPP_STEPS}
                CUCUMBER_RUBY=${CUCUMBER_RUBY}
                ${ARGV1} ${ARGV2} ${ARGV3} ${ARGV4} ${ARGV5} ${ARGV6}
                ${CUKE_FEATURES_DIR}
                DEPENDS cucumber-cpp
                )
        endfunction(add_feature_target)

        add_feature_target(features --format progress)
        add_feature_target(features-pretty --format pretty)
        add_feature_target(features-wip --format pretty --tags @wip)

    else()
        message(WARNING "Could not find Cucumber: skipping end-to-end tests")
    endif()

endif()

#
# Examples
#

if(CUKE_ENABLE_EXAMPLES)
    add_subdirectory(examples)
endif()
