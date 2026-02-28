cmake_minimum_required(VERSION 3.21)
project(MathAnimations VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

set(SRC_FILES
    src/animation/Animation.cpp
    src/animation/Axis.cpp
    src/animation/AnimationManager.cpp
)

# Compila SSE solo se la cartella e i file esistono
if(NOT DEFINED DISABLE_SSE)
    if(EXISTS "${CMAKE_CURRENT_LIST_DIR}/src/animation/SSEFile1.cpp")
        list(APPEND SRC_FILES
            src/animation/SSEFile1.cpp
            src/animation/SSEFile2.cpp
        )
    else()
        message(STATUS "SSE source files not found - skipping SSE compilation")
    endif()
endif()

# Crea la libreria
add_library(MathAnimations ${SRC_FILES})
target_include_directories(MathAnimations PUBLIC include)

# Detect OS
if(APPLE)
    if(CMAKE_SYSTEM_PROCESSOR STREQUAL "arm64")
        message(STATUS "Apple Silicon detected - disabling SSE")
        add_compile_definitions(DISABLE_SSE)
    else()
        message(STATUS "Intel Mac detected - enabling SSE")
        add_compile_options(-msse2 -mmmx)
    endif()
endif()

if(WIN32)
    set(MATH_ANIMATION_OS_WINDOWS TRUE)
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(MATH_ANIMATION_OS_LINUX TRUE)
endif()

# Aggiungi subdirectory dei sorgenti
add_subdirectory(Animations)
