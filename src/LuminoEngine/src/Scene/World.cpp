
#include "Internal.hpp"
#include "../Rendering/RenderStage.hpp"
#include <LuminoEngine/Scene/Component.hpp>
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
    m_renderingContext = makeRef<detail::WorldSceneGraphRenderingContext>();
}

void World::addObject(WorldObject* obj)
{
    if (LN_REQUIRE(obj)) return;
    if (LN_REQUIRE(!obj->m_world)) return;
    m_rootWorldObjectList.add(obj);
    obj->m_world = this;
    // TODO: 
    //obj->onAttachedWorld(this);
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
    m_renderingContext->reset();

    for (auto& obj : m_rootWorldObjectList)
    {
        obj->render();

        for (auto& c : obj->m_components)
        {
            c->onRender(m_renderingContext);
        }
    }
}

//==============================================================================
// WorldSceneGraphRenderingContext

namespace detail {

WorldSceneGraphRenderingContext::WorldSceneGraphRenderingContext()
	: m_elementList(makeRef<detail::DrawElementList>(detail::EngineDomain::renderingManager()))
{
	setDrawElementList(m_elementList);
}

void WorldSceneGraphRenderingContext::reset()
{
	RenderingContext::reset();
	m_elementList->clear();
}

} // namespace detail

} // namespace ln

