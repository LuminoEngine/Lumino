
#include "Internal.hpp"
#include <LuminoEngine/PostEffect/TransitionPostEffect.hpp>
#include <LuminoEngine/Scene/Level.hpp>
#include <LuminoEngine/Scene/SceneConductor.hpp>
#include "SceneManager.hpp"

namespace ln {
namespace detail {

//==============================================================================
// SceneConductor

//Level* SceneConductor::loadScene(const StringView& sceneAssetFilePath)
//{
//    return detail::EngineDomain::sceneManager()->loadScene(sceneAssetFilePath);
//}
//
//void SceneConductor::unloadScene(Level* scene)
//{
//    detail::EngineDomain::sceneManager()->unloadScene(scene);
//}

#if 0
void SceneConductor::gotoScene(Level* scene)
{
    detail::EngineDomain::sceneManager()->gotoScene(scene);
}

void SceneConductor::callScene(Level* scene)
{
    detail::EngineDomain::sceneManager()->callScene(scene);
}

void SceneConductor::returnScene()
{
    detail::EngineDomain::sceneManager()->returnScene();
}

Level* SceneConductor::activeScene()
{
    return detail::EngineDomain::sceneManager()->activeScene();
}
#endif

SceneConductor::SceneConductor()
	: m_transitionMode(LevelTransitionEffectMode::FadeInOut)
	, m_transitionEffectDuration(1.0)
{
	m_transitionEffect = makeObject_deprecated<TransitionPostEffect>();
}

//void SceneConductor::init()
//{
//
//}
//

void SceneConductor::setTransitionEffectColor(const Color& value)
{
	m_transitionEffect->setFadeColor(value);
}

const Color& SceneConductor::transitionEffectColor() const
{
	return m_transitionEffect->fadeColor();
}

void SceneConductor::setTransitionMaskTexture(Texture* value)
{
	m_transitionEffect->setMaskTexture(value);
}

Texture* SceneConductor::transitionMaskTexture() const
{
	return m_transitionEffect->maskTexture();
}

void SceneConductor::setTransitionEffectVague(float value)
{
	m_transitionEffect->setVague(value);
}

float SceneConductor::transitionEffectVague() const
{
	return m_transitionEffect->vague();
}

void SceneConductor::gotoScene(Level* scene, bool withEffect)
{
	if (LN_REQUIRE(scene != nullptr)) return;
	EventCommsnd c;
	c.type = EventType::Goto;
	c.scene = scene;
	c.withEffect = withEffect;
	m_eventQueue.push_back(c);

	if (withEffect) {
		attemptFadeOutTransition();
	}
}

void SceneConductor::callScene(Level* scene, bool withEffect)
{
	if (LN_REQUIRE(scene != nullptr)) return;
	EventCommsnd c;
	c.type = EventType::Call;
	c.scene = scene;
	c.withEffect = withEffect;
	m_eventQueue.push_back(c);

	if (withEffect) {
		attemptFadeOutTransition();
	}
}

void SceneConductor::returnScene(bool withEffect)
{
	if (LN_REQUIRE(m_activeScene != nullptr)) return;
	EventCommsnd c;
	c.type = EventType::Return;
	c.scene = nullptr;
	c.withEffect = withEffect;
	m_eventQueue.push_back(c);

	if (withEffect) {
		attemptFadeOutTransition();
	}
}

Level* SceneConductor::activeScene() const
{
	return m_activeScene;
}

bool SceneConductor::isTransitionEffectRunning() const
{
	return m_transitionEffect->isRunning();
}

void SceneConductor::executeCommands()
{
	if (m_transitionMode == LevelTransitionEffectMode::FadeInOut && isTransitionEffectRunning()) {
		return;
	}

	while (!m_eventQueue.empty())
	{
		const EventCommsnd& cmd = m_eventQueue.front();
		bool fadein = true;//(m_activeScene != nullptr);

		switch (cmd.type)
		{
			/////////////// 直接遷移
			case EventType::Goto:
			{
				// 現在の全てのシーンを解放 (onTerminate() 呼び出し)
				releaseAndTerminateAllRunningScenes();

				m_activeScene = cmd.scene;
				if (m_activeScene) {
					m_activeScene->onStart();
				}

				if (cmd.withEffect) {
					attemptFadeInTransition();
				}
				break;
			}
			/////////////// 呼び出し
			case EventType::Call:
			{
				if (m_activeScene && m_activeScene->updateMode() == LevelUpdateMode::PauseWhenInactive) {
					m_activeScene->onPause();
				}

				m_sceneStack.push_back(m_activeScene);
				m_activeScene = cmd.scene;
				m_activeScene->onStart();

				if (cmd.withEffect) {
					attemptFadeInTransition();
				}
				break;
			}
			/////////////// 呼び出し元へ戻る
			case EventType::Return:
			{
				Ref<Level> oldScene = m_activeScene;
				m_activeScene = m_sceneStack.back();
				m_sceneStack.pop_back();
				oldScene->onStop();

				if (m_activeScene && m_activeScene->updateMode() == LevelUpdateMode::PauseWhenInactive) {
					m_activeScene->onResume();
				}

				if (cmd.withEffect) {
					attemptFadeInTransition();
				}
				break;
			}
		}

		m_eventQueue.pop_front();
	}
}

void SceneConductor::releaseAndTerminateAllRunningScenes()
{
	if (m_activeScene != nullptr)
	{
		m_activeScene->onStop();
		m_activeScene = nullptr;
	}

	for (int i = m_sceneStack.size() - 1; i >= 0; i--) {
		m_sceneStack[i]->onStop();
	}
	m_sceneStack.clear();
}

bool SceneConductor::traverse(detail::IWorldObjectVisitor* visitor) const
{
	for (const auto& level : m_sceneStack) {
		if (!level->traverse(visitor)) {
			return false;
		}
	}

	if (m_activeScene) {
		if (!m_activeScene->traverse(visitor)) {
			return false;
		}
	}

	return true;
}

void SceneConductor::updateObjectsWorldMatrix() const
{
	for (const auto& level : m_sceneStack) {
		level->updateObjectsWorldMatrix();
	}

	if (m_activeScene) {
		m_activeScene->updateObjectsWorldMatrix();
	}
}

void SceneConductor::preUpdate(float elapsedSeconds)
{
	for (const auto& level : m_sceneStack) {
		if (level->updateMode() == LevelUpdateMode::Always) {
			level->onPreUpdate(elapsedSeconds);
		}
	}

	if (m_activeScene) {
		m_activeScene->onPreUpdate(elapsedSeconds);
	}
}

void SceneConductor::update(float elapsedSeconds)
{
	for (const auto& level : m_sceneStack) {
		if (level->updateMode() == LevelUpdateMode::Always) {
			level->update(elapsedSeconds);
		}
	}

	if (m_activeScene) {
		m_activeScene->update(elapsedSeconds);
	}
}

void SceneConductor::postUpdate(float elapsedSeconds)
{
	for (const auto& level : m_sceneStack) {
		if (level->updateMode() == LevelUpdateMode::Always) {
			level->onPostUpdate(elapsedSeconds);
		}
	}

	if (m_activeScene) {
		m_activeScene->onPostUpdate(elapsedSeconds);
	}
}

void SceneConductor::collectActiveObjects(World* world)
{
	for (const auto& level : m_sceneStack) {
		level->collectActiveObjects(world);
	}

	if (m_activeScene) {
		m_activeScene->collectActiveObjects(world);
	}
}

void SceneConductor::collectRenderObjects(World* world, RenderingContext* context)
{
	for (const auto& level : m_sceneStack) {
		level->collectRenderObjects(world, context);
	}

	if (m_activeScene) {
		m_activeScene->collectRenderObjects(world, context);
	}
}

void SceneConductor::renderGizmos(RenderingContext* context)
{
	for (const auto& level : m_sceneStack) {
		level->renderGizmos(context);
	}

	if (m_activeScene) {
		m_activeScene->renderGizmos(context);
	}
}

void SceneConductor::attemptFadeOutTransition()
{
	if (m_activeScene) {
		switch (m_transitionMode)
		{
		case LevelTransitionEffectMode::None:
			break;
		case LevelTransitionEffectMode::FadeInOut:
			m_transitionEffect->startFadeOut(m_transitionEffectDuration / 2.0);
			break;
		case LevelTransitionEffectMode::CrossFade:
			m_transitionEffect->startCrossFade(m_transitionEffectDuration);
			break;
		default:
			LN_UNREACHABLE();
			break;
		}
	}
}

void SceneConductor::attemptFadeInTransition()
{
	switch (m_transitionMode)
	{
	case LevelTransitionEffectMode::None:
		break;
	case LevelTransitionEffectMode::FadeInOut:
		m_transitionEffect->startFadeIn(m_transitionEffectDuration / 2.0);
		break;
	case LevelTransitionEffectMode::CrossFade:
		break;
	default:
		LN_UNREACHABLE();
		break;
	}
}

} // namespace detail
} // namespace ln
