
#include "Internal.hpp"
#include <LuminoEngine/Animation/AnimationContext.hpp>
#include <LuminoFramework/Physics/PhysicsWorld.hpp>
#include <LuminoFramework/Physics/PhysicsWorld2D.hpp>
#include <LuminoFramework/Effect/EffectContext.hpp>
#include <LuminoFramework/PostEffect/FilmicPostEffect.hpp>
#include <LuminoFramework/PostEffect/TransitionPostEffect.hpp>
#include <LuminoFramework/Scene/Component.hpp>
#include <LuminoFramework/Scene/Level.hpp>
#include <LuminoFramework/Scene/Scene.hpp>
#include <LuminoFramework/Scene/SceneConductor.hpp>
#include <LuminoFramework/Scene/WorldObject.hpp>
#include <LuminoFramework/Scene/Light.hpp>
#include <LuminoFramework/Scene/World.hpp>
#include <LuminoFramework/Scene/WorldRenderView.hpp>
#include "../../../LuminoRuntime/src/Rendering/CommandListServer.hpp"
#include <LuminoEngine/Rendering/detail/RenderStage.hpp>
#include "../../../LuminoRuntime/src/Rendering/RenderElement.hpp"

namespace ln {

//==============================================================================
// World

LN_OBJECT_IMPLEMENT(World, Object) {
}

World::World()
    : m_masterScene(makeObject_deprecated<Level>())
    , m_sceneList(makeList<Ref<Level>>())
    , m_timeScale(1.0f) {
    m_masterScene->m_ownerWorld = this;
    m_masterScene->m_initialUpdate = true;
    m_activeObjects.reserve(100);
}

World::~World() {
}

void World::init() {
    Object::init();
    m_animationContext = makeObject_deprecated<AnimationContext>();
    m_physicsWorld = makeObject_deprecated<PhysicsWorld>();
    m_physicsWorld2D = makeObject_deprecated<PhysicsWorld2D>();
    m_effectContext = makeObject_deprecated<EffectContext>();

    m_sceneConductor = makeRef<detail::SceneConductor>();

    m_mainLight = makeObject_deprecated<EnvironmentLight>();
    m_mainLight->setSpecialObject(true);
    add(m_mainLight);
}

void World::onDispose(bool explicitDisposing) {
    removeAllObjects();

    if (m_sceneConductor) {
        m_sceneConductor->releaseAndTerminateAllRunningScenes();
    }

    m_mainLight = nullptr;

    if (m_effectContext) {
        m_effectContext->dispose();
        m_effectContext = nullptr;
    }

    m_physicsWorld.reset();
    m_physicsWorld2D.reset();
    m_animationContext.reset();
    Object::onDispose(explicitDisposing);
}

void World::add(WorldObject* obj) {
    masterScene()->addObject(obj);
}

// TODO: テスト用。置き場考えておく。
void World::removeAllObjects() {
    masterScene()->removeAllObjects();
}

ReadOnlyList<Ref<WorldObject>>* World::rootObjects() const {
    return masterScene()->m_rootWorldObjectList;
}

// void World::setMainAmbientLight(AmbientLight* value)
//{
//	m_mainAmbientLight = value;
// }

WorldObject* World::findObjectByComponentType(const TypeInfo* type) const {
    for (auto& scene : m_sceneList) {
        auto obj = scene->findObjectByComponentType(type);
        if (obj) {
            return obj;
        }
    }

    return nullptr;
}

Level* World::masterScene() const {
    return m_masterScene;
}

Level* World::activeLevel() const {
    if (Level* active = m_sceneConductor->activeScene())
        return active;
    else
        return m_masterScene;
}

void World::addScene(Level* scene) {
    if (LN_REQUIRE(scene)) return;
    if (LN_REQUIRE(!scene->m_ownerWorld)) return;
    m_sceneList->add(scene);
    scene->m_ownerWorld = this;
    scene->m_initialUpdate = true;
}

// void World::gotoScene(Level* scene)
//{
//     m_sceneConductor->gotoScene(scene);
// }
//
// void World::callScene(Level* scene)
//{
//     m_sceneConductor->callScene(scene);
// }
//
// void World::returnScene()
//{
//     m_sceneConductor->returnScene();
// }

// Level* World::activeScene() const
//{
//     return m_sceneConductor->activeScene();
// }

void World::traverse(detail::IWorldObjectVisitor* visitor) const {
    for (auto& scene : m_sceneList) {
        for (auto& obj : scene->m_rootWorldObjectList) {
            if (!obj->traverse(visitor)) {
                return;
            }
        }
    }

    if (m_sceneConductor) {
        m_sceneConductor->traverse(visitor);
    }
}

WorldObject* World::findObjectById(int id) const {
    class LocalVisitor : public detail::IWorldObjectVisitor {
    public:
        int id;
        WorldObject* result = nullptr;
        bool visit(WorldObject* obj) override {
            if (obj->m_id == id) {
                result = obj;
                return false;
            }
            else {
                return true;
            }
        }
    } visitor;
    visitor.id = id;
    traverse(&visitor);
    return visitor.result;
}

WorldObject* World::findObjectByName(const StringView& name) const {
    class LocalVisitor : public detail::IWorldObjectVisitor {
    public:
        StringView name;
        WorldObject* result = nullptr;
        bool visit(WorldObject* obj) override {
            if (obj->name() == name) {
                result = obj;
                return false;
            }
            else {
                return true;
            }
        }
    } visitor;
    visitor.name = name;
    traverse(&visitor);
    return visitor.result;
}

void World::updateObjectsWorldMatrix() {
    m_masterScene->updateObjectsWorldMatrix();
    for (auto& scene : m_sceneList) {
        scene->updateObjectsWorldMatrix();
    }

    if (m_sceneConductor) {
        m_sceneConductor->updateObjectsWorldMatrix();
    }
}

void World::updateFrame(float elapsedSeconds) {
    m_sceneConductor->transitionEffect()->onUpdateFrame(elapsedSeconds);
    m_sceneConductor->executeCommands();

    // Collect ActiveObjects
    {
        m_activeObjects.clear();

        m_masterScene->collectActiveObjects(this);
        for (auto& scene : m_sceneList) {
            scene->collectActiveObjects(this);
        }
        if (m_sceneConductor) {
            m_sceneConductor->collectActiveObjects(this);
        }
    }

    // TODO: イベントの実行順序はこれで決定。ドキュメントに書いておく
    float t = elapsedSeconds * m_timeScale;
    onPreUpdate(t);
    onInternalPhysicsUpdate(t);
    onUpdate(t);
    onInternalAnimationUpdate(t);
    onPostUpdate(t);
}

void World::onPreUpdate(float elapsedSeconds) {
    m_effectContext->update(elapsedSeconds);

    m_combinedSceneGlobalRenderParams.reset();

    m_masterScene->onPreUpdate(elapsedSeconds);
    m_masterScene->mergeToRenderParams(&m_combinedSceneGlobalRenderParams);

    for (auto& level : m_sceneList) {
        level->onPreUpdate(elapsedSeconds);
        level->mergeToRenderParams(&m_combinedSceneGlobalRenderParams);
    }
    if (m_sceneConductor) {
        m_sceneConductor->preUpdate(elapsedSeconds);
        if (auto* level = m_sceneConductor->activeScene()) {
            level->mergeToRenderParams(&m_combinedSceneGlobalRenderParams);
        }
    }
}

void World::onInternalPhysicsUpdate(float elapsedSeconds) {
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

void World::onUpdate(float elapsedSeconds) {
    m_masterScene->update(elapsedSeconds);
    for (auto& scene : m_sceneList) {
        scene->update(elapsedSeconds);
    }
    if (m_sceneConductor) {
        m_sceneConductor->update(elapsedSeconds);
    }
}

void World::onInternalAnimationUpdate(float elapsedSeconds) {
}

void World::onPostUpdate(float elapsedSeconds) {
    m_masterScene->onPostUpdate(elapsedSeconds);
    for (auto& scene : m_sceneList) {
        scene->onPostUpdate(elapsedSeconds);
    }
    if (m_sceneConductor) {
        m_sceneConductor->postUpdate(elapsedSeconds);
    }

    for (auto& obj : m_activeObjects) {
        obj->forEachComponents([](auto& x) { x->onApplyPhysicsFeedForward(); });
    }
}


//detail::WorldSceneGraphRenderingContext* World::prepareRender2(const RenderViewPoint* viewPoint) {
//    m_renderingContext->resetForBeginRendering(viewPoint);
//    return m_renderingContext;
//}

// Offscreen 描画など、何回か描画を行うものを List に集める。
// ビューカリングなどはこの時点では行わない。
void World::prepareRender(RenderingContext* context, const WorldRenderView* renderView) {
    context->world = this;
    m_worldRenderingElement.clear();
    m_offscreenRenderViews.clear();

    m_masterScene->collectRenderObjects(this, context);
    for (auto& scene : m_sceneList) {
        scene->collectRenderObjects(this, context);
    }
    if (m_sceneConductor) {
        m_sceneConductor->collectRenderObjects(this, context);
    }

    if (auto& finishingProcess = renderView->finishingProcess()) {
        context->collectPostEffect(finishingProcess);
    }
    context->collectPostEffect(m_sceneConductor->transitionEffect());
}

void World::renderObjects(RenderingContext* context) {
    // m_masterScene->collectRenderObjects(m_renderingContext);
    // for (auto& scene : m_sceneList) {
    //     scene->renderObjects(m_renderingContext);
    // }
    // if (auto* scene = m_sceneConductor->activeScene()) {
    //     scene->renderObjects(m_renderingContext);
    // }
    for (IWorldRenderingElement* element : m_worldRenderingElement) {
        // element->onPrepareRender(m_renderingContext); // TODO: 全体の前にした方がいいかも
        element->render(context);
    }

    context->pushState(true);
    m_effectContext->render(context);
    context->popState();
    context->setObjectId(0);
}

void World::renderGizmos(RenderingContext* context) {
    m_masterScene->renderGizmos(context);
    for (auto& scene : m_sceneList) {
        scene->renderGizmos(context);
    }
    if (m_sceneConductor) {
        m_sceneConductor->renderGizmos(context);
    }
}

void World::enqueueActiveWorldObject(WorldObject* obj) {
    m_activeObjects.add(obj);
}

void World::enqueueWorldRenderingElement(IWorldRenderingElement* element) {
    m_worldRenderingElement.add(element);
}

void World::enqueueOffscreenRenderView(OffscreenWorldRenderView* element) {
    m_offscreenRenderViews.add(element);
}

} // namespace ln
