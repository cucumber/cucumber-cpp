find_path(ASIO_INCLUDE_DIR asio.hpp)

if (ASIO_INCLUDE_DIR)
    set(ASIO_FOUND TRUE)
else ()
    set(ASIO_FOUND FALSE)
endif ()

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Asio REQUIRED_VARS ASIO_INCLUDE_DIR)
