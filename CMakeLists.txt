cmake_minimum_required(VERSION 3.21)

project(
    MathAnimations
    VERSION 1.0
    LANGUAGES C CXX
)

# Detect OS
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(MATH_ANIMATION_OS_LINUX TRUE)
endif()

if(WIN32)
    set(MATH_ANIMATION_OS_WINDOWS TRUE)
endif()

if(APPLE)
    set(MATH_ANIMATION_OS_OSX TRUE)
    message(STATUS "macOS detected: building minimal app (excluding AV1, SSE, Freetype)")
    set(MACOS_MINIMAL TRUE)
endif()

# Compiler settings
cmake_policy(SET CMP0077 NEW)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Build configuration types
set(CMAKE_CONFIGURATION_TYPES Debug Release RelWithProfiler DebugTests RelTests CACHE STRING "" FORCE)

# Output directories
foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG_UPPER)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG_UPPER} ${CMAKE_CURRENT_BINARY_DIR}/bin/${OUTPUTCONFIG}/lib)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG_UPPER} ${CMAKE_CURRENT_BINARY_DIR}/bin/${OUTPUTCONFIG}/lib)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG_UPPER} ${CMAKE_CURRENT_BINARY_DIR}/bin/${OUTPUTCONFIG})
endforeach()

# Exclude AV1 and Freetype on macOS
if(NOT MACOS_MINIMAL)
    add_subdirectory(Animations/vendor/av1)
    add_subdirectory(Animations/vendor/freetype)
endif()

# Include main animation module
add_subdirectory(Animations)

# macOS minimal build (no SSE / Windows / AV1 / Freetype)
if(MACOS_MINIMAL)
    file(GLOB_RECURSE SRC_FILES
        "Animations/src/animation/*.cpp"
        # Escludi file problematici SSE/Windows-only
        "Animations/src/animation/Axis.cpp"
        "Animations/src/animation/AnimationManager.cpp"
        "Animations/src/animation/Animation.cpp"
    )
    add_executable(MathAnimApp ${SRC_FILES})
endif()

# IDE settings (Visual Studio)
set_property(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    PROPERTY VS_STARTUP_PROJECT MathAnimations
)
