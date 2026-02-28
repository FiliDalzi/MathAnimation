#ifndef MATH_ANIM_CORE_H
#define MATH_ANIM_CORE_H

// ======================================================
// Logger fallback (CI / macOS ARM safe)
// ======================================================

#include <iostream>
#include <cassert>

#ifndef g_logger_assert
#define g_logger_assert(cond, msg, ...) \
    do { \
        if (!(cond)) { \
            std::cerr << "Assertion failed: " << msg << std::endl; \
            assert(cond); \
        } \
    } while(0)
#endif

#ifndef g_logger_warning
#define g_logger_warning(msg, ...) \
    do { \
        std::cerr << "Warning: " << msg << std::endl; \
    } while(0)
#endif

// ======================================================
// GLM
// ======================================================

#pragma warning(push)
#pragma warning(disable : 4201)
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#pragma warning(pop)

// ======================================================
// My stuff
// ======================================================

#define USE_GABE_CPP_PRINT
#include <cppUtils/cppUtils.hpp>

// ======================================================
// Standard
// ======================================================

#include <filesystem>
#include <cstring>
#include <fstream>
#include <sstream>
#include <array>
#include <cstdio>
#include <vector>
#include <unordered_map>
#include <string>
#include <optional>
#include <random>
#include <queue>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <set>
#include <unordered_set>
#include <regex>
#include <type_traits>

// ======================================================
// GLFW / GLAD
// ======================================================

#ifndef APIENTRY
#ifdef _WIN32
#define APIENTRY __stdcall
#else
#define APIENTRY
#endif
#endif

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#ifdef APIENTRY
#undef APIENTRY
#endif

// ======================================================
// stb
// ======================================================

#include <stb/stb_image.h>
#include <stb/stb_write.h>
#include <stb/stb_image_resize.h>

// ======================================================
// Freetype
// ======================================================

#include <ft2build.h>
#include FT_FREETYPE_H

// ======================================================
// Core library stuff
// ======================================================

#include "math/DataStructures.h"

#include <imgui.h>
#include <oniguruma.h>

// ======================================================
// SIMD intrinsics (only on x86)
// ======================================================

#if defined(__x86_64__) && !defined(DISABLE_SSE)
    #include <xmmintrin.h>
    #include <emmintrin.h>
    #include <mmintrin.h>
#endif

// ======================================================
// User defined literals
// ======================================================

MathAnim::Vec4 operator""_hex(const char* hexColor, size_t length);
MathAnim::Vec4 toHex(const std::string& str);
MathAnim::Vec4 toHex(const char* hex, size_t length);
MathAnim::Vec4 toHex(const char* hex);
std::string toHexString(const MathAnim::Vec4& color);

// ======================================================
// Utility macros
// ======================================================

#define MATH_ANIM_ENUM_FLAG_OPS(enumName) \
inline enumName operator|(enumName lhs, enumName rhs) { \
    return static_cast<enumName>( \
        static_cast<std::underlying_type_t<enumName>>(lhs) | \
        static_cast<std::underlying_type_t<enumName>>(rhs)); \
} \
inline enumName operator&(enumName lhs, enumName rhs) { \
    return static_cast<enumName>( \
        static_cast<std::underlying_type_t<enumName>>(lhs) & \
        static_cast<std::underlying_type_t<enumName>>(rhs)); \
}

#define KB(x) (x * 1024)
#define MB(x) (x * KB(1024))
#define GB(x) (x * MB(1024))

// ======================================================
// Memory structures
// ======================================================

struct RawMemory
{
    uint8* data;
    size_t size;
    size_t offset;

    void init(size_t initialSize);
    void free();
    void shrinkToFit();
    void resetReadWriteCursor();

    void writeDangerous(const uint8* data, size_t dataSize);
    bool readDangerous(uint8* data, size_t dataSize);

    template<typename T>
    void write(const T* data)
    {
        writeDangerous((uint8*)data, sizeof(T));
    }

    template<typename T>
    bool read(T* data)
    {
        return readDangerous((uint8*)data, sizeof(T));
    }

    void setCursor(size_t offset);
};

struct SizedMemory
{
    uint8* memory;
    size_t size;
};

// ======================================================
// Memory helper
// ======================================================

namespace MemoryHelper
{
    template<typename T>
    size_t copyDataByType(uint8* dst, size_t offset, const T& data)
    {
        *(T*)(dst + offset) = data;
        return sizeof(T);
    }

    template<typename First, typename... Rest>
    void copyDataToType(uint8* dst, size_t offset, const First& data, Rest... rest)
    {
        static_assert(std::is_trivially_copyable_v<First>,
            "Only trivially copyable types allowed.");

        offset += copyDataByType<First>(dst, offset, data);

        if constexpr (sizeof...(Rest) > 0)
        {
            copyDataToType<Rest...>(dst, offset, rest...);
        }
    }

    template<typename First, typename... Rest>
    void unpackData(const SizedMemory& memory, size_t offset, First* first, Rest*... rest)
    {
#ifdef _DEBUG
        g_logger_assert(offset + sizeof(First) <= memory.size,
            "Buffer overrun while unpacking memory.");
#endif

        static_assert(std::is_trivially_copyable_v<First>,
            "Only trivially copyable types allowed.");

        *first = *(First*)(memory.memory + offset);

        if constexpr (sizeof...(Rest) > 0)
        {
            unpackData<Rest...>(memory, offset + sizeof(First), rest...);
        }
    }
}

// ======================================================
// Pack / Unpack helpers
// ======================================================

template<typename First, typename... Rest>
constexpr size_t sizeOfTypes()
{
    if constexpr (sizeof...(Rest) == 0)
        return sizeof(First);
    else
        return sizeof(First) + sizeOfTypes<Rest...>();
}

template<typename... Types>
SizedMemory pack(const Types&... data)
{
    size_t totalSize = sizeOfTypes<Types...>();
    uint8* result = (uint8*)g_memory_allocate(totalSize);
    MemoryHelper::copyDataToType<Types...>(result, 0, data...);
    return { result, totalSize };
}

template<typename... Types>
void unpack(const SizedMemory& memory, Types*... data)
{
    MemoryHelper::unpackData<Types...>(memory, 0, data...);
}

// ======================================================
// Array helpers (USED BY Svg.h)
// ======================================================

template <typename T, std::size_t N, class... Args>
constexpr std::array<T, N> fixedSizeArray(Args&&... values)
{
    static_assert(sizeof...(values) == N,
        "Number of values must match array size");
    return std::array<T, N>{ { std::forward<Args>(values)... } };
}

template<typename T, std::size_t N>
T findMatchingEnum(const std::array<const char*, N>& names,
                   const std::string& value)
{
    for (size_t i = 0; i < N; i++)
    {
        if (names[i] == value)
            return static_cast<T>(i);
    }
    return static_cast<T>(0);
}

// ======================================================
// IDs
// ======================================================

typedef uint64 AnimObjId;
typedef uint64 AnimId;
typedef uint64 TextureHandle;

namespace MathAnim
{
    constexpr AnimObjId NULL_ANIM_OBJECT = UINT64_MAX;
    constexpr AnimId NULL_ANIM = UINT64_MAX;
    constexpr TextureHandle NULL_TEXTURE_HANDLE = UINT64_MAX;

    inline bool isNull(uint64 handle)
    {
        return handle == UINT64_MAX;
    }
}

#endif
