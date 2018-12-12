
#pragma once
#include "../Graphics/GeometryStructs.hpp"
#include "Common.hpp"

namespace ln {

class TilemapLayer
	: public Object
{
public:
    void resize(int width, int height);

    void setTileId(int x, int y, int id);
    int tileId(int x, int y) const;


LN_CONSTRUCT_ACCESS:
    TilemapLayer();
	virtual ~TilemapLayer();
	void initialize();
    void initialize(int width, int height);

private:
    List<int> m_data;
    SizeI m_size;
};

} // namespace ln
