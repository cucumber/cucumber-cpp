project(CalcQt)

if(QT_LIBRARIES)
    add_library(libcalcqt src/CalculatorWidget.cpp src/CalculatorWidget.h)
    set_target_properties(libcalcqt PROPERTIES AUTOMOC ON)
    target_include_directories(libcalcqt PUBLIC src)
    target_link_libraries(libcalcqt ${QT_LIBRARIES})

    add_executable(calcqt src/CalcQt.cpp)
    target_link_libraries(calcqt libcalcqt ${QT_LIBRARIES})

    if(Qt5TEST_FOUND)
        add_executable(QtTestCalculatorQtSteps features/step_definitions/QtTestCalculatorQtSteps)
        target_link_libraries(QtTestCalculatorQtSteps PRIVATE libcalcqt ${QT_LIBRARIES} ${CUKE_LIBRARIES})
    endif()

    if(Boost_UNIT_TEST_FRAMEWORK_FOUND)
        add_executable(BoostCalculatorQtSteps features/step_definitions/BoostCalculatorQtSteps)
        target_link_libraries(BoostCalculatorQtSteps libcalcqt ${CUKE_LIBRARIES} ${QT_LIBRARIES} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
    endif()

    if(GTEST_FOUND)
        add_executable(GTestCalculatorQtSteps features/step_definitions/GTestCalculatorQtSteps)
        target_link_libraries(GTestCalculatorQtSteps libcalcqt ${CUKE_LIBRARIES} ${CUKE_GTEST_LIBRARIES} ${QT_LIBRARIES})
    endif()

endif()
