project(Calc)

include_directories(${CUKE_INCLUDE_DIRS} src)

add_library(Calc src/Calculator)

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
    include_directories(${Boost_INCLUDE_DIRS})
    add_executable(BoostCalculatorSteps features/step_definitions/BoostCalculatorSteps)
    target_link_libraries(BoostCalculatorSteps Calc ${CUKE_LIBRARIES} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
endif()
