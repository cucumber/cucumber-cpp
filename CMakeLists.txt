cmake_minimum_required(VERSION 2.8)

project(CukeBins)

enable_testing()

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)

if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Weffc++")
elseif(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DNOMINMAX") # exclude M$ min/max macros
    set (Boost_USE_STATIC_LIBS ON)
    set (Boost_USE_MULTITHREAD ON)
    #set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /analyze")
endif()

find_package(Boost REQUIRED COMPONENTS thread system regex date_time)
include_directories(${Boost_INCLUDE_DIRS})

find_package(GTest)
find_package(CppSpec)

set(CUKE_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CUKE_INCLUDE_DIRS})

add_subdirectory(tests)
add_subdirectory(examples)
