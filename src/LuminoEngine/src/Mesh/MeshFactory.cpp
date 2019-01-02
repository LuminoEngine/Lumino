
#include "Internal.hpp"
#include <LuminoEngine/Rendering/Vertex.hpp>
#include "MeshFactory.hpp"

namespace ln {
namespace detail {

void MeshGeneraterBuffer::setBuffer(Vertex* vertexBuffer, void* indexBuffer, IndexBufferFormat indexFormat)
{
    m_vertexBuffer = vertexBuffer;
    m_indexBuffer = indexBuffer;
    m_indexFormat = indexFormat;
}

} // namespace detail
} // namespace ln

