cmake_minimum_required(VERSION 3.21)

project(MathAnimations VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

#################################
# Detect OS & Architecture
#################################

if(APPLE)
    if(CMAKE_SYSTEM_PROCESSOR STREQUAL "arm64")
        message(STATUS "Apple Silicon detected - disabling SSE")
        add_compile_definitions(DISABLE_SSE)
    else()
        message(STATUS "Intel Mac detected - enabling SSE")
        add_compile_options(-msse2 -mmmx)
    endif()
endif()

#################################
# Source files (local folder)
#################################

set(SRC_FILES
    Animation.cpp
    Axis.cpp
    AnimationManager.cpp
)

# Add SSE files only if they exist and SSE is enabled
if(NOT DEFINED DISABLE_SSE)
    if(EXISTS "${CMAKE_CURRENT_LIST_DIR}/SSEFile1.cpp")
        list(APPEND SRC_FILES
            SSEFile1.cpp
            SSEFile2.cpp
        )
    else()
        message(STATUS "SSE source files not found - skipping SSE compilation")
    endif()
endif()

#################################
# Create library
#################################

add_library(MathAnimations ${SRC_FILES})

# Include directory (go up to include folder)
target_include_directories(MathAnimations PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/../../include
)
