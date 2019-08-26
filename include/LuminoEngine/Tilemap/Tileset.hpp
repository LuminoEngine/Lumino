
#pragma once
#include "Common.hpp"
#include "../Graphics/GeometryStructs.hpp"

namespace ln {
struct Size;
class Material;
class RenderingContext;

enum class TileCollisionType : uint8_t
{
	PassageFlags = 0,
};


// 単純にテクスチャの転送元領域を管理するのではなく、
// タイル ID を受け取って「どのように描くか？」を担当する。
class Tileset
	: public Object
{
    LN_OBJECT;
public:
    static Ref<Tileset> create();

    // util
    void reset(Material* material, int tilePixelWidth, int tilePixelHeight);

    void resize(int tileCount);
    void setTilePixelSize(int width, int height);
    void setTileImageRect(int tileId, int x, int y, int width, int height);


	TileCollisionType tileCollisionType(int tileId) const { return m_tiles[tileId].collisionType; }
	void setTilePassageFlags(int tileId, uint8_t value) { m_tiles[tileId].passageFlags = value; }
	uint8_t tilePassageFlags(int tileId) const { return m_tiles[tileId].passageFlags; }

    int tilePixelWidth() const { return m_tilePixelWidth; }
    int tilePixelHeight() const { return m_tilePixelHeight; }
    Material* material() const;

    void setMaterial(Material* material);

public: // TODO: internal
    void drawTile(RenderingContext* context, int tileId, const Vector3& pos, const Size& tileSize);
    virtual void onSetAssetFilePath(const Path& filePath) { m_assetFilePath = filePath; }

    Path m_assetFilePath;

protected:
    LN_SERIALIZE_CLASS_VERSION(1);
    virtual void serialize(Archive& ar) override;

LN_CONSTRUCT_ACCESS:
    Tileset();
	virtual ~Tileset();
	void init();

private:
    struct Tile
    {
		TileCollisionType collisionType = TileCollisionType::PassageFlags;
		uint8_t passageFlags = 0x00;

        Rect sourceRect;    // unit: px


        void serialize(Archive& ar);
    };

    void resetInfo();

    int m_tilePixelWidth;
    int m_tilePixelHeight;
    //int m_horizontalTileCount;
    //Size m_tileUVSize;
    Vector2 m_tileScale;
    Ref<Material> m_material;
    List<Tile> m_tiles;
};

} // namespace ln
