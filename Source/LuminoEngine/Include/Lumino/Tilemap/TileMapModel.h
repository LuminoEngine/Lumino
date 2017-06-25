
#pragma once
#include "Common.h"
#include "TileLayer.h"
#include "TileSet.h"

LN_NAMESPACE_BEGIN
class TileMapModel;
using TileMapModelPtr = RefPtr<TileMapModel>;

/**
	@brief	
*/
class TileMapModel
	: public tr::ReflectionObject
{
	LN_OBJECT();

public:
	static TileMapModelPtr create();

public:
	TileMapModel();
	virtual ~TileMapModel();


	TileSet* GetTileSet();
	void SetTileSet(TileSet* tileSet);
	TileSet* GetTileSet() const { return m_tileSet; }

	tr::ReflectionObjectList<TileLayer*>* GetLayers() { return &m_layers; }

private:
	TileSet*								m_tileSet;
	tr::ReflectionObjectList<TileLayer*>	m_layers;
};

LN_NAMESPACE_END
