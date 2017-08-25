project(Calc)

add_library(Calc src/Calculator)
target_include_directories(Calc PUBLIC src)

if(GMOCK_FOUND)
    add_executable(GTestCalculatorSteps features/step_definitions/GTestCalculatorSteps)
    target_link_libraries(GTestCalculatorSteps Calc ${CUKE_LIBRARIES} ${CUKE_GTEST_LIBRARIES})

    list(FIND CMAKE_CXX_COMPILE_FEATURES cxx_variadic_templates HAS_VARIADIC_TEMPLATES)
    if(HAS_VARIADIC_TEMPLATES GREATER -1)
        add_executable(FuncArgsCalculatorSteps features/step_definitions/FuncArgsCalculatorSteps)
        target_link_libraries(FuncArgsCalculatorSteps Calc ${CUKE_LIBRARIES} ${CUKE_GTEST_LIBRARIES})

        target_compile_features(FuncArgsCalculatorSteps PRIVATE cxx_variadic_templates)
    endif()
endif()

if(Boost_UNIT_TEST_FRAMEWORK_FOUND)
    add_executable(BoostCalculatorSteps features/step_definitions/BoostCalculatorSteps)
    target_link_libraries(BoostCalculatorSteps Calc ${CUKE_LIBRARIES} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
endif()

if(Qt5TEST_FOUND)
    add_executable(QtTestCalculatorSteps features/step_definitions/QtTestCalculatorSteps)
    target_link_libraries(QtTestCalculatorSteps Calc Qt5::Test ${CUKE_LIBRARIES})
endif()
