
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

	LN_SERIALIZE_CLASS_VERSION(1);
	virtual void serialize(Archive& ar) override;

public: // TODO: internal
    void render(TilemapModel* model, RenderingContext* context, const Matrix& transform, const detail::TilemapBounds& bounds);

LN_CONSTRUCT_ACCESS:
    TilemapLayer();
    virtual ~TilemapLayer();
    void init();
    void init(int width, int height);

private:
	Size m_tileSize;
	TilemapOrientation m_orientation;
	int m_width;
	int m_height;
	List<int> m_data;
};

} // namespace ln
