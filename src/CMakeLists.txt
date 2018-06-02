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
    list(APPEND CUKE_DEP_LIBRARIES ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
endif()

if(TARGET Qt5::Test)
    qt5_wrap_cpp(MOC_FILE ../include/cucumber-cpp/internal/drivers/QtTestDriver.hpp)
    list(APPEND CUKE_SOURCES ${MOC_FILE})
    list(APPEND CUKE_SOURCES drivers/QtTestDriver.cpp)
    list(APPEND CUKE_EXTRA_PRIVATE_LIBRARIES Qt5::Test)
endif()

if(CMAKE_EXTRA_GENERATOR OR MSVC_IDE)
    message(STATUS "Adding header files to project")
    file(GLOB_RECURSE CUKE_HEADERS "${CUKE_INCLUDE_DIR}/cucumber-cpp/*.hpp")
    if(MSVC_IDE)
        source_group("Header Files" FILES ${CUKE_HEADERS})
    endif()
    list(APPEND CUKE_SOURCES ${CUKE_HEADERS})
endif()

# Library for unit tests relying on internals
add_library(cucumber-cpp-internal STATIC ${CUKE_SOURCES})

add_library(cucumber-cpp-nomain ${CUKE_SOURCES})
add_library(cucumber-cpp ${CUKE_SOURCES} main.cpp)

set_target_properties(
    cucumber-cpp-internal
    cucumber-cpp
    cucumber-cpp-nomain
    PROPERTIES
        DEFINE_SYMBOL cucumber_cpp_EXPORTS
        CXX_VISIBILITY_PRESET hidden
        VISIBILITY_INLINES_HIDDEN ON
)

generate_export_header(cucumber-cpp
    EXPORT_FILE_NAME "cucumber-cpp/internal/CukeExport.hpp"
)

foreach(TARGET
        cucumber-cpp-internal
        cucumber-cpp-nomain
        cucumber-cpp
)
    target_include_directories(${TARGET}
        PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
            $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>
    )
    # Declaring json_spirit.header to be a direct dependency with target_link_libraries breaks installation
    target_include_directories(${TARGET} SYSTEM
        PRIVATE
            $<TARGET_PROPERTY:json_spirit.header,INTERFACE_INCLUDE_DIRECTORIES>
    )
    target_link_libraries(${TARGET}
        PUBLIC
            Boost::boost
            Boost::regex
        PRIVATE
            Boost::date_time
            Boost::filesystem
            Boost::thread
            ${CUKE_EXTRA_PRIVATE_LIBRARIES}
    )
    # Don't export or import symbols for statically linked libraries
    get_property(type TARGET ${TARGET} PROPERTY TYPE)
    if(NOT "${type}" MATCHES "^(SHARED|MODULE)_LIBRARY$")
        target_compile_definitions(${TARGET} PUBLIC CUCUMBER_CPP_STATIC_DEFINE)
    endif()
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

include(GNUInstallDirs)
install(DIRECTORY ${CUKE_INCLUDE_DIR}/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(
    FILES
        "${CMAKE_CURRENT_BINARY_DIR}/cucumber-cpp/internal/CukeExport.hpp"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/cucumber-cpp/internal"
)
install(
    TARGETS
        cucumber-cpp-nomain
        cucumber-cpp
    EXPORT   CucumberCpp
    ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR}
)
install(
    EXPORT      CucumberCpp
    NAMESPACE   CucumberCpp::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake
    FILE        CucumberCppConfig.cmake
)
