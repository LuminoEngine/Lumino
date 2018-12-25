
#include "Internal.hpp"
#include <LuminoEngine/Scene/World.hpp>
#include <LuminoEngine/Animation/AnimationContext.hpp>
#include "../Scene/SceneManager.hpp"
#include "AnimationManager.hpp"

namespace ln {
namespace detail {

//==============================================================================
// AssetManager

AnimationManager::AnimationManager()
    : m_sceneManager(nullptr)
{
}

AnimationManager::~AnimationManager()
{
}

void AnimationManager::initialize(const Settings& settings)
{
}

void AnimationManager::dispose()
{
}

void AnimationManager::addClockToAffiliation(AnimationClock* clock, AnimationClockAffiliation affiliation)
{
    if (LN_REQUIRE(clock)) return;

    switch (affiliation)
    {
    case AnimationClockAffiliation::Standalone:
        break;
    case AnimationClockAffiliation::ActiveWorld:
        m_sceneManager->activeWorld()->animationContext()->addAnimationClock(clock);
        break;
    default:
        LN_UNREACHABLE();
        break;
    }
}

void AnimationManager::updateFrame(float elapsedSeconds)
{
}

} // namespace detail
} // namespace ln

