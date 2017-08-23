find_package(Boost 1.34 REQUIRED)

add_library(json_spirit.header INTERFACE)
target_sources(json_spirit.header INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/json_spirit.h
    ${CMAKE_CURRENT_SOURCE_DIR}/json_spirit_error_position.h
    ${CMAKE_CURRENT_SOURCE_DIR}/json_spirit_reader_template.h
    ${CMAKE_CURRENT_SOURCE_DIR}/json_spirit_stream_reader.h
    ${CMAKE_CURRENT_SOURCE_DIR}/json_spirit_utils.h
    ${CMAKE_CURRENT_SOURCE_DIR}/json_spirit_value.h
    ${CMAKE_CURRENT_SOURCE_DIR}/json_spirit_writer_template.h
)
target_include_directories(json_spirit.header SYSTEM
    INTERFACE
        ${Boost_INCLUDE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/..
)

add_library(json_spirit STATIC EXCLUDE_FROM_ALL
    ${CMAKE_CURRENT_SOURCE_DIR}/json_spirit_reader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/json_spirit_reader.h
    ${CMAKE_CURRENT_SOURCE_DIR}/json_spirit_value.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/json_spirit_writer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/json_spirit_writer.h
)
target_link_libraries(json_spirit PUBLIC json_spirit.header)
