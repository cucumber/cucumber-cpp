cmake_minimum_required(VERSION 3.16)

cmake_policy(SET CMP0063 NEW)

# CMP0077: option() honors normal variables
# https://cmake.org/cmake/help/latest/policy/CMP0077.html
cmake_policy(SET CMP0077 NEW)

# CMP0074: find_package() makes use of <PackageName>_ROOT variables
# https://cmake.org/cmake/help/latest/policy/CMP0074.html
cmake_policy(SET CMP0074 NEW)

project(Cucumber-Cpp)

option(CUKE_ENABLE_BOOST_TEST   "Enable Boost.Test framework" OFF)
option(CUKE_ENABLE_GTEST        "Enable Google Test framework" OFF)
option(CUKE_ENABLE_QT_5         "Enable Qt5 framework" OFF)
option(CUKE_ENABLE_QT_6         "Enable Qt6 framework" OFF)

option(CUKE_ENABLE_EXAMPLES     "Build examples" OFF)
option(CUKE_TESTS_UNIT          "Enable unit tests" OFF)

option(BUILD_SHARED_LIBS        "Generate shared libraries" OFF)
option(CUKE_CODE_COVERAGE       "Enable instrumentation for code coverage" OFF)
set(CUKE_ENABLE_SANITIZER       "OFF" CACHE STRING "Sanitizer to use for checking")
set_property(CACHE CUKE_ENABLE_SANITIZER PROPERTY STRINGS OFF "address" "thread" "undefined")
option(CUKE_TESTS_VALGRIND      "Enable tests within Valgrind" OFF)

if(NOT DEFINED CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS OFF)
elseif(CMAKE_CXX_STANDARD LESS 17)
    message(FATAL_ERROR "C++17 (above) is required")
endif()

if(CUKE_CODE_COVERAGE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")
endif()

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/modules)

#
# Option deprecation: if deprecated option is defined
# then print a warning and use its value instead
#

function(option_depr_message old prefer)
    message (DEPRECATION "${old} is deprecated in favor of ${prefer}")
endfunction()

function(option_depr old prefer)
    if(DEFINED ${old})
        option_depr_message(${old} ${prefer})
        set (${prefer} ${${old}} CACHE BOOL "Set from deprecated ${old}" FORCE)
    endif()
endfunction()

function(option_depr_invert old prefer)
    if(DEFINED ${old})
        option_depr_message(${old} ${prefer})
        set (${prefer} $<NOT:${${old}}> CACHE BOOL "Set from deprecated ${old}" FORCE)
    endif()
endfunction()


option_depr_invert (CUKE_DISABLE_BOOST_TEST   CUKE_ENABLE_BOOST_TEST)
option_depr_invert (CUKE_DISABLE_GTEST        CUKE_ENABLE_GTEST)
option_depr_invert (CUKE_DISABLE_QT_5         CUKE_ENABLE_QT_5)
option_depr_invert (CUKE_DISABLE_QT_6         CUKE_ENABLE_QT_6)
option_depr_invert (CUKE_DISABLE_UNIT_TESTS   CUKE_TESTS_UNIT)
option_depr        (VALGRIND_TESTS            CUKE_TESTS_VALGRIND)

#
# Check that at least one test framework is enabled
#

set(CUKE_TEST_FRAMEWORKS
    CUKE_ENABLE_BOOST_TEST
    CUKE_ENABLE_GTEST
    CUKE_ENABLE_QT_5
    CUKE_ENABLE_QT_6
)

set(TEST_FRAMEWORK_FOUND FALSE)
foreach(test_framework ${CUKE_TEST_FRAMEWORKS})
    if(${test_framework})
        set(TEST_FRAMEWORK_FOUND TRUE)
    endif()
endforeach()

if(NOT TEST_FRAMEWORK_FOUND)
    message(WARNING "No test framework enabled. At least one should be enabled. Options are: ${CUKE_TEST_FRAMEWORKS}.")
endif()

#
# Generic Compiler Flags
#

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_INIT} -Wall -Wextra -Wsuggest-override ${CMAKE_CXX_FLAGS}")
    # TODO: A better fix should handle ld's --as-needed flag
    if(UNIX AND NOT APPLE)
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Xlinker '--no-as-needed'")
    endif()
elseif(MSVC)
    set(CMAKE_CXX_FLAGS "-DNOMINMAX ${CMAKE_CXX_FLAGS}") # exclude M$ min/max macros
    set(CMAKE_CXX_FLAGS "/wd4996 ${CMAKE_CXX_FLAGS}") # don't warn about use of plain C functions without (non-portable) "_s" suffix
    #set(CMAKE_CXX_FLAGS_DEBUG "/analyze ${CMAKE_CXX_FLAGS_DEBUG}")
endif()

#
# Colored Terminal Output
#

if(UNIX AND (
       (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    OR (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.9))
    AND CMAKE_GENERATOR STREQUAL "Ninja")
  # These compilers generate coloured output, but by default only when their output channel is a
  # terminal (TTY/PTY). Ninja however captures all output in a pipe (per-subprocess), disabling
  # coloured compiler diagnostics. We forcefully enable it because Ninja, since 1.0.0
  # (ninja-build/ninja#198) takes care to strip VT100 CSI control sequences from the output if Ninja
  # itself is writing to a pipe instead of a terminal. As a result this should give us the best of
  # both worlds: coloured output when we're running in a terminal, plain output for editors, IDEs,
  # etc.
  set(CMAKE_CXX_FLAGS "-fdiagnostics-color=always ${CMAKE_CXX_FLAGS}")
endif()

#
# Boost
#

set(Boost_USE_STATIC_RUNTIME OFF)
if(CUKE_ENABLE_BOOST_TEST)
    # "An external test runner utility is required to link with dynamic library" (Boost User's Guide)
    set(CMAKE_CXX_FLAGS "-DBOOST_TEST_DYN_LINK ${CMAKE_CXX_FLAGS}")
    find_package(Boost 1.70 COMPONENTS unit_test_framework REQUIRED)
endif()

#
# GTest
#

if(CUKE_ENABLE_GTEST)
    find_package(GTest 1.11.0 REQUIRED)
endif()

#
# Qt
#

if(CUKE_ENABLE_QT_5 AND CUKE_ENABLE_QT_6)
    message(FATAL_ERROR "Qt version 5 and 6 enabled, please select at most one")
endif()


if(CUKE_ENABLE_QT_5)
    find_package(Qt5 REQUIRED COMPONENTS
        Core
        Gui
        Widgets
        Test
    )

    message(STATUS "Found Qt version: ${Qt5Core_VERSION_STRING}")
    if(Qt5Core_VERSION_STRING VERSION_LESS 5.15.0)
        add_library(Qt::Core    INTERFACE IMPORTED)
        add_library(Qt::Gui     INTERFACE IMPORTED)
        add_library(Qt::Widgets INTERFACE IMPORTED)
        add_library(Qt::Test    INTERFACE IMPORTED)
        set_target_properties(Qt::Core    PROPERTIES INTERFACE_LINK_LIBRARIES Qt5::Core   )
        set_target_properties(Qt::Gui     PROPERTIES INTERFACE_LINK_LIBRARIES Qt5::Gui    )
        set_target_properties(Qt::Widgets PROPERTIES INTERFACE_LINK_LIBRARIES Qt5::Widgets)
        set_target_properties(Qt::Test    PROPERTIES INTERFACE_LINK_LIBRARIES Qt5::Test   )
    endif()
endif()

if(CUKE_ENABLE_QT_6)
    find_package(Qt6 REQUIRED COMPONENTS
        Core
        Gui
        Widgets
        Test
    )

    message(STATUS "Found Qt version: ${Qt6Core_VERSION}")
endif()


#
# Sanitizers
#

if(CUKE_ENABLE_SANITIZER AND NOT ${CUKE_ENABLE_SANITIZER} EQUAL "OFF")
    message("Disabling valgrind when a sanitizer is enabled")
    set(CUKE_TESTS_VALGRIND OFF)

    if (WIN32)
        message(WARNING "The use of the sanatizers on Windows is not tested")
    endif()

    add_compile_options("-fsanitize=${CUKE_ENABLE_SANITIZER}")
    add_link_options("-fsanitize=${CUKE_ENABLE_SANITIZER}")
endif()



#
# Valgrind
#

if(CUKE_TESTS_VALGRIND)
    find_package(Valgrind REQUIRED)
    set(VALGRIND_ARGS --error-exitcode=2 --leak-check=full --undef-value-errors=no)
    if(NOT VALGRIND_VERSION_STRING VERSION_LESS 3.9)
        # Valgrind 3.9 no longer shows all leaks unless asked to
        list(APPEND VALGRIND_ARGS --show-leak-kinds=all)
    endif()
    function(add_test name)
        if(NOT name STREQUAL "NAME")
            _add_test(${VALGRIND_EXECUTABLE} ${VALGRIND_ARGS} ${ARGV})
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
        list(INSERT TEST_ARGS ${COMMAND_IDX} ${VALGRIND_EXECUTABLE} ${VALGRIND_ARGS})

        _add_test(${TEST_ARGS})
    endfunction()
endif()

#
# Cucumber-Cpp
#

set(CUKE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
add_subdirectory(src)

#
# Tests
#

if(CUKE_TESTS_UNIT)
    enable_testing()
    add_subdirectory(tests)
else()
    message(STATUS "Skipping unit tests")
endif()

#
# Examples
#

if(CUKE_ENABLE_EXAMPLES)
    add_subdirectory(examples)
endif()
