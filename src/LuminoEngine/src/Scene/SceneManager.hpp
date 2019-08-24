
#pragma once
#include <stack>
#include <LuminoEngine/Scene/Scene.hpp>

namespace ln {
class Shader;
class World;

namespace detail {

class SceneManager
	: public RefObject
{
public:
	SceneManager();
	virtual ~SceneManager();
	void init();
	void dispose();

    Scene* loadScene(const StringRef& sceneAssetFilePath);
    void unloadScene(Scene* scene);

#if 0
	void gotoScene(Scene* scene);
	void callScene(Scene* scene);
	void returnScene();
#endif
	Scene* activeScene() const { return m_activeScene; }

	void updateFrame();

    void setActiveWorld(World* world) { m_activeWorld = world; }
    World* activeWorld() const { return m_activeWorld; }
    const Ref<Shader>& atmosphereShader() const { return m_atmosphereShader; }

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

    World* m_activeWorld;
    Ref<Shader> m_atmosphereShader;
};

} // namespace detail
} // namespace ln
