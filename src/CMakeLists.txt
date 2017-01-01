set(CUKE_SOURCES
    drivers/GenericDriver.cpp
    ContextManager.cpp
    CukeCommands.cpp
    CukeEngine.cpp
    CukeEngineImpl.cpp
    StepManager.cpp
    HookRegistrar.cpp
    Regex.cpp
    Scenario.cpp
    Table.cpp
    Tag.cpp
    connectors/wire/WireServer.cpp
    connectors/wire/WireProtocol.cpp
    connectors/wire/WireProtocolCommands.cpp
    )

if(GTEST_FOUND)
    list(APPEND CUKE_DEP_LIBRARIES GTest::GTest)
    list(APPEND CUKE_SOURCES drivers/GTestDriver.cpp)
endif()

if(Boost_UNIT_TEST_FRAMEWORK_FOUND)
    list(APPEND CUKE_SOURCES drivers/BoostDriver.cpp)
endif()

if(CMAKE_EXTRA_GENERATOR OR MSVC_IDE)
    message(STATUS "Adding header files to project")
    file(GLOB_RECURSE CUKE_HEADERS "${CUKE_INCLUDE_DIR}/cucumber-cpp/*.hpp")
    if(MSVC_IDE)
        source_group("Header Files" FILES ${CUKE_HEADERS})
    endif()
    list(APPEND CUKE_SOURCES ${CUKE_HEADERS})
endif()

add_library(cucumber-cpp-nomain STATIC ${CUKE_SOURCES})
add_library(cucumber-cpp STATIC ${CUKE_SOURCES} main.cpp)

if(MINGW)
    list(APPEND CUKE_DEP_LIBRARIES ws2_32)
endif(MINGW)

target_link_libraries(cucumber-cpp-nomain PRIVATE ${CUKE_DEP_LIBRARIES})
target_link_libraries(cucumber-cpp PRIVATE ${CUKE_DEP_LIBRARIES})
