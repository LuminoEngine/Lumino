
#include "Internal.hpp"
#include <LuminoEngine/Scene/WorldObject.hpp>
#include <LuminoEngine/Scene/World.hpp>

namespace ln {

//==============================================================================
// World

World::World()
{
}

World::~World()
{
}

void World::initialize()
{
    Object::initialize();
}

void World::updateFrame(float elapsedSeconds)
{
    onPreUpdate(elapsedSeconds);
    onInternalPhysicsUpdate(elapsedSeconds);
    onUpdate(elapsedSeconds);
    onInternalAnimationUpdate(elapsedSeconds);
    onPostUpdate(elapsedSeconds);
}

void World::onPreUpdate(float elapsedSeconds)
{
    for (auto& obj : m_rootWorldObjectList)
    {
        obj->onPreUpdate();
    }
}

void World::onInternalPhysicsUpdate(float elapsedSeconds)
{
}

void World::onUpdate(float elapsedSeconds)
{
    for (auto& obj : m_rootWorldObjectList)
    {
        obj->updateFrame(elapsedSeconds);
    }
}

void World::onInternalAnimationUpdate(float elapsedSeconds)
{
}

void World::onPostUpdate(float elapsedSeconds)
{
}

void World::render()
{
    for (auto& obj : m_rootWorldObjectList)
    {
        obj->render();
    }
}

} // namespace ln

