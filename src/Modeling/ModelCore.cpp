
#include "../Internal.h"
#include "ModelCore.h"

LN_NAMESPACE_BEGIN

//=============================================================================
// ModelBoneCore
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ModelBoneCore::ModelBoneCore(ModelCore* owner, int boneIndex)
	: m_owner(owner)
	, m_boneIndex(boneIndex)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ModelBoneCore::RefreshInitialValues()
{
	if (ParentBoneIndex >= 0)
	{
		m_offsetFromParent = OrgPosition - m_owner->Bones[ParentBoneIndex]->OrgPosition;
		m_initialTranstormInv = Matrix::Translation(-OrgPosition);
	}
}
	
//=============================================================================
// ModelCore
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ModelCore::~ModelCore()
{
	for (ModelBoneCore* c : Bones) {
		c->Release();
	}
	for (ModelIKCore* c : IKs) {
		c->Release();
	}
	for (ModelMorphCore* c : Morphs) {
		c->Release();
	}
	for (ModelRigidBodyCore* c : RigidBodys) {
		c->Release();
	}
	for (ModelJointCore* c : Joints) {
		c->Release();
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ModelCore::RefreshInitialValues()
{
	for (ModelBoneCore* c : Bones) {
		c->RefreshInitialValues();
	}
}

LN_NAMESPACE_END
