cmake_minimum_required(VERSION 3.20)
project(TemplatedMatrix VERSION 1.0 LANGUAGES CXX)

# --- Global Settings ---
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Use Ninja and Clang if available
if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_compile_options(-Wall -Wextra -Wpedantic -Wshadow -Wconversion)
endif()

# --- Tooling: Clang-Format ---
file(GLOB_RECURSE ALL_SOURCE_FILES
    "include/*.hpp"
)
find_program(CLANG_FORMAT "clang-format")
if(CLANG_FORMAT)
    add_custom_target(format
        COMMAND ${CLANG_FORMAT} -i -style=GNU ${ALL_SOURCE_FILES}
        COMMENT "Applying GNU code style with clang-format"
    )
endif()

# --- Tooling: Clang-Tidy ---
find_program(CLANG_TIDY "clang-tidy")
if(CLANG_TIDY)
    set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY};-checks=-*,readability-*,bugprone-*,modernize-*,cppcoreguidelines-*;--header-filter=.*")
endif()

# --- Targets ---

# 1. Core Library (Header-only but defined as interface for consistency)
add_library(MatrixCore INTERFACE)
target_include_directories(MatrixCore INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
