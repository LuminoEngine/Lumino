
#pragma once
#include "Common.hpp"

namespace ln {

class MeshTilemapLayer
    : public Object
{
    LN_OBJECT;
public:
    void resize(int sizeX, int sizeY, int sizeZ);
    void setTileId(int x, int y, int z, MeshTileFaceDirection dir, int id);

LN_CONSTRUCT_ACCESS:
    MeshTilemapLayer();
    void init();

private:

    bool isValidIndex(int x, int y, int z) const { return (0 <= x && x < m_sizeX) && (0 <= y && y < m_sizeY) && (0 <= z && z < m_sizeZ); }
    detail::MeshTile& tile(int x, int y, int z) { return m_tiles[((m_sizeX * m_sizeY) * z) + (m_sizeX * y) + x]; }

    void draw(RenderingContext* context, const MeshTileset* tileset);

    List<detail::MeshTile> m_tiles;
    int m_sizeX;
    int m_sizeY;
    int m_sizeZ;

    friend class MeshTilemapModel;
};

} // namespace ln
