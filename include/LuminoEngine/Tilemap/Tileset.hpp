
#pragma once
#include "Common.hpp"

namespace ln {
struct Size;
class Material;
class RenderingContext;

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
    int m_tilePixelWidth;
    int m_tilePixelHeight;
    Ref<Material> m_material;
};

} // namespace ln
