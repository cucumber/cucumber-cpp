project(Calc)

add_library(Calc STATIC src/Calculator)
target_include_directories(Calc INTERFACE src)

if(TARGET GTest::GTest)
    add_executable(GTestCalculatorSteps features/step_definitions/GTestCalculatorSteps)
    target_link_libraries(GTestCalculatorSteps PRIVATE Calc cucumber-cpp GTest::GTest)

    list(FIND CMAKE_CXX_COMPILE_FEATURES cxx_variadic_templates HAS_VARIADIC_TEMPLATES)
    if(HAS_VARIADIC_TEMPLATES GREATER -1)
        add_executable(FuncArgsCalculatorSteps features/step_definitions/FuncArgsCalculatorSteps)
        target_link_libraries(FuncArgsCalculatorSteps PRIVATE Calc cucumber-cpp GTest::GTest)

        target_compile_features(FuncArgsCalculatorSteps PRIVATE cxx_variadic_templates)
    endif()
endif()

if(TARGET Boost::unit_test_framework)
    add_executable(BoostCalculatorSteps features/step_definitions/BoostCalculatorSteps)
    target_link_libraries(BoostCalculatorSteps PRIVATE Calc cucumber-cpp Boost::unit_test_framework)
endif()

if(TARGET Qt::Test)
    add_executable(QtTestCalculatorSteps features/step_definitions/QtTestCalculatorSteps)
    target_link_libraries(QtTestCalculatorSteps Calc Qt::Test cucumber-cpp)
endif()
