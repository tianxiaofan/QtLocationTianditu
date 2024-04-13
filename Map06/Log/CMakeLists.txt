
project(tilogger)

# find_package (Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Quick)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(${PROJECT_NAME})

target_sources(${PROJECT_NAME}
    PRIVATE
        tilogger.cpp
        tilogger.h
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        Qt${QT_VERSION_MAJOR}::Core
)

# interface 谁依赖本项目，就会自动包含本项目目录
target_include_directories(${PROJECT_NAME}
    INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)



