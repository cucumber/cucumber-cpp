set(CUKEBINS_SOURCES
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
    include_directories(${GTEST_INCLUDE_DIRS})
    set(CUKEBINS_SOURCES ${CUKEBINS_SOURCES} drivers/GTestDriver.cpp)
endif()

if(CPPSPEC_FOUND)
    include_directories(${CPPSPEC_INCLUDE_DIRS})
    set(CUKEBINS_SOURCES ${CUKEBINS_SOURCES} drivers/CppSpecDriver.cpp)
endif()

if(Boost_UNIT_TEST_FRAMEWORK_FOUND)
    set(CUKEBINS_SOURCES ${CUKEBINS_SOURCES} drivers/BoostDriver.cpp)
endif()


add_library(CukeBins STATIC ${CUKEBINS_SOURCES})
