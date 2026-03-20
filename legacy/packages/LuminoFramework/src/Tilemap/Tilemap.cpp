#include "Internal.hpp"
#include <LuminoFramework/Tilemap/TilemapComponent.hpp>
#include <LuminoFramework/Tilemap/Tilemap.hpp>

namespace ln {

//==============================================================================
// Tilemap

LN_OBJECT_IMPLEMENT(Tilemap, Object) {}

Ref<Tilemap> Tilemap::create()
{
    return makeObject_deprecated<Tilemap>();
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
    m_component = makeObject_deprecated<TilemapComponent>();
    addComponent(m_component);
    setMainVisualComponent(m_component);
}

void Tilemap::setTilemapModel(TilemapModel* tilemapModel)
{
    m_component->setTilemapModel(tilemapModel);
}

TilemapComponent* Tilemap::tilemapComponent() const
{
	return m_component;
}

} // namespace ln

