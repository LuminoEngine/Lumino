
#include "../Internal.h"
#include <Lumino/Framework/GameScene.h>
#include <Lumino/Scene/WorldObject.h>
#include "GameSceneManager.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// GameScene
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(GameScene, Object);

//------------------------------------------------------------------------------
GameScene::GameScene()
{
}

//------------------------------------------------------------------------------
GameScene::~GameScene()
{
}

//------------------------------------------------------------------------------
bool GameScene::initialize()
{
	return Object::initialize();
}

//------------------------------------------------------------------------------
void GameScene::onStart()
{
}

//------------------------------------------------------------------------------
void GameScene::onUpdate()
{
	for (auto& ptr : m_gameObjectList)
	{
		ptr->onUpdate();
	}
}

//------------------------------------------------------------------------------
void GameScene::onTerminate()
{
}

//------------------------------------------------------------------------------
void GameScene::addGameObject(WorldObject* obj)
{
	if (LN_CHECK_ARG(obj != nullptr)) return;
	m_gameObjectList.add(obj);
}

//------------------------------------------------------------------------------
void GameScene::update()
{

	onUpdate();
	//tr::ReflectionHelper::gcObjectList(&m_gameObjectList);
}


//==============================================================================
// GameSceneManager
//==============================================================================
namespace detail {

static GameSceneManager* g_gameSceneManager;

//------------------------------------------------------------------------------
GameSceneManager* GameSceneManager::getInstance(GameSceneManager* priority)
{
	return (priority != nullptr) ? priority : g_gameSceneManager;
}

//------------------------------------------------------------------------------
GameSceneManager::GameSceneManager()
{
	g_gameSceneManager = this;
}

//------------------------------------------------------------------------------
GameSceneManager::~GameSceneManager()
{
	g_gameSceneManager = nullptr;
}

//------------------------------------------------------------------------------
void GameSceneManager::dispose()
{
	releaseAndTerminateAllRunningScenes();
}

//------------------------------------------------------------------------------
void GameSceneManager::gotoScene(GameScene* scene)
{
	if (LN_CHECK_ARG(scene != nullptr)) return;
	EventCommsnd c;
	c.type = EventType::Goto;
	c.scene = scene;
	m_eventQueue.push_back(c);
}

//------------------------------------------------------------------------------
void GameSceneManager::callScene(GameScene* scene)
{
	if (LN_CHECK_ARG(scene != nullptr)) return;
	EventCommsnd c;
	c.type = EventType::Call;
	c.scene = scene;
	m_eventQueue.push_back(c);
}

//------------------------------------------------------------------------------
void GameSceneManager::returnScene()
{
	if (LN_CHECK_STATE(m_activeScene != nullptr)) return;
	EventCommsnd c;
	c.type = EventType::Return;
	c.scene = nullptr;
	m_eventQueue.push_back(c);
}

//------------------------------------------------------------------------------
void GameSceneManager::updateFrame()
{
	executeCommands();
	if (m_activeScene != nullptr)
	{
		m_activeScene->update();
	}
}

//------------------------------------------------------------------------------
void GameSceneManager::executeCommands()
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
				}
				break;
			}
			/////////////// 呼び出し
			case EventType::Call:
			{
				m_sceneStack.push(m_activeScene);
				m_activeScene = cmd.scene;
				m_activeScene->onStart();
				break;
			}
			/////////////// 呼び出し元へ戻る
			case EventType::Return:
			{
				Ref<GameScene> oldScene = m_activeScene;
				m_activeScene = m_sceneStack.top();
				oldScene->onTerminate();
				break;
			}
		}

		m_eventQueue.pop_front();
	}
}

//------------------------------------------------------------------------------
void GameSceneManager::releaseAndTerminateAllRunningScenes()
{
	if (m_activeScene != nullptr)
	{
		m_activeScene->onTerminate();
		m_activeScene = nullptr;
	}

	while (!m_sceneStack.empty())
	{
		m_sceneStack.top()->onTerminate();
		m_sceneStack.pop();
	}
}

} // namespace detail
LN_NAMESPACE_END
