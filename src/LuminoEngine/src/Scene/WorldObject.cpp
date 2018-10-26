
#include "Internal.hpp"
#include <LuminoEngine/Scene/WorldObject.hpp>

namespace ln {

//==============================================================================
// WorldObject

WorldObject::WorldObject()
{
}

WorldObject::~WorldObject()
{
}

void WorldObject::initialize()
{
    Object::initialize();
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

