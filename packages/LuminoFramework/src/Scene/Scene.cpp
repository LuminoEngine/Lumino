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
#include <LuminoFramework/Scene/Component.hpp>
#include <LuminoFramework/Scene/WorldObject.hpp>
#include <LuminoFramework/Scene/Level.hpp>
#include <LuminoFramework/Scene/Scene.hpp>
#include <LuminoFramework/Scene/World.hpp>
#include <LuminoFramework/Scene/SceneConductor.hpp>
#include <LuminoFramework/Scene/WorldRenderView.hpp>
#include <LuminoFramework/PostEffect/TransitionPostEffect.hpp>
#include <LuminoFramework/PostEffect/FilmicPostEffect.hpp>
#include "../Engine/EngineManager.hpp"
#include "SceneManager.hpp"

namespace ln {

//==============================================================================
// Scene

void Scene::setClearMode(SceneClearMode value)
{
    detail::EngineDomain::engineManager()->mainRenderView()->setClearMode(value);
}

void Scene::setSkyColor(const Color& value)
{
    detail::EngineDomain::engineManager()->mainWorld()->masterScene()->acquireRenderParameters()->m_skydomeSkyColor = value;
}

void Scene::setSkyHorizonColor(const Color& value)
{
    detail::EngineDomain::engineManager()->mainWorld()->masterScene()->acquireRenderParameters()->m_skydomeHorizonColor = value;
}

void Scene::setSkyCloudColor(const Color& value)
{
    detail::EngineDomain::engineManager()->mainWorld()->masterScene()->acquireRenderParameters()->m_skydomeCloudColor = value;
}

void Scene::setSkyOverlayColor(const Color& value)
{
    detail::EngineDomain::engineManager()->mainWorld()->masterScene()->acquireRenderParameters()->m_skydomeOverlayColor = value;
}

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

//void Scene::startCrossFade()
//{
//    auto& sc = detail::EngineDomain::engineManager()->mainWorld()->sceneConductor();
//    sc->transitionEffect()->startCrossFade(sc->transitionEffectDuration());
//}

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

void Scene::setHDREnabled(bool value)
{
    detail::EngineDomain::engineManager()->mainRenderView()->setHDREnabled(true);
}

bool Scene::isHDREnabled()
{
    return detail::EngineDomain::engineManager()->mainRenderView()->isHDREnabled();
}

void Scene::setScreenBlendColor(const Color& value)
{
    detail::EngineDomain::engineManager()->mainRenderView()->finishingProcess()->setScreenBlendColor(value);
}

const Color& Scene::screenBlendColor()
{
    return detail::EngineDomain::engineManager()->mainRenderView()->finishingProcess()->screenBlendColor();
}

void Scene::setColorTone(const ColorTone& value)
{
    detail::EngineDomain::engineManager()->mainRenderView()->finishingProcess()->setScreenColorTone(value);
}

const ColorTone& Scene::colorTone()
{
    return detail::EngineDomain::engineManager()->mainRenderView()->finishingProcess()->screenColorTone();
}

void Scene::setAntialiasEnabled(bool value)
{
    detail::EngineDomain::engineManager()->mainRenderView()->finishingProcess()->setAntialiasEnabled(value);
}

bool Scene::isAntialiasEnabled()
{
    return detail::EngineDomain::engineManager()->mainRenderView()->finishingProcess()->isAntialiasEnabled();
}

void Scene::setSSREnabled(bool value)
{
    detail::EngineDomain::engineManager()->mainRenderView()->finishingProcess()->setSSREnabled(value);
}

bool Scene::isSSREnabled()
{
    return detail::EngineDomain::engineManager()->mainRenderView()->finishingProcess()->isSSREnabled();
}

void Scene::setSSAOEnabled(bool value)
{
    detail::EngineDomain::engineManager()->mainRenderView()->finishingProcess()->setSSAOEnabled(value);
}

bool Scene::isSSAOEnabled()
{
    return detail::EngineDomain::engineManager()->mainRenderView()->finishingProcess()->isSSAOEnabled();
}

void Scene::setBloomEnabled(bool value)
{
    detail::EngineDomain::engineManager()->mainRenderView()->finishingProcess()->setBloomEnabled(value);
}

bool Scene::isBloomEnabled()
{
    return detail::EngineDomain::engineManager()->mainRenderView()->finishingProcess()->isBloomEnabled();
}

void Scene::setDOFEnabled(bool value)
{
    detail::EngineDomain::engineManager()->mainRenderView()->finishingProcess()->setDOFEnabled(value);
}

bool Scene::isDOFEnabled()
{
    return detail::EngineDomain::engineManager()->mainRenderView()->finishingProcess()->isDOFEnabled();
}

void Scene::setTonemapEnabled(bool value)
{
    detail::EngineDomain::engineManager()->mainRenderView()->finishingProcess()->setTonemapEnabled(value);
}

bool Scene::isTonemapEnabled()
{
    return detail::EngineDomain::engineManager()->mainRenderView()->finishingProcess()->isTonemapEnabled();
}

void Scene::setVignetteEnabled(bool value)
{
    detail::EngineDomain::engineManager()->mainRenderView()->finishingProcess()->setVignetteEnabled(value);
}

bool Scene::isVignetteEnabled()
{
    return detail::EngineDomain::engineManager()->mainRenderView()->finishingProcess()->isVignetteEnabled();
}

void Scene::setGammaEnabled(bool value)
{
    detail::EngineDomain::engineManager()->mainRenderView()->finishingProcess()->setGammaEnabled(value);
}

bool Scene::isGammaEnabled()
{
    return detail::EngineDomain::engineManager()->mainRenderView()->finishingProcess()->isGammaEnabled();
}

void Scene::setTonemapExposure(float value)
{
    detail::EngineDomain::engineManager()->mainRenderView()->finishingProcess()->m_exposure = value;
}

void Scene::setTonemapLinearWhite(float value)
{
    detail::EngineDomain::engineManager()->mainRenderView()->finishingProcess()->m_linearWhite = value;
}

void Scene::setTonemapShoulderStrength(float value)
{
    detail::EngineDomain::engineManager()->mainRenderView()->finishingProcess()->m_shoulderStrength = value;
}

void Scene::setTonemapLinearStrength(float value)
{
    detail::EngineDomain::engineManager()->mainRenderView()->finishingProcess()->m_linearStrength = value;
}

void Scene::setTonemapLinearAngle(float value)
{
    detail::EngineDomain::engineManager()->mainRenderView()->finishingProcess()->m_linearAngle = value;
}

void Scene::setTonemapToeStrength(float value)
{
    detail::EngineDomain::engineManager()->mainRenderView()->finishingProcess()->m_toeStrength = value;
}

void Scene::setTonemapToeNumerator(float value)
{
    detail::EngineDomain::engineManager()->mainRenderView()->finishingProcess()->m_toeNumerator = value;
}

void Scene::setTonemapToeDenominator(float value)
{
    detail::EngineDomain::engineManager()->mainRenderView()->finishingProcess()->m_toeDenominator = value;
}

} // namespace ln

