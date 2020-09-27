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
#include <LuminoEngine/Scene/Level.hpp>
#include <LuminoEngine/Scene/Scene.hpp>
#include <LuminoEngine/Scene/World.hpp>
#include <LuminoEngine/Scene/SceneConductor.hpp>
#include <LuminoEngine/Scene/WorldRenderView.hpp>
#include <LuminoEngine/PostEffect/TransitionPostEffect.hpp>
#include <LuminoEngine/PostEffect/FilmicPostEffect.hpp>
#include "../Engine/EngineManager.hpp"
#include "SceneManager.hpp"

namespace ln {

//==============================================================================
// Scene

void Scene::gotoLevel(Level* level, bool withEffect)
{
    detail::EngineDomain::engineManager()->mainWorld()->sceneConductor()->gotoScene(level, withEffect);
}

void Scene::callLevel(Level* level, bool withEffect)
{
    detail::EngineDomain::engineManager()->mainWorld()->sceneConductor()->callScene(level, withEffect);
}

void Scene::returnLevel(bool withEffect)
{
    detail::EngineDomain::engineManager()->mainWorld()->sceneConductor()->returnScene(withEffect);
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

void Scene::setScreenBlendColor(const Color& value)
{
    detail::EngineDomain::engineManager()->mainRenderView()->finishingProcess()->setScreenBlendColor(value);
}

const Color& Scene::screenBlendColor()
{
    return detail::EngineDomain::engineManager()->mainRenderView()->finishingProcess()->screenBlendColor();
}

void Scene::setScreenColorTone(const ColorTone& value)
{
    detail::EngineDomain::engineManager()->mainRenderView()->finishingProcess()->setScreenColorTone(value);
}

const ColorTone& Scene::screenColorTone()
{
    return detail::EngineDomain::engineManager()->mainRenderView()->finishingProcess()->screenColorTone();
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

