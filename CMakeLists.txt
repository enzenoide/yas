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

# Make tests runner executable at configure time (no need to commit exec bit)
execute_process(COMMAND ${CMAKE_COMMAND} -E chmod +x ${CMAKE_SOURCE_DIR}/tests/run_tests.sh
                                RESULT_VARIABLE _chmod_result
                                OUTPUT_QUIET ERROR_QUIET)

# Custom target to run tests after building the `yas` executable
add_custom_target(run_tests
    DEPENDS ${PROJECT_NAME}
    COMMAND ${CMAKE_COMMAND} -E echo "Running tests with ${CMAKE_SOURCE_DIR}/tests/run_tests.sh via bash"
    COMMAND bash ${CMAKE_SOURCE_DIR}/tests/run_tests.sh
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/tests
)
