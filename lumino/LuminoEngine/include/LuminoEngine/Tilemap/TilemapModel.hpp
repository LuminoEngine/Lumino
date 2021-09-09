
#pragma once
#include "Common.hpp"
#include "Tileset.hpp"

namespace ln {
class RenderingContext;
class TilemapLayer;

// 基本タイルは、横8タイル、縦最大 ‭8191‬ タイルまで。最大 ID は 65535.
// ただし、現実的には 4096px 程度に抑えてほしいので、16x16 タイルを使ったとしても、最大は 8 * 256 = 2048 個。
// AutoTile は展開すると32, 余裕見て 64、坂道分も作ると考えると + 20。128 個をワンセットにしてみる。
// それにアニメーションを考慮して、AutoTile ひとつ分の最大オフセットは 1024.
// Tileset span 65536 でも十分足りそう。
// 0~163843 を通常タイル、‬16384~32767 をAutoTile, 32768~65535 はReserved にしてみる。
// - タイルグリッドの座標系は UE4 にならって、左上を 0,0 とする。
class TilemapModel
	: public Object
{
    LN_OBJECT;
public:


    static Ref<TilemapModel> create();

	/** 指定したサイズとレイヤー数で、すべてのタイルを 0 で初期化する。 */
	void reset(int width, int height, int layers);


    Tileset* tileset() const;
    void setTileset(Tileset* tileset);
    void addLayer(TilemapLayer* layer);
	TilemapLayer* layer(int index) const;

	const Size& tileSize() const;
	int width() const;
	int height() const;
	bool isValidTilePosition(int x, int y) const;
	uint8_t tilePassageFlags(int x, int y) const;
    TilemapLayer* getValidFrontLayer(int x, int y) const;


public: // TODO: internal
    void render(RenderingContext* context, const Matrix& transform, const detail::TilemapBounds& bounds);
    bool fetchTileset(int tileGlobalId, Tileset** outTileset, int* outTileLocalId);

protected:

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

    Ref<Tileset> m_tileset;
    List<Ref<TilemapLayer>> m_layers;   // インデックスの低い方が奥。
    int m_tilesetIdSpan;
};

} // namespace ln
