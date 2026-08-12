function(git_get_version VERSION_VARIABLE)
    string(TIMESTAMP TODAY "%Y.%m.%d")
    set(${VERSION_VARIABLE} "0.0.0+${TODAY}" PARENT_SCOPE)

    find_program(GIT_EXECUTABLE git)

    if(NOT GIT_EXECUTABLE)
        message(WARNING "Git not found, using fallback version.")
        return()
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
        message(INFO "Error running 'git describe': ${GIT_DESCRIBE_ERROR}")
        message(WARNING "Git failed, using fallback version.")
        return()
    endif()

    string(LENGTH "${VERSION_STRING}" VERSION_STRING_LENGTH)
    string(SUBSTRING "${VERSION_STRING}" 0 1 FIRST_CHARACTER)

    if("${FIRST_CHARACTER}" STREQUAL "v")
        string(SUBSTRING "${VERSION_STRING}" 1 ${VERSION_STRING_LENGTH} VERSION_STRING)
    endif()

    set(${VERSION_VARIABLE} ${VERSION_STRING} PARENT_SCOPE)
endfunction()
