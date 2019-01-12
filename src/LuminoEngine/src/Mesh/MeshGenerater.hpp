#pragma once
#include "../../../LuminoCore/src/Base/LinearAllocator.hpp"
#include <LuminoEngine/Graphics/Common.hpp>

namespace ln {
namespace detail {

class MeshGeneraterBuffer
{
public:
    void setBuffer(Vertex* vertexBuffer, void* indexBuffer, IndexBufferFormat indexFormat, uint32_t indexNumberOffset);
    void setColor(const Color& color) { m_color = color; }
    void setTransform(const Matrix& transform) { m_transform = transform; }


    Vertex* vertexBuffer() const { return m_vertexBuffer; }

    void setV(int index, const Vertex& v)
    {
        m_vertexBuffer[index] = v;
    }

    void setV(int index, const Vector3&	position, const Vector2& uv, const Vector3& normal)
    {
        m_vertexBuffer[index].position = position;
        m_vertexBuffer[index].uv = uv;
        m_vertexBuffer[index].normal = normal;
        m_vertexBuffer[index].color = m_color;
    }

    void setI(int index, uint32_t i)
    {
        if (m_indexFormat == IndexBufferFormat::UInt16)
            ((uint16_t*)m_indexBuffer)[index] = m_indexNumberOffset + i;
        else
            ((uint32_t*)m_indexBuffer)[index] = m_indexNumberOffset + i;
    }

    void transform(Vertex* begin, int vertexCount)
    {
        if (!m_transform.isIdentity())
        {
            Vertex* end = begin + vertexCount;
            for (Vertex* v = begin; v < end; v++)
            {
                v->position.transformCoord(m_transform);
            }
        }
    }

private:
    Vertex* m_vertexBuffer;
    void* m_indexBuffer;
    IndexBufferFormat m_indexFormat;
    uint32_t m_indexNumberOffset;
    Color m_color;
    Matrix m_transform;
};

// 描画コマンドのバッファに詰め込まれるのであまり大きいデータは持たせないように。
class MeshGenerater
{
public:
    //virtual size_t instanceSize() const = 0;
    //virtual void copyTo(MeshGenerater* other) const = 0;
    virtual int vertexCount() const = 0;
    virtual int indexCount() const = 0;
    virtual PrimitiveType primitiveType() const = 0;
    virtual MeshGenerater* clone(LinearAllocator* allocator) const = 0;
    virtual void generate(MeshGeneraterBuffer* buf) = 0;
};

class SingleLineGenerater
    : public MeshGenerater
{
public:
    Vector3 point1;
    Color point1Color;
    Vector3 point2;
    Color point2Color;

    virtual int vertexCount() const override { return 2; }
    virtual int indexCount() const override { return 2; }
    virtual PrimitiveType primitiveType() const override { return PrimitiveType::LineList; }
    virtual MeshGenerater* clone(LinearAllocator* allocator) const override
    {
        auto* ptr = allocator->allocate(sizeof(SingleLineGenerater));
        return new (ptr)SingleLineGenerater(*this);
    }
    virtual void generate(MeshGeneraterBuffer* buf) override
    {
        buf->setV(0, Vertex{ point1, Vector3::UnitY, Vector2::Zero, point1Color });
        buf->setV(1, Vertex{ point2, Vector3::UnitY, Vector2::Zero, point2Color });
        buf->setI(0, 0);
        buf->setI(1, 1);
    }
};

class RegularSphereMeshFactory
    : public MeshGenerater
{
public:
    float m_radius;
    int m_slices;
    int m_stacks;

    //RegularSphereMeshFactory(float radius, int slices, int stacks)
    //    : m_radius(radius)
    //    , m_slices(slices)
    //    , m_stacks(stacks)
    //{
    //    if (LN_REQUIRE(slices >= 3)) return;
    //    if (LN_REQUIRE(stacks >= 2)) return;
    //    makeSinCosTable();
    //}

    virtual int vertexCount() const override { return (m_slices + 1) * (m_stacks + 1); }
    virtual int indexCount() const override { return m_slices * m_stacks * 6; }
    virtual PrimitiveType primitiveType() const override { return PrimitiveType::TriangleList; }
    virtual MeshGenerater* clone(LinearAllocator* allocator) const override
    {
        auto* ptr = allocator->allocate(sizeof(RegularSphereMeshFactory));
        return new (ptr)RegularSphereMeshFactory(*this);
    }

    struct sinCos
    {
        float	sin;
        float	cos;
    };

    virtual void generate(MeshGeneraterBuffer* buf) override
    {
        makeSinCosTable();

        uint32_t iV = 0;
        uint32_t iI = 0;

        float stackUVStep = 1.0f / m_stacks;
        float stackUV = 0.0f;

        float sliceUVStep = 1.0f / m_slices;

        // XY 平面上の step
        float theta_step = Math::PI / m_stacks;
        float theta = 0.0f;//theta_step;

        // Z+ を起点とし、X- 方向へ回っていく

        // rings (Vertex)
        Vector3 normal;
        for (int iStack = 0; iStack < m_stacks + 1; ++iStack)
        {
            float sin_theta = sinf(theta);
            float cos_theta = cosf(theta);
            float sliceUV = 0.0f;

            for (int iSlice = 0; iSlice < m_slices + 1; ++iSlice)
            {
                // top
                if (iStack == 0)
                {
                    normal.x = 0.0f;
                    normal.y = 1.0f;
                    normal.z = 0.0f;
                }
                // bottom
                else if (iStack == m_stacks)
                {
                    normal.x = 0.0f;
                    normal.y = -1.0f;
                    normal.z = 0.0f;
                }
                // between
                else
                {
                    normal.x = sin_theta * m_sincosTable[iSlice].cos;
                    normal.y = cos_theta;
                    normal.z = sin_theta * m_sincosTable[iSlice].sin;
                }

                buf->setV(iV, normal * m_radius, Vector2(sliceUV, stackUV), normal);
                ++iV;

                sliceUV += sliceUVStep;
            }

            stackUV += stackUVStep;
            theta += theta_step;
        }

        // faces
        for (int iStack = 0; iStack < m_stacks; ++iStack)
        {
            for (int iSlice = 0; iSlice < m_slices; ++iSlice)
            {
                int p1 = (iSlice + 0) + (iStack + 0) * (m_slices + 1);	// ┏
                int p2 = (iSlice + 1) + (iStack + 0) * (m_slices + 1);	// ┓
                int p3 = (iSlice + 0) + (iStack + 1) * (m_slices + 1);	// ┗
                int p4 = (iSlice + 1) + (iStack + 1) * (m_slices + 1);	// ┛
                buf->setI(iI + 0, p1);
                buf->setI(iI + 1, p2);
                buf->setI(iI + 2, p3);
                buf->setI(iI + 3, p3);
                buf->setI(iI + 4, p2);
                buf->setI(iI + 5, p4);
                iI += 6;
            }
        }

        buf->transform(buf->vertexBuffer(), vertexCount());
    }

    void makeSinCosTable()
    {
        if (m_sincosTable.isEmpty())
        {
            float phi_start = Math::PI / 2.0f;
            float phi_step = -2.0f * Math::PI / m_slices;
            m_sincosTable.resize(m_slices + 1);

            float angle = phi_start;
            for (int i = 0; i < m_slices; ++i)
            {
                m_sincosTable[i].sin = std::sinf(angle);
                m_sincosTable[i].cos = std::cosf(angle);
                angle -= phi_step;
            }

            m_sincosTable[m_slices] = m_sincosTable[0];
        }
    }

private:
    List<sinCos> m_sincosTable;
};

} // namespace detail
} // namespace ln

