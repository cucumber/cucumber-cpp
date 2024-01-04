project(CalcQt)

if(TARGET Qt::Core AND TARGET Qt::Gui AND TARGET Qt::Widgets AND TARGET Qt::Test)
    set(CMAKE_AUTOMOC ON)

    add_library(libcalcqt STATIC src/Calculator.cpp src/Calculator.hpp)
    target_include_directories(libcalcqt INTERFACE src)
    target_link_libraries(libcalcqt
        PUBLIC
            Qt::Core
    )

    add_executable(calcqt
        src/CalcQt.cpp
        src/CalculatorWidget.cpp
        src/CalculatorWidget.hpp
    )
    target_link_libraries(calcqt
        PRIVATE
            libcalcqt
            Qt::Core
            Qt::Gui
            Qt::Widgets
    )

    add_executable(QtTestCalculatorQtSteps features/step_definitions/QtTestCalculatorQtSteps.cpp)
    target_link_libraries(QtTestCalculatorQtSteps PRIVATE libcalcqt Qt::Test cucumber-cpp)

    if(TARGET Boost::unit_test_framework)
        add_executable(BoostCalculatorQtSteps features/step_definitions/BoostCalculatorQtSteps.cpp)
        target_link_libraries(BoostCalculatorQtSteps PRIVATE libcalcqt Boost::unit_test_framework cucumber-cpp)
    endif()

    if(TARGET GTest::gtest)
        add_executable(GTestCalculatorQtSteps features/step_definitions/GTestCalculatorQtSteps.cpp)
        target_link_libraries(GTestCalculatorQtSteps PRIVATE libcalcqt GTest::gtest cucumber-cpp)
    endif()

endif()
