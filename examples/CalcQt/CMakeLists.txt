cmake_minimum_required(VERSION 2.8.11)

project(CalcQt)

# Find the Core library
find_package(Qt5Core REQUIRED)
# Find the QtWidgets library
find_package(Qt5Widgets REQUIRED)
# Find the QtTest library
find_package(Qt5Test REQUIRED)

set (QT5_FOUND ${Qt5Core_FOUND} AND ${Qt5Test_FOUND} AND ${Qt5Widgets_FOUND})

if(NOT QT5_FOUND)
    message(WARNING "Qt5 not found")
endif()

if(QT5_FOUND)
    message(STATUS "Using Qt version ${Qt5Core_VERSION_STRING} ")

    # Find includes in corresponding build directories
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
    # Instruct CMake to run moc automatically when needed.
    set(CMAKE_AUTOMOC ON)
    
    set(CALCQT_HEADERS src/CalculatorWidget.h)
    set(CALCQT_SOURCES src/CalcQt.cpp src/CalculatorWidget.cpp)
    set(QT_LIBRARIES Qt5::Core Qt5::Widgets Qt5::Test)  

    add_library(libcalcqt src/CalculatorWidget.cpp ${CALCQT_HEADERS})
    target_link_libraries(libcalcqt ${QT_LIBRARIES})
    
    add_executable(calcqt ${CALCQT_SOURCES} ${CALCQT_HEADERS})
    target_link_libraries(calcqt libcalcqt ${QT_LIBRARIES})
    
    if(Boost_UNIT_TEST_FRAMEWORK_FOUND)
        include_directories(${Boost_INCLUDE_DIRS} src/)
        add_executable(BoostCalculatorQtSteps features/step_definitions/BoostCalculatorQtSteps ${CALCQT_HEADERS})
        target_link_libraries(BoostCalculatorQtSteps libcalcqt ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} ${CUKE_LIBRARIES} ${QT_LIBRARIES})
    endif()

    if(GTEST_FOUND)
        include_directories(${GTEST_INCLUDE_DIRS})
        add_executable(GTestCalculatorQtSteps features/step_definitions/GTestCalculatorQtSteps)
        target_link_libraries(GTestCalculatorQtSteps libcalcqt ${QT_LIBRARIES} ${GTEST_LIBRARIES} ${CUKE_LIBRARIES})
    endif()

endif()

