
#pragma once
#include "../Graphics/Mesh.h"
#include "VisualNode.h"

LN_NAMESPACE_BEGIN
class StaticMesh;
using StaticMeshPtr = RefPtr<StaticMesh>;

/**
	@brief		
*/
class StaticMesh
	: public VisualNode
{
public:
	static StaticMeshPtr CreateBox(const Vector3& size);

LN_INTERNAL_ACCESS:
	StaticMesh();
	virtual ~StaticMesh();
	void Initialize(SceneGraph* ownerSceneGraph);
	virtual void DrawSubset(SceneGraphRenderingContext* dc, int subsetIndex) override;

private:
	RefPtr<StaticMeshModel>	m_mesh;
	RefPtr<Material3>		m_material;
};

LN_NAMESPACE_END
