project(Calc)

include_directories(${CUKE_INCLUDE_DIRS} src)

add_library(Calc src/Calculator)

if(GMOCK_FOUND)
    add_executable(GTestCalculatorSteps features/step_definitions/GTestCalculatorSteps)
    target_link_libraries(GTestCalculatorSteps Calc ${CUKE_LIBRARIES} ${CUKE_GTEST_LIBRARIES})
endif()

if(Boost_UNIT_TEST_FRAMEWORK_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    add_executable(BoostCalculatorSteps features/step_definitions/BoostCalculatorSteps)
    target_link_libraries(BoostCalculatorSteps Calc ${CUKE_LIBRARIES} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
endif()

if(${Qt5Core_FOUND} AND ${Qt5Test_FOUND})
    include_directories(${Qt5Core_INCLUDE_DIRS})
    add_executable(QtTestCalculatorSteps features/step_definitions/QtTestCalculatorSteps)
    target_link_libraries(QtTestCalculatorSteps Calc Qt5::Core Qt5::Test ${CUKE_LIBRARIES})
endif()
