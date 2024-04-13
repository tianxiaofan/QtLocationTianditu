
project(tiMap)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(Qt6 6.5 REQUIRED COMPONENTS Quick Location Positioning Sql Network Core5Compat)

#遍历所有Cpp文件,不包含子目录，若要包含子目录，使用GLOB_RECURSE
file(GLOB_RECURSE CPP_FILES *.cpp *.h)
foreach(filepath ${CPP_FILES})
    string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/" "" filename ${filepath})
    list(APPEND sources_files ${filename})
endforeach(filepath)


qt_add_plugin(${PROJECT_NAME}
    STATIC
    CLASS_NAME TiQeoServiceProviderFactory
    ${sources_files}
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    tilogger
    Qt6::Core5Compat
    Qt6::Core
    Qt6::Location
    Qt6::LocationPrivate
    Qt6::PositioningPrivate
    Qt6::Sql
    Qt6::Network
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

