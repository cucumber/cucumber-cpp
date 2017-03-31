#.rst:
# FindValgrind
# -------
#
# The module defines the following variables:
#
# ``VALGRIND_EXECUTABLE``
#   Path to Valgrind command-line client.
# ``Valgrind_FOUND``
#   True if the Valgrind command-line client was found.
# ``VALGRIND_VERSION_STRING``
#   The version of Valgrind found.
#
# Example usage:
#
# .. code-block:: cmake
#
#    find_package(Valgrind)
#    if(Valgrind_FOUND)
#      message("Valgrind found: ${VALGRIND_EXECUTABLE}")
#    endif()

#=============================================================================
# Copyright (c) 2017 Giel van Schijndel
#
# CMake - Cross Platform Makefile Generator
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

find_program(VALGRIND_EXECUTABLE
  NAMES valgrind
  DOC "Valgrind command line executable"
)
mark_as_advanced(VALGRIND_EXECUTABLE)

if(VALGRIND_EXECUTABLE)
    execute_process(COMMAND ${VALGRIND_EXECUTABLE} --version
                    OUTPUT_VARIABLE VALGRIND_VERSION_STRING
                    ERROR_QUIET
                    OUTPUT_STRIP_TRAILING_WHITESPACE)
    if (VALGRIND_VERSION_STRING MATCHES "^valgrind-[0-9]")
        string(REPLACE "valgrind-" "" VALGRIND_VERSION_STRING "${VALGRIND_VERSION_STRING}")
    endif()
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Valgrind
                                  REQUIRED_VARS VALGRIND_EXECUTABLE
                                  VERSION_VAR VALGRIND_VERSION_STRING)
