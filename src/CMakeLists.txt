include(GenerateExportHeader)

find_package(nlohmann_json 3.10.5 REQUIRED)
find_package(Asio REQUIRED)
find_package(TCLAP REQUIRED)
include(../cmake/modules/GitVersion.cmake)

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

if(TARGET GTest::gtest)
    list(APPEND CUKE_EXTRA_PRIVATE_LIBRARIES GTest::gtest)
    list(APPEND CUKE_SOURCES drivers/GTestDriver.cpp)
endif()

if(TARGET Boost::unit_test_framework)
    list(APPEND CUKE_EXTRA_PRIVATE_LIBRARIES Boost::unit_test_framework)
    list(APPEND CUKE_SOURCES drivers/BoostDriver.cpp)
    list(APPEND CUKE_DEP_LIBRARIES ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
endif()

if(TARGET Qt::Test)
    qt_wrap_cpp(MOC_FILE ../include/cucumber-cpp/internal/drivers/QtTestDriver.hpp)
    list(APPEND CUKE_SOURCES ${MOC_FILE})
    list(APPEND CUKE_SOURCES drivers/QtTestDriver.cpp)
    list(APPEND CUKE_EXTRA_PRIVATE_LIBRARIES Qt::Test)
endif()

message(STATUS "Adding header files to project")
set(CUKE_HEADERS
    ../include/cucumber-cpp/autodetect.hpp
    ../include/cucumber-cpp/defs.hpp
    ../include/cucumber-cpp/generic.hpp
    ../include/cucumber-cpp/internal/ContextManager.hpp
    ../include/cucumber-cpp/internal/CukeCommands.hpp
    ../include/cucumber-cpp/internal/CukeEngine.hpp
    ../include/cucumber-cpp/internal/CukeEngineImpl.hpp
    ../include/cucumber-cpp/internal/Macros.hpp
    ../include/cucumber-cpp/internal/RegistrationMacros.hpp
    ../include/cucumber-cpp/internal/Scenario.hpp
    ../include/cucumber-cpp/internal/Table.hpp
    ../include/cucumber-cpp/internal/connectors/wire/ProtocolHandler.hpp
    ../include/cucumber-cpp/internal/connectors/wire/WireProtocol.hpp
    ../include/cucumber-cpp/internal/connectors/wire/WireProtocolCommands.hpp
    ../include/cucumber-cpp/internal/connectors/wire/WireServer.hpp
    ../include/cucumber-cpp/internal/defs.hpp
    ../include/cucumber-cpp/internal/drivers/BoostDriver.hpp
    ../include/cucumber-cpp/internal/drivers/DriverSelector.hpp
    ../include/cucumber-cpp/internal/drivers/GTestDriver.hpp
    ../include/cucumber-cpp/internal/drivers/GenericDriver.hpp
    ../include/cucumber-cpp/internal/drivers/QtTestDriver.hpp
    ../include/cucumber-cpp/internal/hook/HookMacros.hpp
    ../include/cucumber-cpp/internal/hook/HookRegistrar.hpp
    ../include/cucumber-cpp/internal/hook/Tag.hpp
    ../include/cucumber-cpp/internal/step/StepMacros.hpp
    ../include/cucumber-cpp/internal/step/StepManager.hpp
    ../include/cucumber-cpp/internal/utils/IndexSequence.hpp
    ../include/cucumber-cpp/internal/utils/Regex.hpp
)
if(MSVC_IDE)
    source_group("Header Files" FILES ${CUKE_HEADERS})
endif()
list(APPEND CUKE_SOURCES ${CUKE_HEADERS})

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
            $<BUILD_INTERFACE:${CUKE_INCLUDE_DIR}>
            $<BUILD_INTERFACE:${ASIO_INCLUDE_DIR}>
            $<BUILD_INTERFACE:${TCLAP_INCLUDE_DIR}>
            $<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>
    )
    target_link_libraries(${TARGET}
        PRIVATE
            ${CUKE_EXTRA_PRIVATE_LIBRARIES}
            nlohmann_json::nlohmann_json
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

git_get_version(CUKE_VERSION)
message(STATUS "Version: ${CUKE_VERSION}")
target_compile_definitions(cucumber-cpp PRIVATE
    CUKE_VERSION="${CUKE_VERSION}"
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
