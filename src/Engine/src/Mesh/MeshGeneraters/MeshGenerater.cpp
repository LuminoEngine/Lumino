
#include "Internal.hpp"
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
            v->position = Vector4(Vector3::transformCoord(v->position.xyz(), m_generator->transform()), 0.0f);
            v->transformNormalDirection(m_generator->transform());
        }
    }
}
//
//void MeshGeneraterBuffer::transform(MeshGenerater* generator, int vertexCount)
//{
//    generator->transform(m_vertexBuffer, vertexCount);
//}
//
//void MeshGeneraterBuffer::setV(int index, const Vector3& position, const Vector2& uv, const Vector3& normal)
//{
//	m_vertexBuffer[index].set(position, normal, uv, m_generator->color());
//}
//
MeshGenerater::MeshGenerater()
	: m_color(0.0f, 0.0f, 0.0f, 1.0f)
{
}

MeshGenerater::~MeshGenerater()
{
}

//==============================================================================
// ConeMeshFactory

void ConeMeshFactory::copyFrom(const ConeMeshFactory* other)
{
	MeshGenerater::copyFrom(other);
	m_radius = other->m_radius;
	m_height = other->m_height;
	m_slices = other->m_slices;
}

bool ConeMeshFactory::init(float radius, float height, int slices)
{
	if (LN_REQUIRE(slices >= 3)) return false;
	m_radius = radius;
	m_height = height;
	m_slices = slices;
	return true;
}

void ConeMeshFactory::onGenerate(MeshGeneraterBuffer* buf)
{
	const float halfHeight = m_height / 2.0f;
	const float top = halfHeight;
	const float bottom = -halfHeight;
	const float slope = m_radius / m_height;
	uint32_t iV = 0;
	uint32_t iI = 0;

	for (int iSlice = m_slices; iSlice >= 0; iSlice--)
	{
		const Vector3 pos = MeshHelper::getXZCirclePoint(iSlice, m_slices);
		const Vector3 xz = pos * m_radius;
		const Vector3 normal = Vector3::normalize(pos.x, slope, pos.z);

		// top
		{
			buf->setV(iV, Vector3(0, top, 0), Vector2(0, 0), Vector3::UnitY); iV++;
		}
		// side
		{
			buf->setV(iV, Vector3(xz.x, bottom, xz.z), Vector2(0, 0), normal); iV++;
		}
		// lower base
		{
			buf->setV(iV, Vector3(0, bottom, 0), Vector2(0, 0), -Vector3::UnitY); iV++;
		}
	}

	// faces
	const int stacks = 2;
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

//==============================================================================
// ArcMeshFactory

ArcMeshFactory::ArcMeshFactory()
	: m_startAngle(0)
	, m_endAngle(0)
	, m_innerRadius(0)
	, m_outerRadius(0)
	, m_slices(0)
{
}

bool ArcMeshFactory::init(float startAngle, float endAngle, float innerRadius, float outerRadius, int slices)
{
	if (LN_REQUIRE(slices >= 1)) return false;
	m_startAngle = startAngle;
	m_endAngle = endAngle;
	m_innerRadius = innerRadius;
	m_outerRadius = outerRadius;
	m_slices = slices;
	return true;
}

void ArcMeshFactory::copyFrom(const ArcMeshFactory* other)
{
	MeshGenerater::copyFrom(other);
	m_startAngle = other->m_startAngle;
	m_endAngle = other->m_endAngle;
	m_innerRadius = other->m_innerRadius;
	m_outerRadius = other->m_outerRadius;
	m_slices = other->m_slices;
}

void ArcMeshFactory::onGenerate(MeshGeneraterBuffer* buf)
{
	uint32_t iV = 0;
	uint32_t iI = 0;

	for (int iSlice = 0; iSlice < m_slices + 1; iSlice++)
	{
		Vector3 n = MeshHelper::getXZArcPoint(m_startAngle, m_endAngle, iSlice, m_slices);

		// outer
		{
			buf->setV(iV, n * m_outerRadius, Vector2(0, 0), Vector3::UnitY); iV++;
		}
		// inner
		{
			buf->setV(iV, n * m_innerRadius, Vector2(0, 0), Vector3::UnitY); iV++;
		}
	}

	// faces
	for (int iSlice = 0; iSlice < m_slices; iSlice++)
	{
		int p1 = ((iSlice + 0) * 2 + 0);	// ┏
		int p2 = ((iSlice + 1) * 2 + 0);	// ┓
		int p3 = ((iSlice + 0) * 2 + 1);	// ┗
		int p4 = ((iSlice + 1) * 2 + 1);	// ┛
		buf->setI(iI + 0, p1);
		buf->setI(iI + 1, p2);
		buf->setI(iI + 2, p3);
		buf->setI(iI + 3, p3);
		buf->setI(iI + 4, p2);
		buf->setI(iI + 5, p4);
		iI += 6;
	}
}

} // namespace detail
} // namespace ln

