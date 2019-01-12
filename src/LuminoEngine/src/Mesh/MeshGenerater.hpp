#pragma once
#include "../../../LuminoCore/src/Base/LinearAllocator.hpp"
#include <LuminoEngine/Graphics/Common.hpp>

namespace ln {
namespace detail {

class MeshGeneraterBuffer
{
public:
    void setBuffer(Vertex* vertexBuffer, void* indexBuffer, IndexBufferFormat indexFormat, uint32_t indexNumberOffset);

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
    virtual void generate(MeshGeneraterBuffer* buf) = 0;
    virtual MeshGenerater* clone(LinearAllocator* allocator) const = 0;
};

class SingleLineGenerater
    : public MeshGenerater
{
public:
    Vector3 point1;
    Color point1Color;
    Vector3 point2;
    Color point2Color;

    virtual int vertexCount() const { return 2; }
    virtual int indexCount() const { return 2; }
    virtual PrimitiveType primitiveType() const { return PrimitiveType::LineList; }
    virtual void generate(MeshGeneraterBuffer* buf)
    {
        buf->setV(0, Vertex{ point1, Vector3::UnitY, Vector2::Zero, point1Color });
        buf->setV(1, Vertex{ point2, Vector3::UnitY, Vector2::Zero, point2Color });
        buf->setI(0, 0);
        buf->setI(1, 1);
    }
    virtual MeshGenerater* clone(LinearAllocator* allocator) const
    {
        auto* ptr = allocator->allocate(sizeof(SingleLineGenerater));
        return new (ptr)SingleLineGenerater(*this);
    }
};

} // namespace detail
} // namespace ln

