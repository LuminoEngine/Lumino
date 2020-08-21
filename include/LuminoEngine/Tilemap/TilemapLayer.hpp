
#pragma once
#include "../Graphics/GeometryStructs.hpp"
#include "Common.hpp"

namespace ln {
class RenderingContext;
class TilemapModel;

enum class TilemapOrientation
{
    UpFlow,
    DownFlow,
};

// 
//class AbstractTilemapLayer
//	: public Object
//{
//    LN_OBJECT;
//public:
//
//    //void resize(int width, int height);
//
//    //void setTileId(int x, int y, int id);
//    virtual int getWidth() const = 0;
//    virtual int getHeight() const = 0;
//    virtual int getTileId(int x, int y) const = 0;
//
//    void setTileSize(const Size& size);
//
//    void setOrientation(TilemapOrientation value) { m_orientation = value; }
//
//public: // TODO: internal
//    void render(TilemapModel* model, RenderingContext* context, const Matrix& transform, const detail::TilemapBounds& bounds);
//
//LN_CONSTRUCT_ACCESS:
//    AbstractTilemapLayer();
//	virtual ~AbstractTilemapLayer();
//	void init();
//    //void init(int width, int height);
//
//private:
//    //List<int> m_data;
//    //SizeI m_size;
//    Size m_tileSize;
//    TilemapOrientation m_orientation;
//};

// グリッドタイルのレイヤー。
// Tilemap モジュールとしては、パノラマレイヤーや、表示サイズの異なるタイルはレイヤーでは扱わない。
class TilemapLayer
    : public Object
{
    LN_OBJECT;
public:
    static Ref<TilemapLayer> create();

	void setTileSize(const Size& size);	// TODO: サイズはレイヤー間で共通なので、TilemapModel 側に持たせる
	const Size& tileSize() const { return m_tileSize; }
    void resize(int width, int height);

    void setTileId(int x, int y, int id);
	int tileId(int x, int y);

    int getWidth() const { return m_width; }	// TODO: これらも model 側に持たせる。全体で共通。
    int getHeight() const { return m_height; }
    int getTileId(int x, int y) const;
	int isOutOfRange(int x, int y) const { return (x < 0 || m_width <= x) || (y < 0 || m_height <= y); }
	int isValidRange(int x, int y) const { return !isOutOfRange(x, y); }

    // x, y 位置のタイルId が 0 でない場合 true. 範囲外は false.
    bool isValidTileId(int x, int y) const { return getTileId(x, y) != 0; }



	// 周辺にも適用する
	// autoTilesetId: 0~
	void putAutoTile(int x, int y, int autoTilesetId);

	void putAutoTileDirect(int x, int y, int autoTilesetId, int localAutoTileId);

	// 配置できないこともある。undo 対応のため戻り値で返す。
	bool putAutoTileSlope(int x, int y, int autoTilesetId);

public: // TODO: internal
    void render(TilemapModel* model, RenderingContext* context, const Matrix& transform, const detail::TilemapBounds& bounds);

LN_CONSTRUCT_ACCESS:
    TilemapLayer();
    virtual ~TilemapLayer();
    void init();
    void init(int width, int height);

private:
	enum Corner
	{
		Corner_TL = 0,	// top-left
		Corner_TR = 1,	// top-right
		Corner_BL = 2,	// bottom-left
		Corner_BR = 3,	// bottom-right
		Corner_Invalid = -1,
	};

	enum Edge
	{
		Edge_L = 0,	// left
		Edge_R = 1,	// right
		Edge_T = 2,	// top
		Edge_B = 3,	// bottom
		Edge_Invalid = -1,
	};

	struct AutoTileNearbyInfo
	{
		PointI points[9];
		int tileIds[9];
		int localAutoTileIds[9];
		bool equals[9];

		// [9][TL, TR, BL, BR]
		int halfSlopeTipWeights[9][4];
	};

	void makeAutoTileNearbyInfo(int x, int y, int autoTilesetId, AutoTileNearbyInfo* outInfo) const;
	void refreshAutoTile(int x, int y);

	Size m_tileSize;
	TilemapOrientation m_orientation;
	int m_width;
	int m_height;
	List<int> m_data;
};

} // namespace ln
