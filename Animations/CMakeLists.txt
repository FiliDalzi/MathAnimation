#################################
# Architettura
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
# Vendor
#################################

add_subdirectory(vendor)

#################################
# Source files
#################################

file(GLOB_RECURSE PROJECT_SOURCES
    "${CMAKE_CURRENT_LIST_DIR}/src/*.cpp"
)

list(FILTER PROJECT_SOURCES EXCLUDE REGEX ".*/tests/.*")

#################################
# Executable
#################################

add_executable(MathAnimApp ${PROJECT_SOURCES})

target_include_directories(MathAnimApp PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include
    ${CMAKE_CURRENT_LIST_DIR}/vendor
    ${CMAKE_CURRENT_LIST_DIR}/vendor/cppUtilisLocal/single_include
)

#################################
# Link libraries
#################################

target_link_libraries(MathAnimApp PUBLIC

    glfw
    glad
    NativeFileDialog
    freetype
    OpenAL
    glm
    stb
    nlohmann_json
    onig
    plutovg
    tinyxml2
    DearImGUIGlfwOgl3Backend
    DearImGUI

    CppUtils

    Luau.Ast
    Luau.Compiler
    Luau.CodeGen
    Luau.VM
    Luau.Analysis
)

# Aggiungi Threads qui
find_package(Threads REQUIRED)
target_link_libraries(MathAnimApp PUBLIC Threads::Threads)
