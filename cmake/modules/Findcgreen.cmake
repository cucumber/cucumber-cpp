#.rst:
# Findcgreen
# ----------
#
# Find the cgreen testing framework.
#
# Result variables
# ^^^^^^^^^^^^^^^^
#
# This module will set the following variables in your project:
#
# ``CGREEN_FOUND``
#   true if the cgreen headers, libraries and executable were found
# ``CGREEN_INCLUDE_DIRS``
#   the directory containing the cgreen headers
# ``CGREEN_LIBRARIES``
#   cgreen libraries to be linked
# ``CGREEN_EXECUTABLE``
#   cgreen executable
#
# Cache variables
# ^^^^^^^^^^^^^^^
#
# The following cache variables may also be set:
#
# ``CGREEN_INCLUDE_DIR``
#   the directory containing the cgreen headers
# ``CGREEN_LIBRARY``
#   the path to the cgreen library
# ``CGREEN_RUNNER``
#   cgreen-runner executable

find_path(CGREEN_INCLUDE_DIR cgreen/cgreen.h)
find_library(CGREEN_LIBRARY NAMES cgreen libcgreen)
find_program(CGREEN_RUNNER cgreen-runner)
mark_as_advanced(CGREEN_INCLUDE_DIR CGREEN_LIBRARY CGREEN_RUNNER)

find_package_handle_standard_args(cgreen
  REQUIRED_VARS CGREEN_LIBRARY CGREEN_INCLUDE_DIR CGREEN_RUNNER)

if(CGREEN_FOUND)
  set(CGREEN_LIBRARIES ${CGREEN_LIBRARY})
  set(CGREEN_INCLUDE_DIRS "${CGREEN_INCLUDE_DIR}")
  set(CGREEN_EXECUTABLE "${CGREEN_RUNNER}")
endif()

