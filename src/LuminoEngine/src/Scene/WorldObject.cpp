
#include "Internal.hpp"
#include <LuminoEngine/Scene/Component.hpp>
#include <LuminoEngine/Scene/WorldObject.hpp>

namespace ln {

//==============================================================================
// WorldObject

WorldObject::WorldObject()
    : m_world(nullptr)
    , m_components(makeRef<List<Ref<Component>>>())
{
}

WorldObject::~WorldObject()
{
}

void WorldObject::initialize()
{
    Object::initialize();
}

void WorldObject::addComponent(Component* component)
{
    component->m_object = this;
    m_components->add(component);
}

void WorldObject::onPreUpdate()
{
}

void WorldObject::onUpdate(float elapsedSeconds)
{

}

void WorldObject::onRender()
{

}

void WorldObject::updateFrame(float elapsedSeconds)
{
    onUpdate(elapsedSeconds);
}

void WorldObject::render()
{
    onRender();
}

} // namespace ln

