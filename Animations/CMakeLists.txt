#################################
# Configure vendored libraries  #
#################################

set(DEAR_IMGUI_USER_CONFIG ${CMAKE_CURRENT_LIST_DIR}/include/core/InternalImGuiConfig.h)

add_subdirectory(vendor)

##############
# Animations #
##############

set(MATH_ANIMATIONS_INCLUDE_DIR
    ${CMAKE_CURRENT_LIST_DIR}/include
    ${CMAKE_CURRENT_LIST_DIR}/tests
)

file(GLOB_RECURSE MATH_ANIMATIONS_SOURCE
    "src/*.cpp"
    "include/*.h"
    "include/*.hpp"
    "tests/*.cpp"
    "tests/*.h"
)

add_executable(MathAnimations ${MATH_ANIMATIONS_SOURCE})

target_include_directories(MathAnimations PUBLIC
    ${MATH_ANIMATIONS_INCLUDE_DIR}
)

target_compile_definitions(MathAnimations PUBLIC
    _CRT_SECURE_NO_WARNINGS=1
)

#################################
# Link libraries (FULL BUILD)   #
#################################

target_link_libraries(MathAnimations PUBLIC

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

if(MATH_ANIMATION_OS_LINUX)
    find_package(OpenSSL REQUIRED)
    target_link_libraries(MathAnimations PUBLIC OpenSSL::Crypto)
endif()
