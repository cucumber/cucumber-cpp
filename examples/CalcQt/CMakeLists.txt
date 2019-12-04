project(CalcQt)

if(TARGET Qt::Core AND TARGET Qt::Gui AND TARGET Qt::Widgets AND TARGET Qt::Test)
    add_library(libcalcqt STATIC src/CalculatorWidget.cpp src/CalculatorWidget.h)
    target_compile_options(libcalcqt PRIVATE "-Wno-deprecated-declarations")
    set_target_properties(libcalcqt PROPERTIES AUTOMOC ON)
    target_include_directories(libcalcqt INTERFACE src)
    target_link_libraries(libcalcqt
        PUBLIC
            Qt::Core
            Qt::Widgets
        PRIVATE
            Qt::Gui
    )

    add_executable(calcqt src/CalcQt.cpp)
    target_link_libraries(calcqt PRIVATE libcalcqt Qt::Widgets)

    add_executable(QtTestCalculatorQtSteps features/step_definitions/QtTestCalculatorQtSteps)
    target_link_libraries(QtTestCalculatorQtSteps PRIVATE libcalcqt Qt::Test cucumber-cpp)

    if(TARGET Boost::unit_test_framework)
        add_executable(BoostCalculatorQtSteps features/step_definitions/BoostCalculatorQtSteps.cpp)
        target_link_libraries(BoostCalculatorQtSteps PRIVATE libcalcqt Boost::unit_test_framework cucumber-cpp Qt::Test)
    endif()

    if(TARGET GTest::GTest)
        add_executable(GTestCalculatorQtSteps features/step_definitions/GTestCalculatorQtSteps)
        target_link_libraries(GTestCalculatorQtSteps PRIVATE libcalcqt cucumber-cpp GTest::GTest Qt::Test)
    endif()

endif()
