
#include "Internal.hpp"
#include "../Rendering/RenderStage.hpp"
#include <LuminoEngine/Animation/AnimationContext.hpp>
#include <LuminoEngine/Physics/PhysicsWorld.hpp>
#include <LuminoEngine/Physics/PhysicsWorld2D.hpp>
#include <LuminoEngine/Effect/EffectContext.hpp>
#include <LuminoEngine/Scene/Component.hpp>
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
	: m_rootWorldObjectList(makeList<Ref<WorldObject>>())
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
    if (LN_REQUIRE(obj)) return;
    if (LN_REQUIRE(!obj->m_world)) return;
    m_rootWorldObjectList->add(obj);
	obj->attachWorld(this);
}

void World::removeAllObjects()
{
    for (int i = m_rootWorldObjectList->size() - 1; i >= 0; i--)
    {
		auto& obj = m_rootWorldObjectList[i];
        if (!obj->isSpecialObject())
        {
			if (obj->destroyed()) {
				m_destroyList.remove(obj);
			}

			obj->detachWorld();
            m_rootWorldObjectList->removeAt(i);
        }
    }
}

ReadOnlyList<Ref<WorldObject>>* World::rootObjects() const
{
	return m_rootWorldObjectList;
}

// Multi-Lang 対応のため、テンプレートではなく基本は TypeInfo で検索する
WorldObject* World::findObjectByComponentType(const TypeInfo* type) const
{
    class LocalVisitor : public detail::IWorldObjectVisitor
    {
    public:
        const TypeInfo* type;
        WorldObject* result = nullptr;
        virtual bool visit(WorldObject* obj)
        {
            for (auto& component : obj->m_components) {
                if (TypeInfo::getTypeInfo(component) == type) {
                    result = obj;
                    return false;
                }
            }
            return true;
        }
    } visitor;
    visitor.type = type;

    for (auto& obj : m_rootWorldObjectList) {
        if (!obj->traverse(&visitor)) {
            break;
        }
    }

    return visitor.result;
}

void World::removeRootObject(WorldObject* obj)
{
	if (m_rootWorldObjectList->remove(obj)) {
		if (obj->destroyed()) {
			m_destroyList.remove(obj);
		}
		obj->detachWorld();
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

    if (m_physicsWorld) {
        m_physicsWorld->stepSimulation(elapsedSeconds);
    }
	if (m_physicsWorld2D) {
		m_physicsWorld2D->stepSimulation(elapsedSeconds);
	}
}

void World::onUpdate(float elapsedSeconds)
{
    // onUpdate 内で新しい WorldObject が作成され、m_rootWorldObjectList に add される場合に備えて
    // イテレータによる列挙は行わない。新しく追加されたものは、このループで
    // 今のフレーム中の最初の onUpdate が呼ばれる。
    for (int i = 0; i < m_rootWorldObjectList->size(); i++) {
        m_rootWorldObjectList[i]->updateFrame(elapsedSeconds);
    }
}

void World::onInternalAnimationUpdate(float elapsedSeconds)
{
}

void World::onPostUpdate(float elapsedSeconds)
{

	for (WorldObject* obj : m_destroyList) {
		//obj->removeFromWorld();

		obj->detachWorld();
		m_rootWorldObjectList->remove(obj);
	}
	m_destroyList.clear();
}

void World::serialize(Archive& ar)
{
    Object::serialize(ar);

    ar & ln::makeNVP(u"Children", *m_rootWorldObjectList);

    if (ar.isLoading()) {
        for (auto& obj : m_rootWorldObjectList) {
            obj->attachWorld(this);
        }
    }
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

