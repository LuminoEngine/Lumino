
#include "../Internal.h"
#include <Lumino/TileMap/TileMapRenderer.h>
#include <Lumino/TileMap/TileMapModel.h>

LN_NAMESPACE_BEGIN

//==============================================================================
// TileMap
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(TileMap, tr::ReflectionObject);

//------------------------------------------------------------------------------
TileMap::TileMap()
	: m_tileSet(nullptr)
{
}

//------------------------------------------------------------------------------
TileMap::~TileMap()
{
	LN_SAFE_RELEASE(m_tileSet);
}

//------------------------------------------------------------------------------
TileSet* TileMap::GetTileSet()
{
	return m_tileSet;
}

//------------------------------------------------------------------------------
void TileMap::SetTileSet(TileSet* tileSet)
{
	LN_REFOBJ_SET(m_tileSet, tileSet);
}

LN_NAMESPACE_END
