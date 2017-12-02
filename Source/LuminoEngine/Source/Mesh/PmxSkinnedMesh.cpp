
#include "../Internal.h"
#include "PmxSkinnedMesh.h"

LN_NAMESPACE_BEGIN


//==============================================================================
// PmxMaterialResource
//==============================================================================

//------------------------------------------------------------------------------
Ref<CommonMaterial> PmxMaterialResource::MakeCommonMaterial() const
{
	auto m = Ref<CommonMaterial>::makeRef();
	m->setBuiltinColorParameter(
		CommonMaterial::DiffuseParameter,
		Diffuse.r,
		Diffuse.g,
		Diffuse.b,
		Diffuse.a);
	m->setBuiltinColorParameter(
		CommonMaterial::AmbientParameter,
		Ambient.r,
		Ambient.g,
		Ambient.b,
		Ambient.a);
	m->setBuiltinColorParameter(
		CommonMaterial::SpecularParameter,
		Specular.r,
		Specular.g,
		Specular.b,
		Specular.a);
	m->setBuiltinColorParameter(
		CommonMaterial::EmissiveParameter,
		Emissive.r,
		Emissive.g,
		Emissive.b,
		Emissive.a);
	m->setBuiltinFloatParameter(
		CommonMaterial::PowerParameter,
		Power);
	m->setBuiltinTextureParameter(
		CommonMaterial::MaterialTextureParameter,
		Texture);
	return m;
}

//==============================================================================
// PmxBoneResource
//==============================================================================

//------------------------------------------------------------------------------
PmxBoneResource::PmxBoneResource(PmxSkinnedMeshResource* owner, int boneIndex)
	: m_owner(owner)
	, m_boneIndex(boneIndex)
{
}

//------------------------------------------------------------------------------
void PmxBoneResource::refreshInitialValues()
{
	if (ParentBoneIndex >= 0)
	{
		m_offsetFromParent = OrgPosition - m_owner->bones[ParentBoneIndex]->OrgPosition;
	}
	else
	{
		m_offsetFromParent = OrgPosition;	// モデル原点からのオフセット
	}
		
	m_initialTranstormInv = Matrix::makeTranslation(-OrgPosition);
}

//==============================================================================
// ModelCore
//==============================================================================

//------------------------------------------------------------------------------
PmxSkinnedMeshResource::~PmxSkinnedMeshResource()
{
}

//------------------------------------------------------------------------------
void PmxSkinnedMeshResource::refreshInitialValues()
{
	for (Ref<PmxBoneResource>& b : bones)
	{
		b->refreshInitialValues();
	}
}

LN_NAMESPACE_END
