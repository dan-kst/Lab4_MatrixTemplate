cmake_minimum_required(VERSION 3.20)
project(TemplatedMatrix VERSION 1.0 LANGUAGES CXX)

# --- Global Settings ---
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Use Ninja and Clang if available
if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_compile_options(-Wall -Wextra -Wpedantic -Wshadow -Wconversion)
endif()

# --- Tooling: Clang-Format ---
file(GLOB_RECURSE ALL_SOURCE_FILES
    "include/core/*.hpp"
)

# --- Tooling: Clang-Tidy ---
find_program(CLANG_TIDY "clang-tidy")
if(CLANG_TIDY)
    set(CLANG_TIDY_COMMAND
        "${CLANG_TIDY}"
        "--header-filter=${CMAKE_CURRENT_SOURCE_DIR}/include/.*"
    )
endif()

# --- Tooling: Clang-Format ---
find_program(CLANG_FORMAT "clang-format")
if(CLANG_FORMAT)
  set(CLANG_TIDY_COMMAND
    "${CLANG_FORMAT}"
    "-i"
    "--style=file"
    "--fallback-style=GNU"
    "--verbose"
    "${ALL_SOURCE_FILES}"
  )
endif()

# --- Targets ---
add_library(MatrixCore INTERFACE)
target_include_directories(MatrixCore INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# --- Include Directories ---
include_directories(include)
