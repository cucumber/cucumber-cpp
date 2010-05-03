project(Calc)

include_directories(${CUKE_INCLUDE_DIRS} ${GTEST_INCLUDE_DIRS} Calc)

add_library(Calc Calc/Calculator)

add_executable(CalculatorSteps CalcFeatures/CalculatorSteps)
target_link_libraries(CalculatorSteps Calc ${GTEST_LIBRARIES} ${Boost_LIBRARIES})
