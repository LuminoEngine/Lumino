
#include "Internal.hpp"
#include <LuminoEngine/PostEffect/TransitionPostEffect.hpp>
#include <LuminoEngine/Scene/Scene.hpp>
#include <LuminoEngine/Scene/SceneConductor.hpp>
#include "SceneManager.hpp"

namespace ln {
namespace detail {

//==============================================================================
// SceneConductor

//Level* SceneConductor::loadScene(const StringRef& sceneAssetFilePath)
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
	m_transitionEffect = makeObject<TransitionPostEffect>();
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

void SceneConductor::gotoScene(Level* scene)
{
	if (LN_REQUIRE(scene != nullptr)) return;
	//if (LN_REQUIRE(!isTransitionEffectRunning())) return;
	EventCommsnd c;
	c.type = EventType::Goto;
	c.scene = scene;
	m_eventQueue.push_back(c);

	attemptFadeOutTransition();
}

void SceneConductor::callScene(Level* scene)
{
	if (LN_REQUIRE(scene != nullptr)) return;
	//if (LN_REQUIRE(!isTransitionEffectRunning())) return;
	EventCommsnd c;
	c.type = EventType::Call;
	c.scene = scene;
	m_eventQueue.push_back(c);

	attemptFadeOutTransition();
}

void SceneConductor::returnScene()
{
	if (LN_REQUIRE(m_activeScene != nullptr)) return;
	//if (LN_REQUIRE(!isTransitionEffectRunning())) return;
	EventCommsnd c;
	c.type = EventType::Return;
	c.scene = nullptr;
	m_eventQueue.push_back(c);

	attemptFadeOutTransition();
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
			if (m_activeScene != nullptr)
			{
				m_activeScene->onStart();
				m_activeScene->onActivated();
			}

			if (fadein) {
				attemptFadeInTransition();
			}
			break;
		}
		/////////////// 呼び出し
		case EventType::Call:
		{
			m_sceneStack.push(m_activeScene);
			m_activeScene = cmd.scene;
			m_activeScene->onStart();
			m_activeScene->onActivated();

			if (fadein) {
				attemptFadeInTransition();
			}
			break;
		}
		/////////////// 呼び出し元へ戻る
		case EventType::Return:
		{
			Ref<Level> oldScene = m_activeScene;
			m_activeScene = m_sceneStack.top();
			oldScene->onDeactivated();
			oldScene->onClosed();

			if (fadein) {
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
		m_activeScene->onClosed();
		m_activeScene = nullptr;
	}

	while (!m_sceneStack.empty())
	{
		m_sceneStack.top()->onClosed();
		m_sceneStack.pop();
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
