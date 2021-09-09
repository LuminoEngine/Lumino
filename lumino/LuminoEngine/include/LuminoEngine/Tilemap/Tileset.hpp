
#pragma once
#include "Common.hpp"
#include "../Graphics/GeometryStructs.hpp"

namespace ln {
struct Size;
class Texture;
class RenderingContext;

enum class TileCollisionType : uint8_t
{
	PassageFlags = 0,
};

class AutoTileset
	: public Object
{
public:
	Ref<Material> material;

	// autotileId: 0~AutoTileUnitStride
	// component: [top-left][top-right][bottom-left][bottom-light]
	Rect getSourceRectUV(int autotileId, int component) const;
	
LN_CONSTRUCT_ACCESS:
    AutoTileset();
	void init();
};

// 単純にテクスチャの転送元領域を管理するのではなく、
// タイル ID を受け取って「どのように描くか？」を担当する。
class Tileset
	: public Object
{
    LN_OBJECT;
public:
	static const int AutoTileOffset = 16384;
	static const int AutoTileSetStride = 1024;
	static const int AutoTileUnitStride = 128;
	static const int MaxAutoTilests = AutoTileOffset / AutoTileSetStride;
	static const int LocalAutoTileIdSlopeOffset = 96;

    static Ref<Tileset> create();

	static Ref<Tileset> create(Texture* texture, int tilePixelWidth, int tilePixelHeight);

    // util
    void reset(Material* material, int tilePixelWidth, int tilePixelHeight);

    void resize(int tileCount);
    void setTilePixelSize(int width, int height);
    void setTileImageRect(int tileId, int x, int y, int width, int height);


	TileCollisionType tileCollisionType(int tileId) const { return m_tiles[tileId].collisionType; }
	void setTilePassageFlags(int tileId, uint8_t value) { m_tiles[tileId].passageFlags = value; }
	uint8_t tilePassageFlags(int tileId) const;

    int tilePixelWidth() const { return m_tilePixelWidth; }
    int tilePixelHeight() const { return m_tilePixelHeight; }
    Material* material() const;

    void setMaterial(Material* material);

	static int autoTileIndex(int tileId) { return (tileId < Tileset::AutoTileOffset) ? -1 : ((tileId - Tileset::AutoTileOffset) / Tileset::AutoTileSetStride); }
	static int autoTileLocalId(int tileId) { return (tileId < Tileset::AutoTileOffset) ? -1 : tileId % Tileset::AutoTileUnitStride; }
	static int isBlockAutoTile(int localAutoTileId) { return 0 <= localAutoTileId && localAutoTileId < 96; }
	static int isSlopeAutoTile(int localAutoTileId) { return LocalAutoTileIdSlopeOffset <= localAutoTileId && localAutoTileId < 104; }
	static int isHalfSlopeAutoTile(int localAutoTileId) { return 104 <= localAutoTileId && localAutoTileId < 112; }
	static int isRootHalfSlopeAutoTile(int localAutoTileId) { return isHalfSlopeAutoTile(localAutoTileId) && (localAutoTileId & 1) == 0; }
	static int isTipHalfSlopeAutoTile(int localAutoTileId) { return isHalfSlopeAutoTile(localAutoTileId) && (localAutoTileId & 1) == 1; }

	void addAutoTileset(AutoTileset* autoTileset);


public: // TODO: internal
    void drawTile(RenderingContext* context, int tileId, const Vector3& pos, const Size& tileSize);
    //virtual void onSetAssetFilePath(const Path& filePath) { m_assetFilePath = filePath; }

    Path m_assetFilePath;

protected:

LN_CONSTRUCT_ACCESS:
    Tileset();
	virtual ~Tileset();
	void init();

private:
    struct Tile
    {
		TileCollisionType collisionType = TileCollisionType::PassageFlags;

        // bit が ON である方向へは移動できない。下位から、↓、←、→、↑。NumPad の順。
        // 進入不可能地形は 下位が 0xF.
        // このタイルの移動判定は無視し、下レイヤーの flag を使う場合は 0x10 で論理積する。
		uint8_t passageFlags = 0x00;

        Rect sourceRect;    // unit: px

        void serialize(Archive& ar);
    };



    void resetInfo();
	//const Ref<AutoTileset>& getAutoTileset(int tileId) const { return  }

    int m_tilePixelWidth;	// TODO: float にしていいかも。描画でしかつかわないので
    int m_tilePixelHeight;
    //int m_horizontalTileCount;
    //Size m_tileUVSize;
    Vector2 m_tileScale;
    Ref<Material> m_material;
    List<Tile> m_tiles;
	List<Ref<AutoTileset>> m_autoTilesets;
};

namespace detail {

struct AutoTileInfo
{
	uint8_t subtiles[4];	// [top-left][top-right][bottom-left][bottom-light]
};

extern AutoTileInfo g_AutoTileTable[Tileset::AutoTileUnitStride];

} // namespace detail
} // namespace ln
