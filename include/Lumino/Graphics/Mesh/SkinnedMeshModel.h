
#pragma once
#include "../Mesh.h"

LN_NAMESPACE_BEGIN
class PmxSkinnedMeshResource;
class SkinnedMeshModel;
using SkinnedMeshModelPtr = RefPtr<SkinnedMeshModel>;

/**
	@brief
*/
class SkinnedMeshModel
	: public Object
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:

LN_INTERNAL_ACCESS:
	SkinnedMeshModel();
	virtual ~SkinnedMeshModel();
	void Initialize(GraphicsManager* manager, PmxSkinnedMeshResource* sharingMesh);

LN_INTERNAL_ACCESS:	// TODO:
	RefPtr<PmxSkinnedMeshResource>	m_meshResource;
	RefPtr<MaterialList>			m_materials;
};

LN_NAMESPACE_END
