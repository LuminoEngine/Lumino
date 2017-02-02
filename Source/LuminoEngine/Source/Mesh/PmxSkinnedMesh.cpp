
#include "../Internal.h"
#include "PmxSkinnedMesh.h"

LN_NAMESPACE_BEGIN


//==============================================================================
// PmxMaterialResource
//==============================================================================

//------------------------------------------------------------------------------
RefPtr<Material> PmxMaterialResource::MakeCommonMaterial() const
{
	auto m = RefPtr<Material>::MakeRef();
	m->SetBuiltinColorParameter(
		Material::DiffuseParameter,
		Diffuse.r,
		Diffuse.g,
		Diffuse.b,
		Diffuse.a);
	m->SetBuiltinColorParameter(
		Material::AmbientParameter,
		Ambient.r,
		Ambient.g,
		Ambient.b,
		Ambient.a);
	m->SetBuiltinColorParameter(
		Material::SpecularParameter,
		Specular.r,
		Specular.g,
		Specular.b,
		Specular.a);
	m->SetBuiltinColorParameter(
		Material::EmissiveParameter,
		Emissive.r,
		Emissive.g,
		Emissive.b,
		Emissive.a);
	m->SetBuiltinFloatParameter(
		Material::PowerParameter,
		Power);
	m->SetBuiltinTextureParameter(
		Material::MaterialTextureParameter,
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
void PmxBoneResource::RefreshInitialValues()
{
	if (ParentBoneIndex >= 0)
	{
		m_offsetFromParent = OrgPosition - m_owner->bones[ParentBoneIndex]->OrgPosition;
	}
	else
	{
		m_offsetFromParent = OrgPosition;	// モデル原点からのオフセット
	}
		
	m_initialTranstormInv = Matrix::MakeTranslation(-OrgPosition);
}

//==============================================================================
// ModelCore
//==============================================================================

//------------------------------------------------------------------------------
PmxSkinnedMeshResource::~PmxSkinnedMeshResource()
{
}

//------------------------------------------------------------------------------
void PmxSkinnedMeshResource::RefreshInitialValues()
{
	for (RefPtr<PmxBoneResource>& b : bones)
	{
		b->RefreshInitialValues();
	}
}

LN_NAMESPACE_END
