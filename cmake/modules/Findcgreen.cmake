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

include(FindPackageHandleStandardArgs)

find_path(CGREEN_INCLUDE_DIR "cgreen/cgreen.h")
find_library(CGREEN_LIBRARY NAMES cgreen libcgreen)
find_program(CGREEN_RUNNER cgreen-runner)
mark_as_advanced(CGREEN_INCLUDE_DIR CGREEN_LIBRARY CGREEN_RUNNER)

find_package_handle_standard_args(cgreen
  REQUIRED_VARS CGREEN_LIBRARY CGREEN_INCLUDE_DIR CGREEN_RUNNER)

if(CGREEN_FOUND)
  add_library(Cgreen::Cgreen UNKNOWN IMPORTED)
  set_target_properties(Cgreen::Cgreen PROPERTIES
      IMPORTED_LINK_INTERFACE_LANGUAGES "C"
      IMPORTED_LOCATION "${CGREEN_LIBRARY}"
      INTERFACE_INCLUDE_DIRECTORIES "${CGREEN_INCLUDE_DIR}"
  )
  add_executable(Cgreen::runner IMPORTED)
  set_target_properties(Cgreen::runner PROPERTIES
      IMPORTED_LOCATION "${CGREEN_RUNNER}"
  )
endif()

