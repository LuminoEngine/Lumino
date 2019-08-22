
#pragma once
#include "Common.hpp"

namespace ln {
class RenderingContext;
class Tileset;
class AbstractTilemapLayer;

// 基本タイルは、横8タイル、縦最大 ‭8191‬ タイルまで。最大 ID は 65535.
// ただし、現実的には 4096px 程度に抑えてほしいので、16x16 タイルを使ったとしても、最大は 8 * 256 = 2048 個。
// AutoTile は展開すると32, 余裕見て 64、坂道分も作ると考えると + 20。128 個をワンセットにしてみる。
// それにアニメーションを考慮して、AutoTile ひとつ分の最大オフセットは 1024.
// Tileset span 65536 でも十分足りそう。
// 0~163843 を通常タイル、‬16384~32767 をAutoTile, 32768~65535 はReserved にしてみる。
// - UE4 にならって、左上を 0,0 とする。
class TilemapModel
	: public Object
{
    LN_OBJECT;
public:
    static Ref<TilemapModel> create();

    Tileset* tileset() const;
    void addTileset(Tileset* tileset);
    void addLayer(AbstractTilemapLayer* layer);
    AbstractTilemapLayer* layer(int index) const;

	bool isValidTile(int x, int y) const;

public: // TODO: internal
    void render(RenderingContext* context, const Matrix& transform, const detail::TilemapBounds& bounds);
    bool fetchTileset(int tileGlobalId, Tileset** outTileset, int* outTileLocalId);

protected:
	LN_SERIALIZE_CLASS_VERSION(1);
	virtual void serialize(Archive& ar) override;

LN_CONSTRUCT_ACCESS:
    TilemapModel();
	virtual ~TilemapModel();
	void init();
    void init(const StringRef& filePath);

private:
    //struct TilesetSlot
    //{
    //    Ref<Tileset> tileset;
    //    int startId;
    //};

    List<Ref<Tileset>> m_tilesets;
    List<Ref<AbstractTilemapLayer>> m_layers;
    int m_tilesetIdSpan;
};

} // namespace ln
