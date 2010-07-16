if(GTEST_FOUND)

    include_directories(${GTEST_INCLUDE_DIRS})

    function(cuke_add_test TEST_FILE)
        get_filename_component(TEST_NAME ${TEST_FILE} NAME)
        message(STATUS "Adding " ${TEST_NAME})
        add_executable(${TEST_NAME} ${TEST_FILE}.cpp)
        target_link_libraries(${TEST_NAME} ${GTEST_BOTH_LIBRARIES} ${Boost_LIBRARIES} ${ARGN})
        gtest_add_tests(${TEST_NAME} "" ${TEST_FILE}.cpp)
    endfunction()

    function(cuke_add_driver_test TEST_FILE)
        get_filename_component(TEST_NAME ${TEST_FILE} NAME)
        message(STATUS "Adding " ${TEST_NAME})
        add_executable(${TEST_NAME} ${TEST_FILE}.cpp)
        target_link_libraries(${TEST_NAME} ${GTEST_LIBRARIES} ${Boost_LIBRARIES} ${ARGN})
    endfunction()

    cuke_add_test(integration/ContextHandlingTest)
    cuke_add_test(integration/StepRegistrationTest)
    cuke_add_test(integration/WireServerTest)
    cuke_add_test(unit/ContextManagerTest)
    cuke_add_test(unit/CukeCommandsTest)
    cuke_add_test(unit/StepManagerTest)

    cuke_add_driver_test(integration/GTestDriverTest)
endif ()
