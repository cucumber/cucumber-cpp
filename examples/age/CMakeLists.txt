project(ageCalculator)

if(TARGET GTest::GTest)
  add_executable(ageCalculator features/step_definitions/ageCalculator)
  target_link_libraries(ageCalculator PRIVATE cucumber-cpp GTest::GTest)
  add_test(NAME ageCalculatorTest COMMAND /home/rcl/cucumber-cpp/ageTest.sh)
endif()
