
#include "../Internal.h"
#include <Lumino/TileMap/TileMapRenderer.h>
#include <Lumino/TileMap/TileMapModel.h>

LN_NAMESPACE_BEGIN

//==============================================================================
// TileMapModel
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(TileMapModel, tr::ReflectionObject);

//------------------------------------------------------------------------------
TileMapModelPtr TileMapModel::create()
{
	auto ptr = TileMapModelPtr::makeRef();
	return ptr;
}

//------------------------------------------------------------------------------
TileMapModel::TileMapModel()
	: m_tileSet(nullptr)
{
}

//------------------------------------------------------------------------------
TileMapModel::~TileMapModel()
{
	LN_SAFE_RELEASE(m_tileSet);
}

//------------------------------------------------------------------------------
TileSet* TileMapModel::GetTileSet()
{
	return m_tileSet;
}

//------------------------------------------------------------------------------
void TileMapModel::SetTileSet(TileSet* tileSet)
{
	LN_REFOBJ_SET(m_tileSet, tileSet);
}

LN_NAMESPACE_END
