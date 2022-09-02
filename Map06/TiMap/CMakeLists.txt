cmake_minimum_required(VERSION 3.16)
project(TiMap VERSION 1.0 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Set up AUTOMOC and some sensible defaults for runtime execution
# When using Qt 6.3, you can replace the code block below with
# qt_standard_project_setup()

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

add_library(${PROJECT_NAME})

aux_source_directory(./ src_list)
include_directories(./)

target_sources(${PROJECT_NAME} PRIVATE ${src_list})

target_link_libraries(${PROJECT_NAME}
    PUBLIC
    Qt5::Location
    Qt5::LocationPrivate
    Qt5::PositioningPrivate
    Qt5::Sql
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

