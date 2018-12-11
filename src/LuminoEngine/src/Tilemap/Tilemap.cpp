
#include "Internal.hpp"
#include <LuminoEngine/Tilemap/TilemapComponent.hpp>
#include <LuminoEngine/Tilemap/Tilemap.hpp>

namespace ln {

//==============================================================================
// Tilemap

Tilemap::Tilemap()
{
}

Tilemap::~Tilemap()
{
}

void Tilemap::initialize()
{
    VisualObject::initialize();
    m_component = newObject<TilemapComponent>();
    addComponent(m_component);
    setMainVisualComponent(m_component);
}

} // namespace ln

