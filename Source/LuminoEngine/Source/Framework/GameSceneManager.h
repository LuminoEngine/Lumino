
#pragma once
#include <stack>
#include <Lumino/Framework/GameScene.h>

LN_NAMESPACE_BEGIN
namespace detail {

class GameSceneManager
	: public RefObject
{
public:
	static GameSceneManager* GetInstance(GameSceneManager* priority = nullptr);

	GameSceneManager();
	virtual ~GameSceneManager();
	void Finalize();

	void GotoScene(GameScene* scene);
	void CallScene(GameScene* scene);
	void ReturnScene();
	GameScene* GetActiveScene() const { return m_activeScene; }

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
