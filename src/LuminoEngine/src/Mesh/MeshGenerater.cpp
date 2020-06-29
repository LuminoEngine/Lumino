
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

//==============================================================================
// CylinderMeshFactory

bool CylinderMeshFactory::init(float radius, float height, int slices, int stacks)
{
	if (LN_REQUIRE(slices >= 3)) return false;
	if (LN_REQUIRE(stacks >= 1)) return false;
	m_radius = radius;
	m_height = height;
	m_slices = slices;
	m_stacks = stacks;
	return true;
}

void CylinderMeshFactory::onGenerate(MeshGeneraterBuffer* buf)
{
	uint32_t iV = 0;
	uint32_t iI = 0;

	float yStep = m_height / m_stacks;
	float y;
	float yu = m_height / 2;
	float yd = -m_height / 2;

	for (int iSlice = m_slices; iSlice >= 0; iSlice--)
	{
		Vector3 n = MeshHelper::getXZCirclePoint(iSlice, m_slices);
		Vector3 xz = n * m_radius;

		// upper base
		{
			buf->setV(iV, Vector3(0, yu, 0), Vector2(0, 0), Vector3::UnitY);
			iV++;
		}
		// side
		y = yu;
		for (int iStack = 0; iStack < m_stacks + 1; ++iStack)
		{
			buf->setV(iV, Vector3(xz.x, y, xz.z), Vector2(0, 0), n);
			iV++;
			y -= yStep;
		}
		// lower base
		{
			buf->setV(iV, Vector3(0, yd, 0), Vector2(0, 0), -Vector3::UnitY);
			iV++;
		}
	}

	// faces
	int stacks = m_stacks + 2;
	for (int iSlice = 0; iSlice < m_slices; ++iSlice)	// x
	{
		for (int iStack = 0; iStack < stacks; ++iStack)	// y
		{
			int p1 = (iStack + 0) + (iSlice + 0) * (stacks + 1);	// ┏
			int p2 = (iStack + 0) + (iSlice + 1) * (stacks + 1);	// ┓
			int p3 = (iStack + 1) + (iSlice + 0) * (stacks + 1);	// ┗
			int p4 = (iStack + 1) + (iSlice + 1) * (stacks + 1);	// ┛
			buf->setI(iI + 0, p1);
			buf->setI(iI + 1, p2);
			buf->setI(iI + 2, p3);
			buf->setI(iI + 3, p3);
			buf->setI(iI + 4, p2);
			buf->setI(iI + 5, p4);
			iI += 6;
		}
	}
}

} // namespace detail
} // namespace ln

