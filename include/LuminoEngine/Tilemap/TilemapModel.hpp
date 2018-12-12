
#pragma once
#include "Common.hpp"

namespace ln {
class Tileset;
class TilemapLayer;

class TilemapModel
	: public Object
{
public:
    void addTileset(Tileset* tileset);
    void addLayer(TilemapLayer* layer);


LN_CONSTRUCT_ACCESS:
    TilemapModel();
	virtual ~TilemapModel();
	void initialize();

private:
    struct TilesetSlot
    {
        Ref<Tileset> tileset;
        int startId;
    };

    List<TilesetSlot> m_tilesets;
    List<Ref<TilemapLayer>> m_layers;
    int m_tilesetIdSpan;
};

} // namespace ln
