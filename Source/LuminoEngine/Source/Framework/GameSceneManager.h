
#pragma once
#include <stack>
#include <Lumino/Framework/GameScene.h>

LN_NAMESPACE_BEGIN
namespace detail {

class GameSceneManager
	: public RefObject
{
public:
	static GameSceneManager* getInstance(GameSceneManager* priority = nullptr);

	GameSceneManager();
	virtual ~GameSceneManager();
	void dispose();

	void gotoScene(GameScene* scene);
	void callScene(GameScene* scene);
	void returnScene();
	GameScene* getActiveScene() const { return m_activeScene; }

	void updateFrame();

private:
	void executeCommands();
	void releaseAndTerminateAllRunningScenes();

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
