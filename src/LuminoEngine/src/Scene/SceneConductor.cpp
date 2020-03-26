
#include "Internal.hpp"
#include <LuminoEngine/ImageEffect/TransitionImageEffect.hpp>
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
{
	m_transitionEffect = makeObject<TransitionImageEffect>();
}

//void SceneConductor::init()
//{
//
//}
//
void SceneConductor::gotoScene(Level* scene)
{
	if (LN_REQUIRE(scene != nullptr)) return;
	EventCommsnd c;
	c.type = EventType::Goto;
	c.scene = scene;
	m_eventQueue.push_back(c);
}

void SceneConductor::callScene(Level* scene)
{
	if (LN_REQUIRE(scene != nullptr)) return;
	EventCommsnd c;
	c.type = EventType::Call;
	c.scene = scene;
	m_eventQueue.push_back(c);
}

void SceneConductor::returnScene()
{
	if (LN_REQUIRE(m_activeScene != nullptr)) return;
	EventCommsnd c;
	c.type = EventType::Return;
	c.scene = nullptr;
	m_eventQueue.push_back(c);
}

Level* SceneConductor::activeScene() const
{
	return m_activeScene;
}

void SceneConductor::executeCommands()
{
	while (!m_eventQueue.empty())
	{
		const EventCommsnd& cmd = m_eventQueue.front();

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
			break;
		}
		/////////////// 呼び出し
		case EventType::Call:
		{
			m_sceneStack.push(m_activeScene);
			m_activeScene = cmd.scene;
			m_activeScene->onStart();
			m_activeScene->onActivated();
			break;
		}
		/////////////// 呼び出し元へ戻る
		case EventType::Return:
		{
			Ref<Level> oldScene = m_activeScene;
			m_activeScene = m_sceneStack.top();
			oldScene->onDeactivated();
			oldScene->onClosed();
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

} // namespace detail
} // namespace ln
