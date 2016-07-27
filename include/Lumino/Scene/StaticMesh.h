
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
	static StaticMeshPtr Create(const StringRef& filePath);

	static StaticMeshPtr CreateBox(const Vector3& size);

	static StaticMeshPtr CreateSphere(float radius, int slices, int stacks);

	static StaticMeshPtr CreateSquarePlane(const Vector2& size, const Vector3& front, MeshCreationFlags flags);

	static StaticMeshPtr CreateScreenPlane();

public:

	StaticMeshModel* GetStaticMeshModel() const { return m_mesh; }

LN_INTERNAL_ACCESS:
	StaticMesh();
	virtual ~StaticMesh();
	void Initialize(SceneGraph* ownerSceneGraph, StaticMeshModel* meshModel);
	virtual void DrawSubset(SceneGraphRenderingContext* dc, int subsetIndex) override;

private:
	RefPtr<StaticMeshModel>	m_mesh;
};

LN_NAMESPACE_END
