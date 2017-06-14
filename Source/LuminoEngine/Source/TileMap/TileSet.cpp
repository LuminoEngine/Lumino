
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
TileSetPtr TileSet::create()
{
	TileSetPtr ptr = TileSetPtr::makeRef();
	ptr->initialize(detail::GraphicsManager::getInstance());
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
void TileSet::initialize(detail::GraphicsManager* manager)
{
	m_material = Object::makeRef<Material>();
	// TODO:自己照明
	m_material->setBuiltinColorParameter(Material::AmbientParameter, Color::White);
}

//------------------------------------------------------------------------------
void TileSet::SetImageSource(Texture* texture)
{
	LN_REFOBJ_SET(m_imageSource, texture);
	m_material->setMaterialTexture(texture);
}

//------------------------------------------------------------------------------
Texture* TileSet::GetImageSource() const
{
	return m_imageSource;
}

//------------------------------------------------------------------------------
void TileSet::LookupTileImage(int id, Texture** outTexture, RectI* outSrcRect)
{
	int x = id % m_tileCountH;
	int y = id / m_tileCountH;
	outSrcRect->set(x * m_tileSize.width, y * m_tileSize.height, m_tileSize.width, m_tileSize.height);
	*outTexture = m_imageSource;
}

//------------------------------------------------------------------------------
Material* TileSet::getMaterial() const
{
	return m_material;
}

LN_NAMESPACE_END
