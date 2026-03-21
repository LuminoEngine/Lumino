#pragma once
#include <lumino_base/Types.hpp>
#include <lumino_base/Math.hpp>
#include <lumino_core/graphics/rhi/Rhi.hpp>

namespace ln {

/// Standard vertex layout for 3D rendering (64 bytes).
struct Vertex {
    Vector3 position;   // 12 bytes
    Vector3 normal;     // 12 bytes
    Vector2 uv;         //  8 bytes
    Color   color;      // 16 bytes (r,g,b,a as f32)
    Vector4 tangent;    // 16 bytes (xyz = tangent, w = binormal sign)
};

static_assert(sizeof(Vertex) == 64, "Vertex must be 64 bytes");

/// Returns the standard VertexBufferLayout matching the Vertex struct.
inline rhi::VertexBufferLayout standardVertexLayout() {
    rhi::VertexBufferLayout layout;
    layout.stride = sizeof(Vertex);
    layout.perInstance = false;
    layout.attributes = {
        {0, rhi::VertexFormat::Float32x3, 0},                      // position
        {1, rhi::VertexFormat::Float32x3, sizeof(Vector3)},        // normal
        {2, rhi::VertexFormat::Float32x2, sizeof(Vector3) * 2},    // uv
        {3, rhi::VertexFormat::Float32x4, sizeof(Vector3) * 2 + sizeof(Vector2)}, // color
        {4, rhi::VertexFormat::Float32x4, sizeof(Vector3) * 2 + sizeof(Vector2) + sizeof(Color)}, // tangent
    };
    return layout;
}

} // namespace ln
