
#pragma once
#include "Common.hpp"

namespace ln {
class Level;
class TransitionImageEffect;
namespace detail {
	
/** Level の実行状態を管理します。 */
class SceneConductor : public RefObject
{
public:
	SceneConductor();
	//void init();

	const Ref<TransitionImageEffect>& transitionEffect() const { return m_transitionEffect; }

    ///** Level を読み込み、アクティブな World にアタッチします。 */
    //static Level* loadScene(const StringRef& sceneAssetFilePath);

    //static void unloadScene(Level* scene);

	void gotoScene(Level* scene);
	void callScene(Level* scene);
	void returnScene();
	Level* activeScene() const;

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
		Ref<Level> scene;
	};

	Ref<Level> m_activeScene;
	std::deque<EventCommsnd> m_eventQueue;
	std::stack<Ref<Level>> m_sceneStack;	// not contains m_activeScene

	Ref<TransitionImageEffect> m_transitionEffect;
};

} // namespace detail
} // namespace ln

