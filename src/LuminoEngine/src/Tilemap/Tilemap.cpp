
#include "Internal.hpp"
#include <LuminoEngine/Tilemap/TilemapComponent.hpp>
#include <LuminoEngine/Tilemap/Tilemap.hpp>

namespace ln {

//==============================================================================
// Tilemap

LN_OBJECT_IMPLEMENT(Tilemap, Object)
{
    context->registerType<Tilemap>({});
}

Ref<Tilemap> Tilemap::create()
{
    return makeObject<Tilemap>();
}

Tilemap::Tilemap()
{
}

Tilemap::~Tilemap()
{
}

void Tilemap::init()
{
    VisualObject::init();
    m_component = makeObject<TilemapComponent>();
    addComponent(m_component);
    setMainVisualComponent(m_component);
}

void Tilemap::setTilemapModel(TilemapModel* tilemapModel)
{
    m_component->setTilemapModel(tilemapModel);
}

} // namespace ln

