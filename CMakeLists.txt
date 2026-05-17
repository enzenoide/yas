cmake_minimum_required(VERSION 3.10)
project(yas LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(${PROJECT_SOURCE_DIR}/include)

set(SOURCE_FILES
    src/main.cpp
    src/ast.cpp
    src/lexer.cpp
    src/parser.cpp
    src/transpiler.cpp
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
