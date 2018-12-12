
#include "Internal.hpp"
#include <LuminoEngine/Tilemap/TilemapLayer.hpp>

namespace ln {

//==============================================================================
// TilemapLayer

TilemapLayer::TilemapLayer()
{
}

TilemapLayer::~TilemapLayer()
{
}

void TilemapLayer::initialize()
{
    Object::initialize();
}

void TilemapLayer::resize(int width, int height)
{
    m_size.set(width, height);
    m_data.resize(width * height);
}

void TilemapLayer::setTileId(int x, int y, int id)
{
    m_data[y * m_size.width + x] = id;
}

int TilemapLayer::tileId(int x, int y) const
{
    return m_data[y * m_size.width + x];
}

} // namespace ln

