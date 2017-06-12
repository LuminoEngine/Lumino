
#include "../Internal.h"
#include <Lumino/TileMap/TileSet.h>
#include <Lumino/TileMap/TileMapRenderer.h>
#include <Lumino/TileMap/TileLayer.h>

LN_NAMESPACE_BEGIN

//==============================================================================
// TileLayer
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(TileLayer, tr::ReflectionObject);

//------------------------------------------------------------------------------
TileLayerPtr TileLayer::Create()
{
	TileLayerPtr ptr = TileLayerPtr::MakeRef();
	return ptr;
}

//------------------------------------------------------------------------------
TileLayer::TileLayer()
{
}

//------------------------------------------------------------------------------
TileLayer::~TileLayer()
{
}

//------------------------------------------------------------------------------
void TileLayer::resize(int width, int height)
{
	m_size.Set(width, height);
	m_data.resize(width * height);
}

//------------------------------------------------------------------------------
void TileLayer::SetTileId(int x, int y, int id)
{
	m_data[y * m_size.width + x] = id;
}
	
//------------------------------------------------------------------------------
int TileLayer::GetTileId(int x, int y)
{
	return m_data[y * m_size.width + x];
}


LN_NAMESPACE_END
