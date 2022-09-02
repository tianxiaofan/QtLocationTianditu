cmake_minimum_required(VERSION 3.16)
project(map06 VERSION 1.0 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)


# Set up AUTOMOC and some sensible defaults for runtime execution
# When using Qt 6.3, you can replace the code block below with
# qt_standard_project_setup()

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
include(GNUInstallDirs)

add_executable(${PROJECT_NAME})
#include(compiler_conf)

#if(WIN32)
#    add_compile_options(/W4)
#    add_compile_options(/wd4819)
#endif()

add_definitions(TiQeoServiceProviderFactory)

find_package(QT NAMES Qt5 Qt6 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Concurrent Gui Location Positioning Quick Sql Widgets)
find_package(Qt${QT_VERSION_MAJOR} OPTIONAL_COMPONENTS Multimedia MultimediaWidgets Network OpenGL Qml SerialPort)

add_subdirectory(TiMap)

target_sources(${PROJECT_NAME}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qml.qrc
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    TiMap
    Qt::Core
    Qt5::Location
    Qt5::PositioningPrivate
    Qt5::PositioningQuick
    Qt5::Positioning
    Qt5::Sql
    Qt5::Quick
    Qt5::OpenGL
)

