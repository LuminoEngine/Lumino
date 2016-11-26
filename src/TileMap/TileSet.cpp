
#include "../Internal.h"
#include "../Graphics/GraphicsManager.h"
#include <Lumino/Graphics/Texture.h>
#include <Lumino/Graphics/Material.h>
#include <Lumino/TileMap/TileSet.h>

LN_NAMESPACE_BEGIN

//==============================================================================
// TileSet
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(TileSet, tr::ReflectionObject);

//------------------------------------------------------------------------------
TileSetPtr TileSet::Create()
{
	TileSetPtr ptr = TileSetPtr::MakeRef();
	ptr->Initialize(detail::GraphicsManager::GetInstance());
	return ptr;
}

//------------------------------------------------------------------------------
TileSet::TileSet()
	: m_imageSource(nullptr)
	, m_tileCountH(8)
{
}

//------------------------------------------------------------------------------
TileSet::~TileSet()
{
	LN_SAFE_RELEASE(m_imageSource);
}

//------------------------------------------------------------------------------
void TileSet::Initialize(detail::GraphicsManager* manager)
{
	m_material = Object::MakeRef<Material>();
	// TODO:自己照明
	m_material->SetBuiltinColorParameter(Material::AmbientParameter, Color::White);
}

//------------------------------------------------------------------------------
void TileSet::SetImageSource(Texture* texture)
{
	LN_REFOBJ_SET(m_imageSource, texture);
	m_material->SetMaterialTexture(texture);
}

//------------------------------------------------------------------------------
Texture* TileSet::GetImageSource() const
{
	return m_imageSource;
}

//------------------------------------------------------------------------------
void TileSet::LookupTileImage(int id, Texture** outTexture, Rect* outSrcRect)
{
	int x = id % m_tileCountH;
	int y = id / m_tileCountH;
	outSrcRect->Set(x * m_tileSize.width, y * m_tileSize.height, m_tileSize.width, m_tileSize.height);
	*outTexture = m_imageSource;
}

//------------------------------------------------------------------------------
Material* TileSet::GetMaterial() const
{
	return m_material;
}

LN_NAMESPACE_END
