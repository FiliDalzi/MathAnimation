############################
# macOS Minimal Safe Build #
############################

# Detect minimal mac build
if(APPLE)
    set(MACOS_MINIMAL TRUE)
endif()

##############
# Source Files
##############

file(GLOB_RECURSE MATH_ANIMATIONS_SOURCE
    "src/*.cpp"
    "include/*.h"
    "include/*.hpp"
)

# Rimuove file problematici su mac (SSE / Windows only)
if(MACOS_MINIMAL)
    list(REMOVE_ITEM MATH_ANIMATIONS_SOURCE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/animation/Axis.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/animation/AnimationManager.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/animation/Animation.cpp
    )
endif()

##############
# Executable
##############

add_executable(MathAnimations ${MATH_ANIMATIONS_SOURCE})

target_include_directories(MathAnimations PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include
)

##########################
# Link libraries (minimal)
##########################

# Solo librerie base che servono davvero
target_link_libraries(MathAnimations PUBLIC
    glfw
    glad
    CppUtils
    glm
    stb
    nlohmann_json
)

##########################
# Compiler flags
##########################

if(MSVC)
    target_compile_options(MathAnimations PRIVATE /W4)
else()
    target_compile_options(MathAnimations PRIVATE -Wall -Wextra)
endif()
