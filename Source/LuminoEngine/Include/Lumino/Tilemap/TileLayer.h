
#pragma once
#include "Common.h"

LN_NAMESPACE_BEGIN

class TileLayer;
typedef Ref<TileLayer> TileLayerPtr;

/**
	@brief	
*/
class TileLayer
	: public tr::ReflectionObject
{
	LN_OBJECT();
public:
	static TileLayerPtr create();

	TileLayer();
	virtual ~TileLayer();
	
	void resize(int width, int height);
	const SizeI& getSize() const { return m_size; }

	void SetTileId(int x, int y, int id);
	int GetTileId(int x, int y);

LN_INTERNAL_ACCESS:

private:
	List<int>	m_data;		// マップデータ本体。m_size.X*m_size.Y のサイズを持つ。
	SizeI		m_size;
};

LN_NAMESPACE_END
