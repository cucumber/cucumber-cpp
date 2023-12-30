project(FeatureShowcase)

if(TARGET GTest::gtest)
    function(add_cucumber_executable)
        add_executable(FeatureShowcaseSteps ${ARGV})
        target_link_libraries(FeatureShowcaseSteps PRIVATE cucumber-cpp GTest::gtest)
        foreach(_arg ${ARGN})
            get_filename_component(OBJECT_PREFIX ${_arg} NAME_WE)
            set_source_files_properties(${_arg} PROPERTIES COMPILE_FLAGS "-DCUKE_OBJECT_PREFIX=${OBJECT_PREFIX}")
        endforeach(_arg)
    endfunction()

    add_cucumber_executable(
        features/step_definitions/TagSteps.cpp
        features/step_definitions/TableSteps.cpp
    )
endif()

