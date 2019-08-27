
#include "Internal.hpp"
#include "../Rendering/RenderStage.hpp"
#include <LuminoEngine/Animation/AnimationContext.hpp>
#include <LuminoEngine/Physics/PhysicsWorld.hpp>
#include <LuminoEngine/Physics/PhysicsWorld2D.hpp>
#include <LuminoEngine/Effect/EffectContext.hpp>
#include <LuminoEngine/Scene/Component.hpp>
#include <LuminoEngine/Scene/Scene.hpp>
#include <LuminoEngine/Scene/WorldObject.hpp>
#include <LuminoEngine/Scene/World.hpp>

namespace ln {

//==============================================================================
// World

LN_OBJECT_IMPLEMENT(World, Object)
{
    context->registerType<World>({/*
        makeRef<PropertyInfo>("TimeScale", LN_MAKE_GET_SET_PROPERTY_ACCESSOR(World, float, timeScale, setTimeScale)),*/ });
}

World::World()
	: m_sceneList(makeList<Ref<Scene>>())
    , m_timeScale(1.0f)
{
}

World::~World()
{
}

void World::init()
{
    Object::init();
    m_animationContext = makeObject<AnimationContext>();
    m_physicsWorld = makeObject<PhysicsWorld>();
	m_physicsWorld2D = makeObject<PhysicsWorld2D>();
    m_effectContext = makeObject<EffectContext>();
    m_renderingContext = makeRef<detail::WorldSceneGraphRenderingContext>();
}

void World::onDispose(bool explicitDisposing)
{
    removeAllObjects();
    
    if (m_effectContext) {
        m_effectContext->dispose();
        m_effectContext = nullptr;
    }

    m_renderingContext.reset();
    m_physicsWorld.reset();
	m_physicsWorld2D.reset();
    m_animationContext.reset();
    Object::onDispose(explicitDisposing);
}

void World::addObject(WorldObject* obj)
{
    masterScene()->addObject(obj);
}

// TODO: テスト用。置き場考えておく。
void World::removeAllObjects()
{
    masterScene()->removeAllObjects();
}

ReadOnlyList<Ref<WorldObject>>* World::rootObjects() const
{
	return masterScene()->m_rootWorldObjectList;
}

WorldObject* World::findObjectByComponentType(const TypeInfo* type) const
{
    for (auto& scene : m_sceneList) {
        auto obj = scene->findObjectByComponentType(type);
        if (obj) {
            return obj;
        }
    }

    return nullptr;
}

Scene* World::masterScene() const
{
    if (m_sceneList->isEmpty()) return nullptr;
    return m_sceneList->front();
}

void World::addScene(Scene* scene)
{
	if (LN_REQUIRE(scene)) return;
	if (LN_REQUIRE(!scene->m_world)) return;
	m_sceneList->add(scene);
	scene->m_world = this;
	scene->m_initialUpdate = true;
}

void World::updateObjectsWorldMatrix()
{
    for (auto& scene : m_sceneList) {
        scene->updateObjectsWorldMatrix();
    }
}

void World::updateFrame(float elapsedSeconds)
{
    float t = elapsedSeconds * m_timeScale;
    onPreUpdate(t);
    onInternalPhysicsUpdate(t);
    onUpdate(t);
    onInternalAnimationUpdate(t);
    onPostUpdate(t);
}

void World::onPreUpdate(float elapsedSeconds)
{
    m_effectContext->update(elapsedSeconds);

    for (auto& scene : m_sceneList) {
        scene->onPreUpdate(elapsedSeconds);
    }
}

void World::onInternalPhysicsUpdate(float elapsedSeconds)
{
    // Physics モジュールの Component が、WorldObject の WorldMatrix を元にシミュレーション前準備を行うことがあるので
    // ここで WorldMatrix を更新しておく。
    updateObjectsWorldMatrix();

    if (m_physicsWorld) {
        m_physicsWorld->stepSimulation(elapsedSeconds);
    }
	if (m_physicsWorld2D) {
		m_physicsWorld2D->stepSimulation(elapsedSeconds);
	}
}

void World::onUpdate(float elapsedSeconds)
{
    for (auto& scene : m_sceneList) {
        scene->update(elapsedSeconds);
    }
}

void World::onInternalAnimationUpdate(float elapsedSeconds)
{
}

void World::onPostUpdate(float elapsedSeconds)
{
    for (auto& scene : m_sceneList) {
        scene->onPostUpdate(elapsedSeconds);
    }
}

//void World::serialize(Archive& ar)
//{
//    Object::serialize(ar);
//
//    ar & ln::makeNVP(u"Children", *m_rootWorldObjectList);
//
//    if (ar.isLoading()) {
//        for (auto& obj : m_rootWorldObjectList) {
//            obj->attachWorld(this);
//        }
//    }
//}

detail::WorldSceneGraphRenderingContext* World::prepareRender(RenderViewPoint* viewPoint)
{
	m_renderingContext->resetForBeginRendering();
	m_renderingContext->setViewPoint(viewPoint);
	return m_renderingContext;
}

void World::renderObjects()
{
    for (auto& scene : m_sceneList) {
        scene->renderObjects(m_renderingContext);
    }

    m_renderingContext->pushState(true);
    m_effectContext->render(m_renderingContext);
    m_renderingContext->popState();
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

