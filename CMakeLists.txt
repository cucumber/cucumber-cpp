cmake_minimum_required(VERSION 2.8)

project(CukeBins)

enable_testing()

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)

#
# Generic Compiler Flags
#

if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Weffc++")
elseif(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DNOMINMAX") # exclude M$ min/max macros
    #set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /analyze")
endif()

#
# Boost
#

# "An external test runner utility is required to link with dynamic library" (Boost User's Guide)
set(Boost_USE_STATIC_LIBS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBOOST_TEST_DYN_LINK")
find_package(Boost COMPONENTS unit_test_framework)

if(MSVC)
    set(Boost_USE_STATIC_LIBS ON)
else()
    set(Boost_USE_MULTITHREAD ON)
endif()
find_package(Boost COMPONENTS thread system regex date_time)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
endif()

#
# GTest
#

find_package(GTest)

#
# CppSpec
#

find_package(CppSpec)

#
# CukeBins
#

set(CUKEBINS_LIBRARIES
    ${Boost_THREAD_LIBRARY} ${Boost_SYSTEM_LIBRARY}
    ${Boost_REGEX_LIBRARY} ${Boost_DATE_TIME_LIBRARY})
set(CUKEBINS_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include)

include_directories(${CUKEBINS_INCLUDE_DIRS})

add_subdirectory(tests)
add_subdirectory(examples)
