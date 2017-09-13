
#include "../Internal.h"
#include <Lumino/Mesh/Mesh.h>
#include "SMesh.h"

LN_NAMESPACE_BEGIN
namespace tr {

//==============================================================================
// SrMesh
//==============================================================================

void SrMesh::addVertices(int count)
{
	int start = m_vertices.getCount();
	m_vertices.resize(start + count);
	for (int i = 0; i < count; i++)
	{
		m_vertices[start + i] = newObject<SrVertex>();
	}
}

SrVertex* SrMesh::getVertex(int index)
{
	return m_vertices[index];
}

//==============================================================================
// SrMeshModel
//==============================================================================

SrMesh* SrMeshModel::addMesh()
{
	auto mesh = newObject<SrMesh>();
	m_meshes.add(mesh);
	return mesh;
}

void SrMeshModel::addMaterial(Material* material)
{
	materials.add(material);
}

//==============================================================================
// SrMeshOperation
//==============================================================================

void SrMeshOperation::moveVertex(SrVertex* v, const Vector3& pos)
{
	if (LN_REQUIRE(v)) return;

	v->setPosition(pos);
}

} // namespace tr
LN_NAMESPACE_END
