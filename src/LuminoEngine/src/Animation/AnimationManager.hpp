#pragma once
#include <LuminoEngine/Animation/Common.hpp>

namespace ln {
class AnimationClock;
namespace detail {

class AnimationManager
	: public RefObject
{
public:
	struct Settings
	{
	};

    AnimationManager();
	virtual ~AnimationManager();
	void initialize(const Settings& settings);
	void dispose();
    void setSceneManager(SceneManager* sceneManager) { m_sceneManager = sceneManager; }
        

    void addClockToAffiliation(AnimationClock* clock, AnimationClockAffiliation affiliation);

    void updateFrame(float elapsedSeconds);

private:
    SceneManager* m_sceneManager;
};

} // namespace detail
} // namespace ln

