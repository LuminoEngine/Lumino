#pragma once
#include <LuminoBase/Types.hpp>
#include <LuminoBase/math/Math.hpp>
#include <LuminoCore/Graphics/rhi/Rhi.hpp>

namespace ln {

/** 3D レンダリング用の標準頂点レイアウト (64 バイト)。 */
struct Vertex {
    Vector3 position;   // 12 バイト
    Vector3 normal;     // 12 バイト
    Vector2 uv;         //  8 バイト
    Color   color;      // 16 バイト (r,g,b,a を float で)
    Vector4 tangent;    // 16 バイト (xyz = 接線, w = 従法線の符号)
};

static_assert(sizeof(Vertex) == 64, "Vertex must be 64 bytes");

/** Vertex 構造体に対応する標準の VertexBufferLayout を返す。 */
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
