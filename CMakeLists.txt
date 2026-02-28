cmake_minimum_required(VERSION 3.21)

# Rileva l'architettura del Mac
if(APPLE)
    if(CMAKE_OSX_ARCHITECTURES STREQUAL "" OR CMAKE_OSX_ARCHITECTURES MATCHES "arm64")
        # su Apple Silicon
        message(STATUS "Compilazione per ARM64 (Apple Silicon)")
    else()
        # su Intel
        message(STATUS "Compilazione per x86_64 (Intel Mac)")
        set(CMAKE_OSX_ARCHITECTURES "x86_64")
        add_compile_options(-msse2 -mmmx)
    endif()
endif()

project(MathAnimations VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

# Detect OS
if(APPLE)
    set(MATH_ANIMATION_OS_OSX TRUE)

    # Detect Apple Silicon
    if(CMAKE_SYSTEM_PROCESSOR STREQUAL "arm64")
        message(STATUS "Apple Silicon detected - disabling SSE")
        add_compile_definitions(DISABLE_SSE)
    endif()
endif()

if(WIN32)
    set(MATH_ANIMATION_OS_WINDOWS TRUE)
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(MATH_ANIMATION_OS_LINUX TRUE)
endif()

add_subdirectory(Animations)
