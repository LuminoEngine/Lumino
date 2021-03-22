
#include "Internal.hpp"
#include "CylinderMeshGenerater.hpp"

namespace ln {
namespace detail {

//==============================================================================
// CylinderMeshFactory

void CylinderMeshFactory::copyFrom(const CylinderMeshFactory* other)
{
	MeshGenerater::copyFrom(other);
	m_radiusTop = other->m_radiusTop;
	m_radiusBottom = other->m_radiusBottom;
	m_height = other->m_height;
	m_slices = other->m_slices;
	m_stacks = other->m_stacks;
}

bool CylinderMeshFactory::init(float radiusTop, float radiusBottom, float height, int slices, int stacks)
{
	if (LN_REQUIRE(slices >= 3)) return false;
	if (LN_REQUIRE(stacks >= 1)) return false;
	m_radiusTop = radiusTop;
	m_radiusBottom = radiusBottom;
	m_height = height;
	m_slices = slices;
	m_stacks = stacks;
	return true;
}

void CylinderMeshFactory::onGenerate(MeshGeneraterBuffer* buf)
{
	const float yStep = m_height / m_stacks;
	const float yu = m_height / 2;
	const float yd = -m_height / 2;
	const float slope = (m_radiusBottom - m_radiusTop) / m_height;

	uint32_t iV = 0;
	uint32_t iI = 0;
	float y;
	for (int iSlice = m_slices; iSlice >= 0; iSlice--)
	{
		const Vector3 pos = MeshHelper::getXZCirclePoint(iSlice, m_slices);
		const Vector3 xzTop = pos * m_radiusTop;
		const Vector3 xzBottom = pos * m_radiusBottom;
		const Vector3 normal = Vector3::normalize(pos.x, slope, pos.z);

		// upper base
		{
			buf->setV(iV, Vector3(0, yu, 0), Vector2(0, 0), Vector3::UnitY);
			iV++;
		}
		// side
		y = yu;
		for (int iStack = 0; iStack < m_stacks + 1; ++iStack)
		{
			const float u = static_cast<float>(iStack) / m_stacks + 1;
			const float radius = u * (m_radiusBottom - m_radiusTop) + m_radiusTop;

			buf->setV(iV, Vector3(pos.x * radius, y, pos.z * radius), Vector2(0, 0), normal);
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

