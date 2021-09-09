﻿#pragma once
#include <LuminoEngine/Animation/Common.hpp>
#include <LuminoEngine/Animation/AnimationClip.hpp>
#include "../Base/RefObjectCache.hpp"

namespace ln {
class AnimationClock;
namespace detail {

class AnimationManager
	: public RefObject
{
public:
	struct Settings
	{
		AssetManager* assetManager = nullptr;
	};

    AnimationManager();
	virtual ~AnimationManager();
	void init(const Settings& settings);
	void dispose();
    void setSceneManager(SceneManager* sceneManager) { m_sceneManager = sceneManager; }
	const Ref<AnimationClipImportSettings>& defaultAnimationClipImportSettings() const { return m_defaultAnimationClipImportSettings; }

    void addClockToAffiliation(AnimationClock* clock, AnimationClockAffiliation affiliation);

	Ref<GenericTask<Ref<AnimationClip>>> loadAnimationClip(const StringRef& filePath);
	//Ref<AnimationClipPromise> loadAnimationClipAsync(const StringRef& filePath);
	//Ref<AnimationClip> acquireAnimationClip(const AssetPath& assetPath);
	//void loadAnimationClip(AnimationClip* clip, const AssetPath& assetPath);

    void updateFrame(float elapsedSeconds);

private:
	AssetManager* m_assetManager;
    SceneManager* m_sceneManager;
	ObjectCache<String, AnimationClip> m_animationClipCache;
	Ref<AnimationClipImportSettings> m_defaultAnimationClipImportSettings;
};

} // namespace detail
} // namespace ln

