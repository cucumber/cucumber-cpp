# Get the Google C++ Mocking Framework.
# (This file is almost an copy of the original FindGTest.cmake file,
#  altered to download and compile GMock and GTest,
#  feel free to use it as it is or modify it for your own needs.)
#
# Defines the following variables:
#
#   GMOCK_FOUND - Got the Google Mocking framework
#   GMOCK_INCLUDE_DIRS - GMock include directory
#   GTEST_INCLUDE_DIRS - GTest include direcotry 
#
# Also defines the library variables below as normal variables
#
#   GMOCK_BOTH_LIBRARIES - Both libgmock & libgmock-main
#   GMOCK_LIBRARIES - libgmock
#   GMOCK_MAIN_LIBRARIES - libgmock-main
#
#   GTEST_BOTH_LIBRARIES - Both libgtest & libgtest-main
#   GTEST_LIBRARIES - libgtest
#   GTEST_MAIN_LIBRARIES - libgtest-main
#
# Accepts the following variables as input:
#
#   GMOCK_ROOT - The root directory of the gmock install prefix
#
#   GMOCK_SRC_DIR -The directory of the gmock sources
#
#   GMOCK_VER - The version of the gmock sources to be downloaded
#
#-----------------------
# Example Usage:
#
#    set(GMOCK_ROOT "~/gmock")
#    find_package(GMock REQUIRED)
#    include_directories(${GMOCK_INCLUDE_DIRS})
#
#    add_executable(foo foo.cc)
#    target_link_libraries(foo ${GMOCK_BOTH_LIBRARIES})
#
#=============================================================================
# Copyright (c) 2016 Kamil Strzempowicz
# Copyright (c) 2011 Matej Svec
#
# CMake - Cross Platform Makefile Generator
# Copyright 2000-2016 Kitware, Inc.
# Copyright 2000-2011 Insight Software Consortium
# All rights reserved.
# 
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
# 
# * Redistributions of source code must retain the above copyright
#   notice, this list of conditions and the following disclaimer.
# 
# * Redistributions in binary form must reproduce the above copyright
#   notice, this list of conditions and the following disclaimer in the
#   documentation and/or other materials provided with the distribution.
# 
# * Neither the names of Kitware, Inc., the Insight Software Consortium,
#   nor the names of their contributors may be used to endorse or promote
#   products derived from this software without specific prior written
#   permission.
# 
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
# 
# ------------------------------------------------------------------------------
# 
# The above copyright and license notice applies to distributions of
# CMake in source and binary form.  Some source files contain additional
# notices of original copyright by their contributors; see each source
# for details.  Third-party software packages supplied with CMake under
# compatible licenses provide their own copyright notices documented in
# corresponding subdirectories.
# 
# ------------------------------------------------------------------------------
# 
# CMake was initially developed by Kitware with the following sponsorship:
# 
#  * National Library of Medicine at the National Institutes of Health
#    as part of the Insight Segmentation and Registration Toolkit (ITK).
# 
#  * US National Labs (Los Alamos, Livermore, Sandia) ASC Parallel
#    Visualization Initiative.
# 
#  * National Alliance for Medical Image Computing (NAMIC) is funded by the
#    National Institutes of Health through the NIH Roadmap for Medical Research,
#    Grant U54 EB005149.
# 
#  * Kitware, Inc.
#=============================================================================
# Thanks to Daniel Blezek <blezek@gmail.com> for the GTEST_ADD_TESTS code

function(GTEST_ADD_TESTS executable extra_args)
    if(NOT ARGN)
        message(FATAL_ERROR "Missing ARGN: Read the documentation for GTEST_ADD_TESTS")
    endif()
    if(ARGN STREQUAL "AUTO")
        # obtain sources used for building that executable
        get_property(ARGN TARGET ${executable} PROPERTY SOURCES)
    endif()
    set(gtest_case_name_regex ".*\\( *([A-Za-z_0-9]+) *, *([A-Za-z_0-9]+) *\\).*")
    set(gtest_test_type_regex "(TYPED_TEST|TEST_?[FP]?)")
    foreach(source ${ARGN})
        file(READ "${source}" contents)
        string(REGEX MATCHALL "${gtest_test_type_regex} *\\(([A-Za-z_0-9 ,]+)\\)" found_tests ${contents})
        foreach(hit ${found_tests})
          string(REGEX MATCH "${gtest_test_type_regex}" test_type ${hit})

          # Parameterized tests have a different signature for the filter
          if("x${test_type}" STREQUAL "xTEST_P")
            string(REGEX REPLACE ${gtest_case_name_regex}  "*/\\1.\\2/*" test_name ${hit})
          elseif("x${test_type}" STREQUAL "xTEST_F" OR "x${test_type}" STREQUAL "xTEST")
            string(REGEX REPLACE ${gtest_case_name_regex} "\\1.\\2" test_name ${hit})
          elseif("x${test_type}" STREQUAL "xTYPED_TEST")
            string(REGEX REPLACE ${gtest_case_name_regex} "\\1/*.\\2" test_name ${hit})
          else()
            message(WARNING "Could not parse GTest ${hit} for adding to CTest.")
            continue()
          endif()
          add_test(NAME ${test_name} COMMAND ${executable} --gtest_filter=${test_name} ${extra_args})
        endforeach()
    endforeach()
endfunction()

include(ExternalProject)

if(UNIX)
    find_package(Threads REQUIRED) # GTest needs this and it's a static library
    set(Suffix ".a")
else()
    set(Suffix ".lib")
endif()

if("${GMOCK_SRC_DIR}" STREQUAL "") 
   if("${GMOCK_VER}" STREQUAL "") 
       set(GMOCK_VER "1.7.0")
   endif() 
   message(STATUS "Downloading GMock / GTest version ${GMOCK_VER}")
   ExternalProject_Add(
       gmock
       URL "https://googlemock.googlecode.com/files/gmock-${GMOCK_VER}.zip"
       PREFIX ${GMOCK_ROOT}
       INSTALL_COMMAND ""
       LOG_DOWNLOAD ON
       LOG_CONFIGURE ON
       LOG_BUILD ON
   )
else()
   Message(STATUS "Building Gmock / Gtest from dir ${GMOCK_SRC_DIR}")
   ExternalProject_Add(
        gmock
        SOURCE_DIR ${GMOCK_SRC_DIR}
        PREFIX ${GMOCK_ROOT}
        INSTALL_COMMAND ""
        LOG_DOWNLOAD OFF
        LOG_CONFIGURE ON
        LOG_BUILD ON
    )
endif()

add_library(libgmock IMPORTED STATIC GLOBAL)
add_dependencies(libgmock gmock)
ExternalProject_Get_Property(gmock source_dir binary_dir)

if(MSVC) 
    set(MS_DIR "/${CMAKE_BUILD_TYPE}") 
endif()

set(GTEST_LIB_DIR "${binary_dir}/gtest${MS_DIR}")
set(GMOCK_LIB_DIR "${binary_dir}${MS_DIR}")

set_target_properties(libgmock PROPERTIES
    "IMPORTED_LOCATION" "${GMOCK_LIB_DIR}/${CMAKE_FIND_LIBRARY_PREFIXES}gmock.${Suffix}"
    "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
    "INTERFACE_INCLUDE_DIRECTORIES" "${source_dir}/include"
)

set(GMOCK_INCLUDE_DIR ${source_dir}/include)
mark_as_advanced(GMOCK_INCLUDE_DIR)
set(GTEST_INCLUDE_DIR ${source_dir}/gtest/include)
mark_as_advanced(GTEST_INCLUDE_DIR)

set(GTEST_LIBRARY "${GTEST_LIB_DIR}/${CMAKE_FIND_LIBRARY_PREFIXES}gtest${Suffix}")
set(GTEST_MAIN_LIBRARY "${GTEST_LIB_DIR}/${CMAKE_FIND_LIBRARY_PREFIXES}gtest_main${Suffix}")
set(GMOCK_LIBRARY "${GMOCK_LIB_DIR}/${CMAKE_FIND_LIBRARY_PREFIXES}gmock${Suffix}")
set(GMOCK_MAIN_LIBRARY "${GMOCK_LIB_DIR}/${CMAKE_FIND_LIBRARY_PREFIXES}gmock_main${Suffix}")

FIND_PACKAGE_HANDLE_STANDARD_ARGS(GMock DEFAULT_MSG GMOCK_LIBRARY GMOCK_INCLUDE_DIR GMOCK_MAIN_LIBRARY)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GTest DEFAULT_MSG GTEST_LIBRARY GTEST_INCLUDE_DIR GTEST_MAIN_LIBRARY)

if(GMOCK_FOUND)
    set(GMOCK_INCLUDE_DIRS ${GMOCK_INCLUDE_DIR})
    set(GMOCK_LIBRARIES ${GMOCK_LIBRARY})
    set(GMOCK_MAIN_LIBRARIES ${GMOCK_MAIN_LIBRARY})
    set(GMOCK_BOTH_LIBRARIES ${GMOCK_LIBRARIES} ${GMOCK_MAIN_LIBRARIES})
    if(VERBOSE)
      Message(STATUS "GMock includes: ${GMOCK_INCLUDE_DIRS}")
      Message(STATUS "GMock libs: ${GMOCK_BOTH_LIBRARIES}")
    endif()
endif()
if(GTEST_FOUND)
    set(GTEST_INCLUDE_DIRS ${GTEST_INCLUDE_DIR})
    set(GTEST_LIBRARIES ${GTEST_LIBRARY})
    set(GTEST_MAIN_LIBRARIES ${GTEST_MAIN_LIBRARY})
    set(GTEST_BOTH_LIBRARIES ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES})
    if(VERBOSE)
        Message(STATUS "GTest includes: ${GTEST_INCLUDE_DIRS}")
        Message(STATUS "GTest libs: ${GTEST_BOTH_LIBRARIES}")
    endif()
endif()

