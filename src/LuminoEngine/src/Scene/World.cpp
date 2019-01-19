
#include "Internal.hpp"
#include "../Rendering/RenderStage.hpp"
#include <LuminoEngine/Animation/AnimationContext.hpp>
#include <LuminoEngine/Physics/PhysicsWorld.hpp>
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

void World::init()
{
    Object::init();
    m_animationContext = newObject<AnimationContext>();
    m_physicsWorld = newObject<PhysicsWorld>();
    m_renderingContext = makeRef<detail::WorldSceneGraphRenderingContext>();
}

void World::dispose()
{
    removeAllObjects();
    m_renderingContext.reset();
    m_physicsWorld.reset();
    m_animationContext.reset();
    Object::dispose();
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

void World::removeAllObjects()
{
    for (int i = m_rootWorldObjectList.size() - 1; i >= 0; i--)
    {
        if (!m_rootWorldObjectList[i]->isSpecialObject())
        {
            m_rootWorldObjectList[i]->m_world = nullptr;
            m_rootWorldObjectList.removeAt(i);
        }
    }
}

void World::updateObjectsWorldMatrix()
{
    for (auto& obj : m_rootWorldObjectList)
    {
        obj->updateWorldMatrixHierarchical();
    }
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
    // Physics モジュールの Component が、WorldObject の WorldMatrix を元にシミュレーション前準備を行うことがあるので
    // ここで WorldMatrix を更新しておく。
    updateObjectsWorldMatrix();

    if (m_physicsWorld != nullptr)
    {
        m_physicsWorld->stepSimulation(elapsedSeconds);
    }
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

detail::WorldSceneGraphRenderingContext* World::prepareRender(RenderViewPoint* viewPoint)
{
	m_renderingContext->resetForBeginRendering();
	m_renderingContext->setViewPoint(viewPoint);
	return m_renderingContext;
}

void World::renderObjects()
{
    for (auto& obj : m_rootWorldObjectList)
    {
        obj->render();

        for (auto& c : obj->m_components)
        {

            c->onPrepareRender(m_renderingContext); // TODO: 全体の前にした方がいいかも
            c->render(m_renderingContext);
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

void WorldSceneGraphRenderingContext::resetForBeginRendering()
{
	RenderingContext::resetForBeginRendering();
	m_elementList->clear();
}

} // namespace detail

} // namespace ln

