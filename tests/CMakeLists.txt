function(cuke_add_copy_libs_command TEST_NAME)
    foreach(CUKE_BOOST_TEST_LIB in ${CUKE_BOOST_TEST_LIBS})
        if(EXISTS ${CUKE_BOOST_TEST_LIB})
            get_filename_component(CUKE_BOOST_TEST_LIB_DIR ${CUKE_BOOST_TEST_LIB} DIRECTORY)
            get_filename_component(CUKE_BOOST_TEST_LIB_COMPONENT ${CUKE_BOOST_TEST_LIB} NAME_WE)
            set(CUKE_BOOST_TEST_LIB_SHARED "${CUKE_BOOST_TEST_LIB_DIR}/${CUKE_BOOST_TEST_LIB_COMPONENT}${CMAKE_SHARED_LIBRARY_SUFFIX}")
            add_custom_command(TARGET ${TEST_NAME} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    "${CUKE_BOOST_TEST_LIB_SHARED}"
                    "${CMAKE_CURRENT_BINARY_DIR}"
                    )
        endif()

        add_custom_command(TARGET ${TEST_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "$<TARGET_FILE:cucumber-cpp-nomain>"
                "${CMAKE_CURRENT_BINARY_DIR}"
                )
    endforeach()
endfunction()

function(cuke_add_driver_test TEST_FILE)
    get_filename_component(TEST_NAME ${TEST_FILE} NAME)
    message(STATUS "Adding " ${TEST_NAME})
    add_executable(${TEST_NAME} ${TEST_FILE}.cpp)
    target_link_libraries(${TEST_NAME} cucumber-cpp-nomain ${CUKE_EXTRA_LIBRARIES} ${ARGN})
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
    if(CUKE_ENABLE_SHARED_LIB)
        cuke_add_copy_libs_command(${TEST_NAME})
    endif()
endfunction()

if(GMOCK_FOUND)
    function(cuke_add_test TEST_FILE)
        get_filename_component(TEST_NAME ${TEST_FILE} NAME)
        message(STATUS "Adding " ${TEST_NAME})
        add_executable(${TEST_NAME} ${TEST_FILE}.cpp)
        target_link_libraries(${TEST_NAME} cucumber-cpp-nomain ${CUKE_EXTRA_LIBRARIES} ${ARGN} ${CUKE_GMOCK_LIBRARIES})
        gtest_add_tests(${TEST_NAME} "" ${TEST_FILE}.cpp)
        # Run all tests in executable at once too. This ensures that the used fixtures get tested properly too.
        add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
        if(CUKE_ENABLE_SHARED_LIB)
            cuke_add_copy_libs_command(${TEST_NAME})
        endif()
    endfunction()

    # TODO Compile tests with the least possible code, not with the entire library
    cuke_add_test(integration/ContextHandlingTest)
    cuke_add_test(integration/HookRegistrationTest)
    cuke_add_test(integration/StepRegistrationTest)
    cuke_add_test(integration/TaggedHookRegistrationTest)
    if(NOT WIN32)
        cuke_add_test(integration/WireServerTest)
    endif()
    cuke_add_test(integration/WireProtocolTest)
    cuke_add_test(unit/BasicStepTest)
    cuke_add_test(unit/ContextManagerTest)
    cuke_add_test(unit/CukeCommandsTest)
    cuke_add_test(unit/RegexTest)
    cuke_add_test(unit/StepCallChainTest)
    cuke_add_test(unit/StepManagerTest)
    cuke_add_test(unit/TableTest)
    cuke_add_test(unit/TagTest)

    cuke_add_driver_test(integration/drivers/GTestDriverTest ${CUKE_GTEST_LIBRARIES})
endif()

if(Boost_UNIT_TEST_FRAMEWORK_FOUND)
    cuke_add_driver_test(integration/drivers/BoostDriverTest ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
endif()

cuke_add_driver_test(integration/drivers/GenericDriverTest)
