
#pragma once
#include "Common.hpp"

namespace ln {
class Scene;

namespace detail {
	
/** Scene の実行状態を管理します。 */
class SceneConductor : public RefObject
{
public:
    ///** Scene を読み込み、アクティブな World にアタッチします。 */
    //static Scene* loadScene(const StringRef& sceneAssetFilePath);

    //static void unloadScene(Scene* scene);

	void gotoScene(Scene* scene);
	void callScene(Scene* scene);
	void returnScene();
	Scene* activeScene() const;

	void executeCommands();
	void releaseAndTerminateAllRunningScenes();

private:
	enum class EventType
	{
		Goto,
		Call,
		Return,
		/*
		CHANGE_LOADING,
		PUSH_LOADING
		*/
	};

	struct EventCommsnd
	{
		EventType type;
		Ref<Scene> scene;
	};

	Ref<Scene> m_activeScene;
	std::deque<EventCommsnd> m_eventQueue;
	std::stack<Ref<Scene>> m_sceneStack;	// not contains m_activeScene
};

} // namespace detail
} // namespace ln

