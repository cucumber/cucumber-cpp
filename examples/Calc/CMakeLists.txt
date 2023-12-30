project(Calc)

add_library(Calc STATIC src/Calculator.cpp)
target_include_directories(Calc INTERFACE src)

if(TARGET GTest::gtest)
    add_executable(GTestCalculatorSteps features/step_definitions/GTestCalculatorSteps.cpp)
    target_link_libraries(GTestCalculatorSteps PRIVATE Calc cucumber-cpp GTest::gtest)

    add_executable(FuncArgsCalculatorSteps features/step_definitions/FuncArgsCalculatorSteps.cpp)
    target_link_libraries(FuncArgsCalculatorSteps PRIVATE Calc cucumber-cpp GTest::gtest)
endif()

if(TARGET Boost::unit_test_framework)
    add_executable(BoostCalculatorSteps features/step_definitions/BoostCalculatorSteps.cpp)
    target_link_libraries(BoostCalculatorSteps PRIVATE Calc cucumber-cpp Boost::unit_test_framework)
endif()

if(TARGET Qt::Test)
    add_executable(QtTestCalculatorSteps features/step_definitions/QtTestCalculatorSteps.cpp)
    target_link_libraries(QtTestCalculatorSteps Calc Qt::Test cucumber-cpp)
endif()
