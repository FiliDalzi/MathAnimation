cmake_minimum_required(VERSION 3.21)

project(MathAnimations VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

# Detect macOS
if(APPLE)
    message(STATUS "macOS detected: building minimal version")
    set(MACOS_MINIMAL TRUE)
endif()

# Output folders
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Include main module
add_subdirectory(Animations)
