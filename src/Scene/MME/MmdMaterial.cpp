
#include "../Internal.h"
#include <Lumino/Graphics/Texture.h>
#include "MmdMaterial.h"

LN_NAMESPACE_BEGIN

//=============================================================================
// MmdMaterial
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MmdMaterial::MmdMaterial()
	: m_toonTexture(nullptr)
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

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MmdMaterial::~MmdMaterial()
{
	LN_SAFE_RELEASE(m_toonTexture);
	LN_SAFE_RELEASE(m_sphereTexture);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void MmdMaterial::SetToonTexture(Texture* texture)
{
	LN_REFOBJ_SET(m_toonTexture, texture);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void MmdMaterial::SetSphereTexture(Texture* texture)
{
	LN_REFOBJ_SET(m_sphereTexture, texture);
}

LN_NAMESPACE_END


