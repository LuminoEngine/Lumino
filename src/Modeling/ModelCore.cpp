
#pragma once
#include "../Internal.h"
#include "ModelCore.h"

namespace Lumino
{
namespace Modeling
{

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
	LN_FOREACH(ModelBoneCore* c, Bones) {
		c->Release();
	}
	LN_FOREACH(ModelIKCore* c, IKs) {
		c->Release();
	}
	LN_FOREACH(ModelMorphCore* c, Morphs) {
		c->Release();
	}
	LN_FOREACH(ModelRigidBodyCore* c, RigidBodys) {
		c->Release();
	}
	LN_FOREACH(ModelJointCore* c, Joints) {
		c->Release();
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ModelCore::RefreshInitialValues()
{
	LN_FOREACH(ModelBoneCore* c, Bones) {
		c->RefreshInitialValues();
	}
}


} // namespace Modeling
} // namespace Lumino
