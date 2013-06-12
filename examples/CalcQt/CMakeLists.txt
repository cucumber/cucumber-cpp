project(CalcQt)

find_package(Qt4 COMPONENTS QtCore QtGui QtTest)

if(QT4_FOUND)
    include(${QT_USE_FILE})
    set(CALCQT_HEADERS src/CalculatorWidget.h)
    qt4_wrap_cpp(CALCQT_HEADERS_MOC ${CALCQT_HEADERS})
    include_directories(${CUKE_INCLUDE_DIRS} src)
    add_library(CalcQt src/CalculatorWidget ${CALCQT_HEADERS_MOC})

    if(Boost_UNIT_TEST_FRAMEWORK_FOUND)
        include_directories(${Boost_INCLUDE_DIRS})
        add_executable(BoostCalculatorQtSteps features/step_definitions/BoostCalculatorQtSteps)
        target_link_libraries(BoostCalculatorQtSteps CalcQt ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} ${CUKE_LIBRARIES} ${QT_LIBRARIES})
    endif()

    set(CALCQT_SOURCES src/CalcQt.cpp src/CalculatorWidget.cpp)
    add_executable(calcqt ${CALCQT_SOURCES} ${CALCQT_HEADERS_MOC})
    target_link_libraries(calcqt ${QT_LIBRARIES})
endif()
