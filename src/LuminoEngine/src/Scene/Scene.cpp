/*
 Note: Level なんて必要なの？
    主な目的は、
    1. オブジェクト間の相互作用ルールを記述する
    2. コールツリーを利用して、アクティブなルールを制御する

    1 については RPG ツクールの BattleScene のイメージ。
    コマンド入力中、実行中、など。
    Unity とかだと基本的にコンポーネントの実装の中で頑張ってルールを書くことになる。
    コンポーネントの実装は一般的な MVC の考えだと V が近い。ルールは C。
    V の中に状態に応じた switch とかたくさん書く必要が出てしまうのでよくない。

    2 については、バトル画面のサブとしてメニュー画面を出したりする仕組み。
*/
#include "Internal.hpp"
#include <LuminoEngine/Base/Serializer.hpp>
#include <LuminoEngine/Scene/Component.hpp>
#include <LuminoEngine/Scene/WorldObject.hpp>
#include <LuminoEngine/Scene/Scene.hpp>
#include <LuminoEngine/Scene/World.hpp>
#include <LuminoEngine/Scene/SceneConductor.hpp>
#include <LuminoEngine/PostEffect/TransitionPostEffect.hpp>
#include "../Engine/EngineManager.hpp"
#include "SceneManager.hpp"

namespace ln {

//==============================================================================
// LevelRenderParameters

void LevelRenderParameters::serialize2(Serializer2& ar)
{
    ar & ln::makeNVP(u"fogStartDistance", m_fogStartDistance);
    ar & ln::makeNVP(u"fogColor", m_fogColor);
    ar & ln::makeNVP(u"fogDensity", m_fogDensity);
    ar & ln::makeNVP(u"fogHeightDensity", m_fogHeightDensity);
    ar & ln::makeNVP(u"fogLowerHeight", m_fogLowerHeight);
    ar & ln::makeNVP(u"fogUpperHeight", m_fogUpperHeight);
    
    ar & ln::makeNVP(u"skydomeSkyColor", m_skydomeSkyColor);
    ar & ln::makeNVP(u"skydomeHorizonColor", m_skydomeHorizonColor);
    ar & ln::makeNVP(u"skydomeCloudColor", m_skydomeCloudColor);
    ar & ln::makeNVP(u"skydomeOverlayColor", m_skydomeOverlayColor);
}

void LevelRenderParameters::mergeToRenderParams(detail::SceneGlobalRenderParams* params) const
{
    if (m_fogStartDistance) params->startDistance = m_fogStartDistance.value();
    if (m_fogColor) params->fogColor = m_fogColor.value();
    if (m_fogDensity) params->fogDensity = m_fogDensity.value();
    if (m_fogHeightDensity) params->heightFogDensity = m_fogHeightDensity.value();
    if (m_fogLowerHeight) params->lowerHeight = m_fogLowerHeight.value();
    if (m_fogUpperHeight) params->upperHeight = m_fogUpperHeight.value();

    if (m_skydomeSkyColor) params->skydomeSkyColor = m_skydomeSkyColor.value();
    if (m_skydomeHorizonColor) params->skydomeHorizonColor = m_skydomeHorizonColor.value();
    if (m_skydomeCloudColor) params->skydomeCloudColor = m_skydomeCloudColor.value();
    if (m_skydomeOverlayColor) params->skydomeOverlayColor = m_skydomeOverlayColor.value();
}

//==============================================================================
// Level

LN_OBJECT_IMPLEMENT(Level, AssetObject) {}

Level::Level()
    : m_ownerWorld(nullptr)
	, m_rootWorldObjectList(makeList<Ref<WorldObject>>())
	, m_initialUpdate(true)
{

}

Level::~Level()
{
}

bool Level::init()
{
    return AssetObject::init();
}

void Level::onStart()
{
}

void Level::onClosed()
{
}

void Level::onActivated()
{
}

void Level::onDeactivated()
{
}

void Level::onUpdate()
{
}

void Level::onPostUpdate(float elapsedSeconds)
{
    for (WorldObject* obj : m_destroyList) {
        //obj->removeFromWorld();

        obj->detachScene();
        m_rootWorldObjectList->remove(obj);
    }
    m_destroyList.clear();
}

void Level::update(float elapsedSeconds)
{
    // onUpdate 内で新しい WorldObject が作成され、m_rootWorldObjectList に add される場合に備えて
    // イテレータによる列挙は行わない。新しく追加されたものは、このループで
    // 今のフレーム中の最初の onUpdate が呼ばれる。
    for (int i = 0; i < m_rootWorldObjectList->size(); i++) {
        m_rootWorldObjectList[i]->updateFrame(elapsedSeconds);
    }

	onUpdate();
}


void Level::setup(const ln::Path& filePath)
{
	m_filePath = filePath;
}

void Level::save()
{
}

void Level::load()
{
}

void Level::clear()
{
	m_rootWorldObjectList->clear();
}

void Level::addObject(WorldObject* obj)
{
	if (LN_REQUIRE(obj)) return;
    if (obj->m_scene) {
        if (obj->m_scene == this) {
            return;
        }
        obj->m_scene->removeObject(obj);
    }

	m_rootWorldObjectList->add(obj);
    obj->attachScene(this);
}

void Level::removeObject(WorldObject* obj)
{
	m_rootWorldObjectList->remove(obj);
}

void Level::removeRootObject(WorldObject* obj)
{
    if (m_rootWorldObjectList->remove(obj)) {
        if (obj->destroyed()) {
            m_destroyList.remove(obj);
        }
        obj->detachScene();
    }
}

void Level::removeAllObjects()
{
    for (int i = m_rootWorldObjectList->size() - 1; i >= 0; i--)
    {
        auto& obj = m_rootWorldObjectList[i];
        if (!obj->isSpecialObject())
        {
            if (obj->destroyed()) {
                m_destroyList.remove(obj);
            }

            obj->detachScene();
            m_rootWorldObjectList->removeAt(i);
        }
    }
}

void Level::mergeToRenderParams(detail::SceneGlobalRenderParams* params) const
{
    if (m_levelRenderParameters) {
        m_levelRenderParameters->mergeToRenderParams(params);
    }
}

LevelRenderParameters* Level::acquireRenderParameters()
{
    if (!m_levelRenderParameters) {
        m_levelRenderParameters = makeObject<LevelRenderParameters>();
    }
    return m_levelRenderParameters;
}

// Multi-Lang 対応のため、テンプレートではなく基本は TypeInfo で検索する
WorldObject* Level::findObjectByComponentType(const TypeInfo* type) const
{
    class LocalVisitor : public detail::IWorldObjectVisitor
    {
    public:
        const TypeInfo* type;
        WorldObject* result = nullptr;
        virtual bool visit(WorldObject* obj)
        {
            for (auto& component : *(obj->m_components)) {
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

void Level::updateObjectsWorldMatrix()
{
    for (auto& obj : m_rootWorldObjectList)
    {
        obj->updateWorldMatrixHierarchical();
    }
}

void Level::onPreUpdate(float elapsedSeconds)
{
	if (m_initialUpdate) {
		for (auto& obj : m_rootWorldObjectList) {
			obj->start();
		}
		m_initialUpdate = false;
	}


    for (auto& obj : m_rootWorldObjectList)
    {
        obj->onPreUpdate();
    }
}

void Level::collectRenderObjects(World* world, RenderingContext* context)
{
    for (auto& obj : m_rootWorldObjectList) {
        // TODO: 不要なものははじいたり
        world->enqueueWorldRenderingElement(obj);

        for (auto& c : *(obj->m_components)) {
            c->onPrepareRender(context); // TODO: 全体の前にした方がいいかも
            // TODO: 不要なものははじいたり
            world->enqueueWorldRenderingElement(c);
        }
    }
}

void Level::renderGizmos(RenderingContext* context)
{
    for (auto& obj : m_rootWorldObjectList) {
        for (auto& c : *(obj->m_components)) {
            c->renderGizmo(context);
        }
    }
}

void Level::serialize2(Serializer2& ar)
{
    AssetObject::serialize2(ar);

    if (ar.isLoading()) {
        removeAllObjects();
    }

    ar & ln::makeNVP(u"renderParams", m_levelRenderParameters);

    ar & ln::makeNVP(u"children", *m_rootWorldObjectList);

    if (ar.isLoading()) {
        for (auto& obj : m_rootWorldObjectList) {
            obj->attachScene(this);
        }
    }
}
//
//void Level::onSerialize2(Serializer2* ar) override
//{
//
//}

//==============================================================================
// Scene

void Scene::gotoLevel(Level* level)
{
    detail::EngineDomain::engineManager()->mainWorld()->sceneConductor()->gotoScene(level);
}

void Scene::callLevel(Level* level)
{
    detail::EngineDomain::engineManager()->mainWorld()->sceneConductor()->callScene(level);
}

void Scene::returnLevel()
{
    detail::EngineDomain::engineManager()->mainWorld()->sceneConductor()->returnScene();
}

Level* Scene::activeLevel()
{
    return detail::EngineDomain::engineManager()->mainWorld()->sceneConductor()->activeScene();
}

bool Scene::isTransitionEffectRunning()
{
    return detail::EngineDomain::engineManager()->mainWorld()->sceneConductor()->isTransitionEffectRunning();
}

void Scene::setTransitionEffectMode(LevelTransitionEffectMode value)
{
    return detail::EngineDomain::engineManager()->mainWorld()->sceneConductor()->setTransitionMode(value);
}

LevelTransitionEffectMode Scene::transitionEffectMode()
{
    return detail::EngineDomain::engineManager()->mainWorld()->sceneConductor()->transitionMode();
}

void Scene::setTransitionDuration(float value)
{
    return detail::EngineDomain::engineManager()->mainWorld()->sceneConductor()->setTransitionEffectDuration(value);
}

float Scene::transitionDuration()
{
    return detail::EngineDomain::engineManager()->mainWorld()->sceneConductor()->transitionEffectDuration();
}

void Scene::setTransitionEffectColor(const Color& value)
{
    return detail::EngineDomain::engineManager()->mainWorld()->sceneConductor()->setTransitionEffectColor(value);
}

const Color& Scene::transitionEffectColor()
{
    return detail::EngineDomain::engineManager()->mainWorld()->sceneConductor()->transitionEffectColor();
}

void Scene::setTransitionEffectMaskTexture(Texture* value)
{
    return detail::EngineDomain::engineManager()->mainWorld()->sceneConductor()->setTransitionMaskTexture(value);
}

Texture* Scene::transitionEffectMaskTexture()
{
    return detail::EngineDomain::engineManager()->mainWorld()->sceneConductor()->transitionMaskTexture();
}

void Scene::setTransitionEffectVague(float value)
{
    return detail::EngineDomain::engineManager()->mainWorld()->sceneConductor()->setTransitionEffectVague(value);
}

float Scene::transitionEffectVague()
{
    return detail::EngineDomain::engineManager()->mainWorld()->sceneConductor()->transitionEffectVague();
}

void Scene::startFadeOut()
{
    auto& sc = detail::EngineDomain::engineManager()->mainWorld()->sceneConductor();
    sc->transitionEffect()->startFadeOut(sc->transitionEffectDuration());
}

void Scene::startFadeIn()
{
    auto& sc = detail::EngineDomain::engineManager()->mainWorld()->sceneConductor();
    sc->transitionEffect()->startFadeIn(sc->transitionEffectDuration());
}

void Scene::startCrossFade()
{
    auto& sc = detail::EngineDomain::engineManager()->mainWorld()->sceneConductor();
    sc->transitionEffect()->startCrossFade(sc->transitionEffectDuration());
}

void Scene::setFogStartDistance(float value)
{
    // TODO: Conductor が activeLevel を持ってたらそっちに設定したいかも
    detail::EngineDomain::engineManager()->mainWorld()->masterScene()->acquireRenderParameters()->m_fogStartDistance = value;
}

void Scene::setFogColor(const Color& value)
{
    detail::EngineDomain::engineManager()->mainWorld()->masterScene()->acquireRenderParameters()->m_fogColor = value;
}

void Scene::setFogDensity(float value)
{
    detail::EngineDomain::engineManager()->mainWorld()->masterScene()->acquireRenderParameters()->m_fogDensity = value;
}

void Scene::setFogHeightDensity(float value)
{
    detail::EngineDomain::engineManager()->mainWorld()->masterScene()->acquireRenderParameters()->m_fogHeightDensity = value;
}

void Scene::setFogLowerHeight(float value)
{
    detail::EngineDomain::engineManager()->mainWorld()->masterScene()->acquireRenderParameters()->m_fogLowerHeight = value;
}

void Scene::setFogUpperHeight(float value)
{
    detail::EngineDomain::engineManager()->mainWorld()->masterScene()->acquireRenderParameters()->m_fogUpperHeight= value;
}

void Scene::setSkydomeSkyColor(const Color& value)
{
    detail::EngineDomain::engineManager()->mainWorld()->masterScene()->acquireRenderParameters()->m_skydomeSkyColor = value;
}

void Scene::setSkydomeHorizonColor(const Color& value)
{
    detail::EngineDomain::engineManager()->mainWorld()->masterScene()->acquireRenderParameters()->m_skydomeHorizonColor = value;
}

void Scene::setSkydomeCloudColor(const Color& value)
{
    detail::EngineDomain::engineManager()->mainWorld()->masterScene()->acquireRenderParameters()->m_skydomeCloudColor = value;
}

void Scene::setSkydomeOverlayColor(const Color& value)
{
    detail::EngineDomain::engineManager()->mainWorld()->masterScene()->acquireRenderParameters()->m_skydomeOverlayColor = value;
}

////==============================================================================
//// SceneAsset
//
//namespace ed {
//
//LN_OBJECT_IMPLEMENT(SceneAsset, AssetModel) {}
//
//SceneAsset::SceneAsset()
//{
//}
//
//void SceneAsset::init()
//{
//    AssetModel::init();
//}
//
//void SceneAsset::setup(const ln::Path& filePath)
//{
//	m_filePath = filePath;
//}
//
//void SceneAsset::clear()
//{
//}
//
//void SceneAsset::save()
//{
//	auto json = ln::JsonSerializer::serialize(*this);
//	ln::FileSystem::writeAllText(m_filePath, json);
//}
//
//void SceneAsset::load()
//{
//}
//
//void SceneAsset::serialize(Archive& ar)
//{
//	ar & makeNVP(u"objects", m_rootWorldObjectList);
//}
//
//void SceneAsset::addNewWorldObject()
//{
//	auto obj = makeObject<WorldObjectAsset>();
//	m_rootWorldObjectList.add(obj);
//}

//} // namespace ed
} // namespace ln

