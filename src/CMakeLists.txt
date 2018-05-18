include(GenerateExportHeader)

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

if(TARGET GTest::GTest)
    list(APPEND CUKE_EXTRA_PRIVATE_LIBRARIES GTest::GTest)
    list(APPEND CUKE_SOURCES drivers/GTestDriver.cpp)
endif()

if(TARGET Boost::unit_test_framework)
    list(APPEND CUKE_EXTRA_PRIVATE_LIBRARIES Boost::unit_test_framework)
    list(APPEND CUKE_SOURCES drivers/BoostDriver.cpp)
endif()

if(TARGET Qt::Test)
    qt5_wrap_cpp(MOC_FILE ../include/cucumber-cpp/internal/drivers/QtTestDriver.hpp)
    list(APPEND CUKE_SOURCES ${MOC_FILE})
    list(APPEND CUKE_SOURCES drivers/QtTestDriver.cpp)
    list(APPEND CUKE_EXTRA_PRIVATE_LIBRARIES Qt::Test)
endif()

if(CMAKE_EXTRA_GENERATOR OR MSVC_IDE)
    message(STATUS "Adding header files to project")
    file(GLOB_RECURSE CUKE_HEADERS "${CUKE_INCLUDE_DIR}/cucumber-cpp/*.hpp")
    if(MSVC_IDE)
        source_group("Header Files" FILES ${CUKE_HEADERS})
    endif()
    list(APPEND CUKE_SOURCES ${CUKE_HEADERS})
endif()

if(CUKE_ENABLE_SHARED_LIB)
    set(CUKE_LINKAGE SHARED)
else()
    set(CUKE_LINKAGE STATIC)
endif()

add_library(cucumber-cpp-nomain ${CUKE_LINKAGE} ${CUKE_SOURCES})
add_library(cucumber-cpp ${CUKE_LINKAGE} ${CUKE_SOURCES} main.cpp)

set_target_properties(cucumber-cpp cucumber-cpp-nomain PROPERTIES
    DEFINE_SYMBOL cucumber_cpp_EXPORTS
)

generate_export_header(cucumber-cpp
    EXPORT_FILE_NAME "cucumber-cpp/internal/CukeExport.hpp"
)

foreach(TARGET
        cucumber-cpp-nomain
        cucumber-cpp
)
    target_include_directories(${TARGET}
        PUBLIC
            ${CMAKE_CURRENT_BINARY_DIR}
            ${CMAKE_SOURCE_DIR}/include
    )
    target_link_libraries(${TARGET}
        PUBLIC
            Boost::boost
            Boost::regex
        PRIVATE
            Boost::date_time
            Boost::filesystem
            Boost::thread
            json_spirit.header
            ${CUKE_EXTRA_PRIVATE_LIBRARIES}
    )
    if(MINGW)
        target_link_libraries(${TARGET}
            PRIVATE
                ws2_32
        )
    endif(MINGW)
endforeach()

target_link_libraries(cucumber-cpp
    PRIVATE
        Boost::program_options
)
