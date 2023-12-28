function(git_get_version VERSION_VARIABLE)
    find_program(GIT_EXECUTABLE git)

    if(NOT GIT_EXECUTABLE)
        message(FATAL_ERROR "Git not found. Please install Git and make sure it is in your system's PATH.")
    endif()

    execute_process(
        COMMAND ${GIT_EXECUTABLE} describe --always --dirty
        OUTPUT_VARIABLE VERSION_STRING
        OUTPUT_STRIP_TRAILING_WHITESPACE
        WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
        ERROR_VARIABLE GIT_DESCRIBE_ERROR
        RESULT_VARIABLE GIT_DESCRIBE_RESULT
    )

    if(NOT GIT_DESCRIBE_RESULT EQUAL 0)
        message(FATAL_ERROR "Error running 'git describe': ${GIT_DESCRIBE_ERROR}")
    endif()

    string(LENGTH "${VERSION_STRING}" VERSION_STRING_LENGTH)
    string(SUBSTRING "${VERSION_STRING}" 0 1 FIRST_CHARACTER)

    if("${FIRST_CHARACTER}" STREQUAL "v")
        string(SUBSTRING "${VERSION_STRING}" 1 ${VERSION_STRING_LENGTH} VERSION_STRING)
    endif()

    set(${VERSION_VARIABLE} ${VERSION_STRING} PARENT_SCOPE)
endfunction()
