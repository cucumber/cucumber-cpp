project(FizzBuzz)

include_directories(${CUKE_INCLUDE_DIRS} src)

add_library(FizzBuzz src/FizzBuzzReporter)

if(cgreen_FOUND)
    include_directories(${CGREEN_INCLUDE_DIRS})
    add_executable(FizzBuzzSteps features/step_definitions/FizzBuzzSteps)
    target_link_libraries(FizzBuzzSteps FizzBuzz ${CUKE_LIBRARIES} ${CGREEN_LIBRARIES})
else()
    message("cgeen package required for this example")
endif()
