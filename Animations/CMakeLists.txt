cmake_minimum_required(VERSION 3.21)

project(MathAnimations VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

#################################
# Rileva OS e architettura      #
#################################
if(APPLE)
    set(MATH_ANIMATION_OS_OSX TRUE)

    if(CMAKE_SYSTEM_PROCESSOR STREQUAL "arm64")
        message(STATUS "Apple Silicon detected - disabling SSE")
        add_compile_definitions(DISABLE_SSE)
    else()
        message(STATUS "Intel Mac detected - enabling SSE")
        add_compile_options(-msse2 -mmmx)
        set(CMAKE_OSX_ARCHITECTURES "x86_64")
    endif()
elseif(WIN32)
    set(MATH_ANIMATION_OS_WINDOWS TRUE)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(MATH_ANIMATION_OS_LINUX TRUE)
endif()

#################################
# Configure vendored libraries  #
#################################
set(DEAR_IMGUI_USER_CONFIG ${CMAKE_CURRENT_LIST_DIR}/include/core/InternalImGuiConfig.h)
add_subdirectory(vendor)

#################################
# Animations library            #
#################################
set(SRC_FILES
    src/animation/Animation.cpp
    src/animation/Axis.cpp
    src/animation/AnimationManager.cpp
)

# Compila i file SSE solo se SSE non è disabilitato
if(NOT DEFINED DISABLE_SSE)
    list(APPEND SRC_FILES
        src/animation/SSEFile1.cpp
        src/animation/SSEFile2.cpp
    )
endif()

add_library(MathAnimations ${SRC_FILES})
target_include_directories(MathAnimations PUBLIC include)

set(MATH_ANIMATIONS_INCLUDE_DIR
    ${CMAKE_CURRENT_LIST_DIR}/include
    ${CMAKE_CURRENT_LIST_DIR}/tests
)

#################################
# Eseguibile principale         #
#################################
file(GLOB_RECURSE MATH_ANIMATIONS_SOURCE
    "src/*.cpp"
    "include/*.h"
    "include/*.hpp"
    "tests/*.cpp"
    "tests/*.h"
)

add_executable(MathAnimationsApp ${MATH_ANIMATIONS_SOURCE})

target_include_directories(MathAnimationsApp PUBLIC ${MATH_ANIMATIONS_INCLUDE_DIR})

target_compile_definitions(MathAnimationsApp PUBLIC
    _CRT_SECURE_NO_WARNINGS=1
)

#################################
# Link libraries (FULL BUILD)   #
#################################
target_link_libraries(MathAnimationsApp PUBLIC
    MathAnimations

    # Window / OpenGL
    glfw
    glad
    NativeFileDialog

    # Fonts / Audio
    freetype
    OpenAL

    # Core utils
    CppUtils
    glm
    stb
    nlohmann_json

    # Parsing
    onig
    plutovg
    tinyxml2

    # ImGUI
    DearImGUIGlfwOgl3Backend
    DearImGUI

    # Luau
    Luau.Ast
    Luau.CodeGen
    Luau.Compiler
    Luau.VM
    Luau.Analysis

    # AV1 Encoder (for export)
    SvtAv1Enc
)

# OpenSSL su Linux
if(MATH_ANIMATION_OS_LINUX)
    find_package(OpenSSL REQUIRED)
    target_link_libraries(MathAnimationsApp PUBLIC OpenSSL::Crypto)
endif()
