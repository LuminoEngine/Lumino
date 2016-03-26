
#pragma once
#include "Common.h"
#include "TileLayer.h"
#include "TileSet.h"

LN_NAMESPACE_BEGIN

/**
	@brief	
*/
class TileMap
	: public tr::ReflectionObject
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
	TileMap();
	virtual ~TileMap();
	void CreateCore();


	TileSet* GetTileSet();
	void SetTileSet(TileSet* tileSet);
	TileSet* GetTileSet() const { return m_tileSet; }

	tr::ReflectionObjectList<TileLayer*>* GetLayers() { return &m_layers; }

private:
	TileSet*								m_tileSet;
	tr::ReflectionObjectList<TileLayer*>	m_layers;
};

LN_NAMESPACE_END
