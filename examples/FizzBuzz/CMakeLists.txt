project(FizzBuzz)

include_directories(${CUKE_INCLUDE_DIRS} src)

add_library(FizzBuzz src/FizzBuzzReporter)

if(CGREEN_FOUND)
    add_executable(FizzBuzzSteps features/step_definitions/FizzBuzzSteps)
    target_link_libraries(FizzBuzzSteps FizzBuzz ${CUKE_LIBRARIES} Cgreen::Cgreen)
endif()
