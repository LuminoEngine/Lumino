
#pragma once
#include "Common.hpp"

namespace ln {
class RenderingContext;
class Tileset;
class AbstractTilemapLayer;

class TilemapModel
	: public Object
{
public:
    static Ref<TilemapModel> create();

    void addTileset(Tileset* tileset);
    void addLayer(AbstractTilemapLayer* layer);

public: // TODO: internal
    void render(RenderingContext* context, const Matrix& transform, const detail::TilemapBounds& bounds);
    bool fetchTileset(int tileGlobalId, Tileset** outTileset, int* outTileLocalId);

LN_CONSTRUCT_ACCESS:
    TilemapModel();
	virtual ~TilemapModel();
	void init();
    void init(const StringRef& filePath);

private:
    struct TilesetSlot
    {
        Ref<Tileset> tileset;
        int startId;
    };

    List<TilesetSlot> m_tilesets;
    List<Ref<AbstractTilemapLayer>> m_layers;
    int m_tilesetIdSpan;
};

} // namespace ln
