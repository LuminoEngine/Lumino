
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


	void setTransitionMode(LevelTransitionEffectMode value) { m_transitionMode = value; }
	LevelTransitionEffectMode transitionMode() { return m_transitionMode; }
	void setTransitionEffectColor(const Color& value);
	const Color& transitionEffectColor() const;
	void setTransitionMaskTexture(Texture* value);
	Texture* transitionMaskTexture() const;
	void setTransitionEffectDuration(float value) { m_transitionEffectDuration = value; }
	float transitionEffectDuration() const { return m_transitionEffectDuration; }
	void setTransitionEffectVague(float value);
	float transitionEffectVague() const;
	void gotoScene(Level* scene);
	void callScene(Level* scene);
	void returnScene();
	Level* activeScene() const;
	bool isTransitionEffectRunning() const;

	//const Ref<TransitionImageEffect>& transitionEffect() const { return m_transitionEffect;  }

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

	void attemptFadeOutTransition();
	void attemptFadeInTransition();

	Ref<Level> m_activeScene;
	std::deque<EventCommsnd> m_eventQueue;
	std::stack<Ref<Level>> m_sceneStack;	// not contains m_activeScene

	LevelTransitionEffectMode m_transitionMode;
	Ref<TransitionImageEffect> m_transitionEffect;
	float m_transitionEffectDuration;
};

} // namespace detail
} // namespace ln

