cmake_minimum_required(VERSION 3.16)

project(frame C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Collect all .c files in src/
# Note: without CONFIGURE_DEPENDS, adding new .c files after the initial
# configure won't be picked up until you re-run CMake manually.
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/src/*.c"
)

# Create executable
add_executable(frame ${SOURCES})

# Include headers from src/
target_include_directories(frame PRIVATE
    "${CMAKE_SOURCE_DIR}/src"
)

# Optional: better warnings (recommended)
target_compile_options(frame PRIVATE
    -Wall -Wextra -Wpedantic
)
