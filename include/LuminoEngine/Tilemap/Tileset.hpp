
#pragma once
#include "Common.hpp"
#include "../Graphics/GeometryStructs.hpp"

namespace ln {
struct Size;
class Material;
class RenderingContext;


// 単純にテクスチャの転送元領域を管理するのではなく、
// タイル ID を受け取って「どのように描くか？」を担当する。
class Tileset
	: public Object
{
public:
    static Ref<Tileset> create();

    void setTilePixelSize(int width, int height);

    int tilePixelWidth() const { return m_tilePixelWidth; }
    int tilePixelHeight() const { return m_tilePixelHeight; }

    void setMaterial(Material* material);

public: // TODO: internal
    void drawTile(RenderingContext* context, int tileId, const Vector3& pos, const Size& tileSize);

LN_CONSTRUCT_ACCESS:
    Tileset();
	virtual ~Tileset();
	void init();

private:
    void resetInfo();

    int m_tilePixelWidth;
    int m_tilePixelHeight;
    int m_horizontalTileCount;
    Size m_tileUVSize;
    Ref<Material> m_material;
};

} // namespace ln
