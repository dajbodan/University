cmake_minimum_required(VERSION 3.15)

project(minipascal-llvm-compiler
        LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Find LLVM -------------------------------------------------------------
# On many systems you need to set LLVM_DIR first, e.g.:
#   - macOS (brew):  LLVM_DIR=/opt/homebrew/opt/llvm/lib/cmake/llvm
#   - Linux (apt):   LLVM_DIR=/usr/lib/llvm-16/lib/cmake/llvm
#
# Then run: cmake -DLLVM_DIR=/path/to/llvm/cmake ..

find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include_directories(SYSTEM ${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

# If you want to link all LLVM components in one go:
# llvm_map_components_to_libnames(LLVM_LIBS core support)

# --- Sources ---------------------------------------------------------------

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(SOURCES
    ${SRC_DIR}/main.cpp
    ${SRC_DIR}/Lexer.cpp
    ${SRC_DIR}/ast_gen.cpp
    ${SRC_DIR}/Parser.cpp
    ${SRC_DIR}/ast.cpp
    ${SRC_DIR}/fce.c
)

add_executable(minipascal ${SOURCES})

# Modern LLVM recommends linking against LLVM directly:
target_link_libraries(minipascal PRIVATE LLVM)

# Or, if LLVM complains, you can instead do:
# llvm_map_components_to_libnames(LLVM_LIBS core support)
# target_link_libraries(minipascal PRIVATE ${LLVM_LIBS})

