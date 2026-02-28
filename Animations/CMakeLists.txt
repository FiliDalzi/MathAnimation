# Animations/CMakeLists.txt

#################################
# Detect Apple Silicon / SSE
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
# Source files
#################################

set(SRC_FILES
    src/animation/Animation.cpp
    src/animation/Axis.cpp
    src/animation/AnimationManager.cpp
)

#################################
# Create library
#################################

add_library(MathAnimations ${SRC_FILES})

target_include_directories(MathAnimations PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include
)
