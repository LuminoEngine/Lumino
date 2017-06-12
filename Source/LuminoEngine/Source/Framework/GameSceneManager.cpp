
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
void GameScene::initialize()
{

}

//------------------------------------------------------------------------------
void GameScene::OnStart()
{
}

//------------------------------------------------------------------------------
void GameScene::OnUpdate()
{
	for (auto& ptr : m_gameObjectList)
	{
		ptr->OnUpdate();
	}
}

//------------------------------------------------------------------------------
void GameScene::OnTerminate()
{
}

//------------------------------------------------------------------------------
void GameScene::AddGameObject(WorldObject* obj)
{
	if (LN_CHECK_ARG(obj != nullptr)) return;
	m_gameObjectList.add(obj);
}

//------------------------------------------------------------------------------
void GameScene::Update()
{

	OnUpdate();
	//tr::ReflectionHelper::GCObjectList(&m_gameObjectList);
}


//==============================================================================
// GameSceneManager
//==============================================================================
namespace detail {

static GameSceneManager* g_gameSceneManager;

//------------------------------------------------------------------------------
GameSceneManager* GameSceneManager::GetInstance(GameSceneManager* priority)
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
void GameSceneManager::Finalize()
{
	ReleaseAndTerminateAllRunningScenes();
}

//------------------------------------------------------------------------------
void GameSceneManager::GotoScene(GameScene* scene)
{
	if (LN_CHECK_ARG(scene != nullptr)) return;
	EventCommsnd c;
	c.type = EventType::Goto;
	c.scene = scene;
	m_eventQueue.push_back(c);
}

//------------------------------------------------------------------------------
void GameSceneManager::CallScene(GameScene* scene)
{
	if (LN_CHECK_ARG(scene != nullptr)) return;
	EventCommsnd c;
	c.type = EventType::Call;
	c.scene = scene;
	m_eventQueue.push_back(c);
}

//------------------------------------------------------------------------------
void GameSceneManager::ReturnScene()
{
	if (LN_CHECK_STATE(m_activeScene != nullptr)) return;
	EventCommsnd c;
	c.type = EventType::Return;
	c.scene = nullptr;
	m_eventQueue.push_back(c);
}

//------------------------------------------------------------------------------
void GameSceneManager::UpdateFrame()
{
	ExecuteCommands();
	if (m_activeScene != nullptr)
	{
		m_activeScene->Update();
	}
}

//------------------------------------------------------------------------------
void GameSceneManager::ExecuteCommands()
{
	while (!m_eventQueue.empty())
	{
		const EventCommsnd& cmd = m_eventQueue.front();

		switch (cmd.type)
		{
			/////////////// 直接遷移
			case EventType::Goto:
			{
				// 現在の全てのシーンを解放 (OnTerminate() 呼び出し)
				ReleaseAndTerminateAllRunningScenes();

				m_activeScene = cmd.scene;
				if (m_activeScene != nullptr)
				{
					m_activeScene->OnStart();
				}
				break;
			}
			/////////////// 呼び出し
			case EventType::Call:
			{
				m_sceneStack.push(m_activeScene);
				m_activeScene = cmd.scene;
				m_activeScene->OnStart();
				break;
			}
			/////////////// 呼び出し元へ戻る
			case EventType::Return:
			{
				RefPtr<GameScene> oldScene = m_activeScene;
				m_activeScene = m_sceneStack.top();
				oldScene->OnTerminate();
				break;
			}
		}

		m_eventQueue.pop_front();
	}
}

//------------------------------------------------------------------------------
void GameSceneManager::ReleaseAndTerminateAllRunningScenes()
{
	if (m_activeScene != nullptr)
	{
		m_activeScene->OnTerminate();
		m_activeScene = nullptr;
	}

	while (!m_sceneStack.empty())
	{
		m_sceneStack.top()->OnTerminate();
		m_sceneStack.pop();
	}
}

} // namespace detail
LN_NAMESPACE_END
