#pragma once
#include <LuminoGraphics/Animation/Common.hpp>
#include <LuminoGraphics/Animation/AnimationClip.hpp>
#include <LuminoEngine/Base/detail/RefObjectCache.hpp>

namespace ln {
class AnimationClock;
namespace detail {

class AnimationManager : public RefObject {
public:
    struct Settings {
        AssetManager* assetManager = nullptr;
    };

    static AnimationManager* initialize(const Settings& settings);
    static void terminate();
    static inline AnimationManager* instance() { return s_instance; }

    // void setSceneManager(SceneManager* sceneManager) { m_sceneManager = sceneManager; }
    const Ref<AnimationClipImportSettings>& defaultAnimationClipImportSettings() const { return m_defaultAnimationClipImportSettings; }

    void addClockToAffiliation(AnimationClock* clock, AnimationClockAffiliation affiliation);

    Ref<GenericTask<Ref<AnimationClip>>> loadAnimationClip(const StringView& filePath);
    // Ref<AnimationClipPromise> loadAnimationClipAsync(const StringView& filePath);
    // Ref<AnimationClip> acquireAnimationClip(const AssetPath& assetPath);
    // void loadAnimationClip(AnimationClip* clip, const AssetPath& assetPath);

    void updateFrame(float elapsedSeconds);

private:
    AnimationManager();
    virtual ~AnimationManager();
    Result<> init(const Settings& settings);
    void dispose();

    AssetManager* m_assetManager;
    // SceneManager* m_sceneManager;
    ObjectCache<String, AnimationClip> m_animationClipCache;
    Ref<AnimationClipImportSettings> m_defaultAnimationClipImportSettings;

    static Ref<AnimationManager> s_instance;
};

} // namespace detail
} // namespace ln
