
#pragma once
#include <stack>
#include <Lumino/Foundation/GameScene.h>

LN_NAMESPACE_BEGIN
namespace detail {

class GameSceneManager
	: public RefObject
{
public:
	GameSceneManager();
	virtual ~GameSceneManager();
	void Finalize();

	void GotoScene(GameScene* scene);
	void CallScene(GameScene* scene);
	void ReturnScene();
	void UpdateFrame();

private:
	void ExecuteCommands();
	void ReleaseAndTerminateAllRunningScenes();

	enum class EventType
	{
		Goto,
		Call,
		Return,
		/*
		SMC_CHANGE_LOADING,
		SMC_PUSH_LOADING
		*/
	};

	struct EventCommsnd
	{
		EventType			type;
		RefPtr<GameScene>   scene;
	};

	RefPtr<GameScene>				m_activeScene;
	std::deque<EventCommsnd>		m_eventQueue;
	std::stack<RefPtr<GameScene>>	m_sceneStack;	// not contains m_activeScene
};

} // namespace detail
LN_NAMESPACE_END
