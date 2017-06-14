
#include "../Internal.h"
#include "PmxSkinnedMesh.h"

LN_NAMESPACE_BEGIN


//==============================================================================
// PmxMaterialResource
//==============================================================================

//------------------------------------------------------------------------------
RefPtr<Material> PmxMaterialResource::MakeCommonMaterial() const
{
	auto m = RefPtr<Material>::makeRef();
	m->setBuiltinColorParameter(
		Material::DiffuseParameter,
		Diffuse.r,
		Diffuse.g,
		Diffuse.b,
		Diffuse.a);
	m->setBuiltinColorParameter(
		Material::AmbientParameter,
		Ambient.r,
		Ambient.g,
		Ambient.b,
		Ambient.a);
	m->setBuiltinColorParameter(
		Material::SpecularParameter,
		Specular.r,
		Specular.g,
		Specular.b,
		Specular.a);
	m->setBuiltinColorParameter(
		Material::EmissiveParameter,
		Emissive.r,
		Emissive.g,
		Emissive.b,
		Emissive.a);
	m->setBuiltinFloatParameter(
		Material::PowerParameter,
		Power);
	m->setBuiltinTextureParameter(
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
	for (RefPtr<PmxBoneResource>& b : bones)
	{
		b->refreshInitialValues();
	}
}

LN_NAMESPACE_END
