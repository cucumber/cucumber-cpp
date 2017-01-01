project(FeatureShowcase)

include_directories(${CUKE_INCLUDE_DIRS})

if(GMOCK_FOUND)
    function(add_cucumber_executable)
        add_executable(FeatureShowcaseSteps ${ARGV})
        target_link_libraries(FeatureShowcaseSteps ${CUKE_LIBRARIES} ${CUKE_GTEST_LIBRARIES})
        foreach(_arg ${ARGN})
            get_filename_component(OBJECT_PREFIX ${_arg} NAME_WE)
            set_source_files_properties(${_arg} PROPERTIES COMPILE_FLAGS "-DCUKE_OBJECT_PREFIX=${OBJECT_PREFIX}")
        endforeach(_arg)
    endfunction()

    add_cucumber_executable(
        features/step_definitions/TagSteps
        features/step_definitions/TableSteps
    )
endif()

