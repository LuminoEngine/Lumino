
#include "../Internal.h"
#include <Lumino/Graphics/Texture.h>
#include "MmdMaterial.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// MmdMaterialInstance
//==============================================================================

//------------------------------------------------------------------------------
MmdMaterialInstance::MmdMaterialInstance()
	: Material2(detail::MmdMaterialTypeId)
	, m_toonTexture(nullptr)
	, m_sphereTexture(nullptr)

{
	m_diffuse.Set(1.0f, 1.0f, 1.0f, 1.0f);
	m_ambient.Set(0.0f, 0.0f, 0.0f, 0.0f);
	m_specular.Set(0.5f, 0.5f, 0.5f, 0.5f);
	m_emissive.Set(0.0f, 0.0f, 0.0f, 0.0f);
	m_power = 50.0f;
	ToonColor.Set(1.0f, 1.0f, 1.0f, 1.0f);
	EdgeColor.Set(0.0f, 0.0f, 0.0f, 1.0f);
	EdgeSize = 0.0f;
	SphereMode = Material::SphereMode_Disable;
	DrawingFlags = 0;
}

//------------------------------------------------------------------------------
MmdMaterialInstance::~MmdMaterialInstance()
{
	LN_SAFE_RELEASE(m_toonTexture);
	LN_SAFE_RELEASE(m_sphereTexture);
}

//------------------------------------------------------------------------------
void MmdMaterialInstance::OnCombine(Material3* owner, Material3* parent)
{
	m_diffuse = owner->GetColor(_T("Diffuse"), Color(1.0f, 1.0f, 1.0f, 1.0f));
	m_ambient = owner->GetColor(_T("Ambient"), Color(0.0f, 0.0f, 0.0f, 0.0f));
	m_specular = owner->GetColor(_T("Specular"), Color(0.5f, 0.5f, 0.5f, 0.5f));
	m_emissive = owner->GetColor(_T("Emissive"), Color(0.0f, 0.0f, 0.0f, 0.0f));
	m_power = owner->GetFloat(_T("Power"), 50.0f);

	m_toonTexture = owner->GetTexture(_T("ToonTexture"), nullptr);
	m_sphereTexture = owner->GetTexture(_T("SphereTexture"), nullptr);

	ToonColor = owner->GetColor(_T("ToonColor"), Color(1.0f, 1.0f, 1.0f, 1.0f));
	EdgeColor = owner->GetColor(_T("EdgeColor"), Color(0.0f, 0.0f, 0.0f, 1.0f));
	EdgeSize = owner->GetFloat(_T("EdgeSize"), 0.0f);
	TextureCoe = owner->GetColor(_T("TextureCoe"), Color(1.0f, 1.0f, 1.0f, 1.0f));
	SphereTextureCoe = owner->GetColor(_T("SphereTextureCoe"), Color(1.0f, 1.0f, 1.0f, 1.0f));
	ToonTextureCoe = owner->GetColor(_T("ToonTextureCoe"), Color(1.0f, 1.0f, 1.0f, 1.0f));
	DrawingFlags = owner->GetInt(_T("DrawingFlags"), 0);
	SphereMode = owner->GetInt(_T("SphereMode"), 0);
}

////------------------------------------------------------------------------------
//void MmdMaterial::SetToonTexture(Texture* texture)
//{
//	LN_REFOBJ_SET(m_toonTexture, texture);
//}
//
////------------------------------------------------------------------------------
//void MmdMaterial::SetSphereTexture(Texture* texture)
//{
//	LN_REFOBJ_SET(m_sphereTexture, texture);
//}
//
LN_NAMESPACE_END


