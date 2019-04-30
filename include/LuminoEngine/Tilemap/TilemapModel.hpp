
#pragma once
#include "Common.hpp"

namespace ln {
class RenderingContext;
class Tileset;
class TilemapLayer;

class TilemapModel
	: public Object
{
public:
    static Ref<TilemapModel> create();

    void addTileset(Tileset* tileset);
    void addLayer(TilemapLayer* layer);

public: // TODO: internal
    void render(RenderingContext* context, const Matrix& transform, const detail::TilemapBounds& bounds);
    bool fetchTileset(int tileGlobalId, Tileset** outTileset, int* outTileLocalId);

LN_CONSTRUCT_ACCESS:
    TilemapModel();
	virtual ~TilemapModel();
	void init();

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
