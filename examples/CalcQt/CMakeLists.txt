project(CalcQt)

set(CALCQT_HEADERS )

if(QT_LIBRARIES)
    add_library(libcalcqt src/CalculatorWidget.cpp)
    target_link_libraries(libcalcqt ${QT_LIBRARIES})
    
    add_executable(calcqt src/CalcQt.cpp src/CalculatorWidget.h)
    target_link_libraries(calcqt libcalcqt ${QT_LIBRARIES})

    if(Boost_UNIT_TEST_FRAMEWORK_FOUND)
        include_directories(${Boost_INCLUDE_DIRS})
        add_executable(BoostCalculatorQtSteps features/step_definitions/BoostCalculatorQtSteps)
        target_link_libraries(BoostCalculatorQtSteps libcalcqt ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} ${CUKE_LIBRARIES} ${QT_LIBRARIES})
    endif()
    if(GTEST_FOUND)
        add_executable(GTestCalculatorQtSteps features/step_definitions/GTestCalculatorQtSteps)
        target_link_libraries(GTestCalculatorQtSteps libcalcqt ${CUKE_LIBRARIES} ${CUKE_GTEST_LIBRARIES} ${QT_LIBRARIES})
    endif()
endif()
