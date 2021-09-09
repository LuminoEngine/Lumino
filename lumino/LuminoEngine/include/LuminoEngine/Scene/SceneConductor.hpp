
#pragma once
#include "Common.hpp"

namespace ln {
class Level;
class TransitionPostEffect;
namespace detail {
	
/** Level の実行状態を管理します。 */
class SceneConductor : public RefObject
{
public:
	SceneConductor();
	//void init();

	const Ref<TransitionPostEffect>& transitionEffect() const { return m_transitionEffect; }

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
	void gotoScene(Level* scene, bool withEffect);
	void callScene(Level* scene, bool withEffect);
	void returnScene(bool withEffect);
	Level* activeScene() const;
	bool isTransitionEffectRunning() const;

	//const Ref<TransitionPostEffect>& transitionEffect() const { return m_transitionEffect;  }

	void executeCommands();
	void releaseAndTerminateAllRunningScenes();

	bool traverse(detail::IWorldObjectVisitor* visitor) const;
	void updateObjectsWorldMatrix() const;
	void preUpdate(float elapsedSeconds);
	void update(float elapsedSeconds);
	void postUpdate(float elapsedSeconds);
	void collectActiveObjects(World* world);
	void collectRenderObjects(World* world, RenderingContext* context);
	void renderGizmos(RenderingContext* context);

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
		bool withEffect;
	};

	void attemptFadeOutTransition();
	void attemptFadeInTransition();

	Ref<Level> m_activeScene;
	std::deque<EventCommsnd> m_eventQueue;
	std::vector<Ref<Level>> m_sceneStack;	// not contains m_activeScene

	LevelTransitionEffectMode m_transitionMode;
	Ref<TransitionPostEffect> m_transitionEffect;
	float m_transitionEffectDuration;
};

} // namespace detail
} // namespace ln

