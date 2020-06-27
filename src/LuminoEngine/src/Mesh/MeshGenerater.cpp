
#include "Internal.hpp"
#include <LuminoEngine/Rendering/Vertex.hpp>
#include "MeshGenerater.hpp"

namespace ln {
namespace detail {

void MeshGeneraterBuffer::setBuffer(Vertex* vertexBuffer/*, uint32_t vertexCount*/, void* indexBuffer, IndexBufferFormat indexFormat, uint32_t indexNumberOffset)
{
    m_vertexBuffer = vertexBuffer;
    //m_vertexCount = vertexCount;
    m_indexBuffer = indexBuffer;
    m_indexFormat = indexFormat;
    m_indexNumberOffset = indexNumberOffset;
}

void MeshGeneraterBuffer::generate(MeshGenerater* generator)
{
    m_generator = generator;
    m_generator->onGenerate(this);
    transform();
}

void MeshGeneraterBuffer::transform(/*Vertex* begin, int vertexCount*/)
{
    int vertexCount = m_generator->vertexCount();
    if (!m_generator->transform().isIdentity()) {
        Vertex* end = m_vertexBuffer + vertexCount;
        for (Vertex* v = m_vertexBuffer; v < end; v++) {
            v->position.transformCoord(m_generator->transform());
            v->normal.transformDirection(m_generator->transform());
        }
    }
}
//
//void MeshGeneraterBuffer::transform(MeshGenerater* generator, int vertexCount)
//{
//    generator->transform(m_vertexBuffer, vertexCount);
//}

void MeshGeneraterBuffer::setV(int index, const Vector3& position, const Vector2& uv, const Vector3& normal)
{
    m_vertexBuffer[index].position = position;
    m_vertexBuffer[index].uv = uv;
    m_vertexBuffer[index].normal = normal;
    m_vertexBuffer[index].color = m_generator->color();
    m_vertexBuffer[index].tangent = Vector4(1, 0, 0, 1);
}


MeshGenerater::~MeshGenerater()
{
}

} // namespace detail
} // namespace ln

