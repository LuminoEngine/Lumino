
#pragma once
#include <stack>
#include <LuminoEngine/Scene/Scene.hpp>

namespace ln {

namespace detail {

class SceneManager
	: public RefObject
{
public:
	SceneManager();
	virtual ~SceneManager();
	void initialize();
	void dispose();

	void gotoScene(Scene* scene);
	void callScene(Scene* scene);
	void returnScene();
	Scene* getActiveScene() const { return m_activeScene; }

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
		Ref<Scene>   scene;
	};

	Ref<Scene>				m_activeScene;
	std::deque<EventCommsnd>		m_eventQueue;
	std::stack<Ref<Scene>>	m_sceneStack;	// not contains m_activeScene

};

} // namespace detail
} // namespace ln
