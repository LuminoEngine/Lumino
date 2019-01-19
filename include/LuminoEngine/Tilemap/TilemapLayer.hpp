
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

class TilemapLayer
	: public Object
{
public:
    static Ref<TilemapLayer> create();

    void resize(int width, int height);

    void setTileId(int x, int y, int id);
    int tileId(int x, int y) const;

    void setTileSize(const Size& size);

    void setOrientation(TilemapOrientation value) { m_orientation = value; }

public: // TODO: internal
    void render(TilemapModel* model, RenderingContext* context, const Matrix& transform, const detail::TilemapBounds& bounds);

LN_CONSTRUCT_ACCESS:
    TilemapLayer();
	virtual ~TilemapLayer();
	void init();
    void init(int width, int height);

private:
    List<int> m_data;
    SizeI m_size;
    Size m_tileSize;
    TilemapOrientation m_orientation;
};

} // namespace ln
