
#include "Internal.hpp"
#include "SceneManager.hpp"

// for registerType
#include <LuminoEngine/Scene/World.hpp>
#include <LuminoEngine/Scene/Sprite.hpp>
#include <LuminoEngine/Shader/Shader.hpp>
#include <LuminoEngine/Tilemap/Tileset.hpp>
#include <LuminoEngine/Tilemap/TilemapLayer.hpp>
#include <LuminoEngine/Tilemap/TilemapModel.hpp>
#include <LuminoEngine/Tilemap/TilemapComponent.hpp>
#include <LuminoEngine/Tilemap/Tilemap.hpp>

namespace ln {
namespace detail {

//==============================================================================
// SceneManager

SceneManager::SceneManager()
    : m_activeWorld(nullptr)
{
}

SceneManager::~SceneManager()
{
}

void SceneManager::init()
{
    LN_LOG_DEBUG << "SceneManager Initialization started.";

    m_atmosphereShader = Shader::create(u"C:/Proj/LN/Lumino/src/LuminoEngine/src/Scene/Resource/SkyFromAtmosphere.fx");

    EngineDomain::registerType<World>();
    EngineDomain::registerType<Sprite>();

    EngineDomain::registerType<Tileset>();
    EngineDomain::registerType<TilemapLayer>();
    EngineDomain::registerType<TilemapModel>();
    EngineDomain::registerType<TilemapComponent>();
    EngineDomain::registerType<Tilemap>();

    LN_LOG_DEBUG << "SceneManager Initialization ended.";
}

void SceneManager::dispose()
{
	releaseAndTerminateAllRunningScenes();
}

void SceneManager::gotoScene(Scene* scene)
{
	if (LN_REQUIRE(scene != nullptr)) return;
	EventCommsnd c;
	c.type = EventType::Goto;
	c.scene = scene;
	m_eventQueue.push_back(c);
}

void SceneManager::callScene(Scene* scene)
{
	if (LN_REQUIRE(scene != nullptr)) return;
	EventCommsnd c;
	c.type = EventType::Call;
	c.scene = scene;
	m_eventQueue.push_back(c);
}

void SceneManager::returnScene()
{
	if (LN_REQUIRE(m_activeScene != nullptr)) return;
	EventCommsnd c;
	c.type = EventType::Return;
	c.scene = nullptr;
	m_eventQueue.push_back(c);
}

void SceneManager::updateFrame()
{
	executeCommands();
	if (m_activeScene != nullptr)
	{
		m_activeScene->update();
	}
}

void SceneManager::executeCommands()
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
					m_activeScene->onCreated();
                    m_activeScene->onActivated();
				}
				break;
			}
			/////////////// 呼び出し
			case EventType::Call:
			{
				m_sceneStack.push(m_activeScene);
				m_activeScene = cmd.scene;
				m_activeScene->onCreated();
                m_activeScene->onActivated();
				break;
			}
			/////////////// 呼び出し元へ戻る
			case EventType::Return:
			{
				Ref<Scene> oldScene = m_activeScene;
				m_activeScene = m_sceneStack.top();
                oldScene->onDeactivated();
				oldScene->onClosed();
				break;
			}
		}

		m_eventQueue.pop_front();
	}
}

void SceneManager::releaseAndTerminateAllRunningScenes()
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
