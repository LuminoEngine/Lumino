
#pragma once
#include <LuminoEngine/Graphics/ColorStructs.hpp>

namespace ln {

struct Vertex
{
    static const Vertex Default;

    Vector4 position;
    Vector4 normal;
    Vector4 uv;
    Color color;
    Vector4 tangent;    // 接線ベクトル, 従法線の符号

    Vertex()
        : position(0.0f, 0.0f, 0.0f, 1.0f)
        , normal(0.0f, 0.0f, 1.0f, 0.0f)
        , uv(0.0f, 1.0f, 0.0f, 0.0f)
        , color(0.0f, 0.0f, 0.0f, 1.0f)
        , tangent(1.0f, 0.0f, 0.0f, 1.0f)
    {}

    Vertex(const Vector3& position_, const Vector3& normal_, const Vector2& uv_, const Color& color_, const Vector3& tangent_ = Vector3::UnitX)
        : position(position_.x, position_.y, position_.z, 1.0f)
        , normal(normal_.x, normal_.y, normal_.z, 0.0f)
        , uv(uv_.x, uv_.y, 0.0f, 0.0f)
        , color(color_.r, color_.g, color_.b, color_.a)
        , tangent(tangent_.x, tangent_.y, tangent_.z, 1.0f)
    {}

    void set(const Vector3& position_, const Vector3& normal_, const Vector2& uv_, const Color& color_, const Vector3& tangent_ = Vector3::UnitX)
    {
        position.set(position_.x, position_.y, position_.z, 1.0f);
        normal.set(normal_.x, normal_.y, normal_.z, 0.0f);
        uv.set(uv_.x, uv_.y, 0.0f, 0.0f);
        color.set(color_.r, color_.g, color_.b, color_.a);
        tangent.set(tangent_.x, tangent_.y, tangent_.z, 1.0f);
    }

    void setPosition(const Vector3& value) { position.set(value.x, value.y, value.z, position.w); }

    void setPosition(float x, float y, float z) { position.set(x, y, z, position.w); }

    void setNormal(const Vector3& value) { normal.set(value.x, value.y, value.z, normal.w); }

    void setUV(float x, float y) { uv.set(x, y, uv.z, uv.w); }
    void setUV(const Vector2& value) { uv.set(value.x, value.y, uv.z, uv.w); }

    void transformPosition(const Matrix& m)
    {
        position = Vector4(Vector3::transformCoord(position.xyz(), m), 1.0f);
    }

    void transformNormalDirection(const Matrix& m)
    {
        Vector3 t = normal.xyz();
        t.transformDirection(m);
        normal = Vector4(t, 0.0f);
    }
};

struct VertexBlendWeight
{
    static const VertexBlendWeight Default;

    float        indices[4];
    float        weights[4];
};

struct VertexMorphTarget
{
    Vector3 position;
    Vector3 normal;
    Vector3 tangent;
};

// TEXCOORD_1,TEXCOORD_2,TEXCOORD_3
struct VertexAdditionalUV
{
    Vector4        uv[3];
};

struct VertexSdefInfo
{
    Vector4        sdefC;
    Vector3        sdefR0;
    Vector3        sdefR1;
};

struct VertexMmdExtra
{
    float        edgeWeight;
    float        index;
};

} // namespace ln
