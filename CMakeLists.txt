cmake_minimum_required(VERSION 2.8)

project(Cucumber-Cpp)

set(CUKE_USE_STATIC_BOOST ${MSVC} CACHE BOOL "Statically link Boost (except boost::test)")
set(CUKE_DISABLE_BOOST_TEST OFF CACHE BOOL "Disable boost:test")
set(CUKE_DISABLE_CPPSPEC OFF CACHE BOOL "Disable CppSpec")
set(CUKE_DISABLE_GTEST OFF CACHE BOOL "Disable Google Test framework")
set(CUKE_DISABLE_FUNCTIONAL OFF CACHE BOOL "Disable Functional Tests")
set(CUKE_ENABLE_EXAMPLES OFF CACHE BOOL "Enable the examples")

enable_testing()

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)

#
# Generic Compiler Flags
#

if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Weffc++")
    # TODO: A better fix should handle ld's --as-needed flag 
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Xlinker '--no-as-needed'")
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
    set(BOOST_MIN_VERSION "1.40")
endif()

set(CUKE_CORE_BOOST_LIBS thread system regex date_time)
if(NOT CUKE_DISABLE_BOOST_TEST)
    set(CUKE_TEST_BOOST_LIBS unit_test_framework)
endif()

if(CUKE_USE_STATIC_BOOST)
    set(CUKE_STATIC_BOOST_LIBS ${CUKE_CORE_BOOST_LIBS})
    # "An external test runner utility is required to link with dynamic library" (Boost User's Guide)
    set(CUKE_DYNAMIC_BOOST_LIBS ${CUKE_TEST_BOOST_LIBS})
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBOOST_TEST_DYN_LINK")

    if(NOT MSVC)
        find_package(Threads)
        set(CUKE_EXTRA_LIBRARIES ${CUKE_EXTRA_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
    endif()
else()
    set(CUKE_DYNAMIC_BOOST_LIBS ${CUKE_CORE_BOOST_LIBS} ${CUKE_TEST_BOOST_LIBS})
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBOOST_ALL_DYN_LINK")
endif()

if(CUKE_STATIC_BOOST_LIBS)
    set(Boost_USE_STATIC_LIBS ON)
    find_package(Boost ${BOOST_MIN_VERSION} COMPONENTS ${CUKE_STATIC_BOOST_LIBS})
endif()

if(CUKE_DYNAMIC_BOOST_LIBS)
    set(Boost_USE_STATIC_LIBS OFF)
    find_package(Boost ${BOOST_MIN_VERSION} COMPONENTS ${CUKE_DYNAMIC_BOOST_LIBS})
endif()

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    set(CUKE_EXTRA_LIBRARIES ${CUKE_EXTRA_LIBRARIES} ${Boost_THREAD_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${Boost_REGEX_LIBRARY} ${Boost_DATE_TIME_LIBRARY})
endif()

#
# CppSpec
#

if(NOT CUKE_DISABLE_CPPSPEC)
    find_package(CppSpec)
endif()

#
# GTest
#

if(NOT CUKE_DISABLE_GTEST)
    find_package(GTest)
    find_package(GMock)
    if(GMOCK_FOUND AND GTEST_FOUND)
        set(CUKE_TEST_LIBRARIES ${GTEST_LIBRARIES} ${GMOCK_BOTH_LIBRARIES})
        if(UNIX)
            find_package(Threads) # GTest needs this and it's a static library
            set(CUKE_TEST_LIBRARIES ${CUKE_TEST_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
        endif()
    endif()
endif()

#
# Cucumber-Cpp
#

set(CUKE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

include_directories(${CUKE_INCLUDE_DIR})

set(CUKE_LIBRARIES cucumber-cpp ${CUKE_EXTRA_LIBRARIES})

add_subdirectory(src)
add_subdirectory(tests)

if(CUKE_ENABLE_EXAMPLES)
    add_subdirectory(examples)
endif()

#
# Functional Tests
#

if(NOT CUKE_DISABLE_FUNCTIONAL)

    find_program(BUNDLE bundle)
    if(BUNDLE)
        message(STATUS "Installing gem dependencies")
        execute_process(COMMAND ${BUNDLE} WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
    else()
        message(WARNING "Could not find Bundler: skipping Ruby Gem management")
    endif()

    find_program(CUCUMBER_RUBY cucumber)
    if(CUCUMBER_RUBY)
        message(STATUS "Found Cucumber")
        set(CUKE_FEATURES_DIR ${CMAKE_SOURCE_DIR}/features)
        set(CUKE_FEATURES_TMP ${CMAKE_BINARY_DIR}/tmp)
        set(CUKE_TEST_FEATURES_DIR ${CUKE_FEATURES_TMP}/test_features)
        set(CUKE_DYNAMIC_CPP_STEPS ${CUKE_TEST_FEATURES_DIR}/step_definitions/cpp_steps.cpp)

        file(WRITE ${CUKE_DYNAMIC_CPP_STEPS})
        add_executable(functional-steps EXCLUDE_FROM_ALL ${CUKE_DYNAMIC_CPP_STEPS})
        target_link_libraries(functional-steps ${CUKE_LIBRARIES})

        # TODO It does not escape paths
        set(CUKE_COMPILE_DYNAMIC_CPP_STEPS "${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target functional-steps")

        function(add_feature_target TARGET_NAME)
            add_custom_target(${TARGET_NAME}
                COMMAND ${CUCUMBER_RUBY}
                TEST_FEATURES_DIR=${CUKE_TEST_FEATURES_DIR}
                TMP_DIR=${CUKE_FEATURES_TMP}
                DYNAMIC_CPP_STEPS_SRC=${CUKE_DYNAMIC_CPP_STEPS}
                DYNAMIC_CPP_STEPS_EXE=${CMAKE_BINARY_DIR}/functional-steps
                COMPILE_DYNAMIC_CPP_STEPS=${CUKE_COMPILE_DYNAMIC_CPP_STEPS}
                ${ARGV1} ${ARGV2} ${ARGV3} ${ARGV4} ${ARGV5} ${ARGV6}
                ${CUKE_FEATURES_DIR}
                DEPENDS cucumber-cpp
            )
        endfunction(add_feature_target)

        add_feature_target(features --format progress)
        add_feature_target(features-pretty --format pretty)
        add_feature_target(features-wip --format pretty --tags @wip)

    else()
        message(WARNING "Could not find Cucumber: skipping functional tests")
    endif()

endif()

