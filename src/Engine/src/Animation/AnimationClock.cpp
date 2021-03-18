
#include "Internal.hpp"
#include <LuminoEngine/Animation/AnimationClock.hpp>
#include <LuminoEngine/Animation/AnimationTrack.hpp>
#include "AnimationManager.hpp"

namespace ln {

//==============================================================================
// AnimationClock

AnimationClock::AnimationClock()
{
}

AnimationClock::~AnimationClock()
{
}

void AnimationClock::init(AnimationClockAffiliation affiliation)
{
    Object::init();
    detail::EngineDomain::animationManager()->addClockToAffiliation(this, affiliation);
}

bool AnimationClock::isFinished() const
{
	LN_NOTIMPLEMENTED();
	return false;
	//return m_timelineInstance == nullptr || m_timelineInstance->isFinished();
}

void AnimationClock::advanceTime(float deltaTime)
{
	LN_NOTIMPLEMENTED();
	//if (m_timelineInstance != nullptr)
	//{
	//	m_timelineInstance->advanceTime(deltaTime);
	//}
}

//==============================================================================
// SingleAnimationClock

SingleAnimationClock::SingleAnimationClock()
{
}

SingleAnimationClock::~SingleAnimationClock()
{
}

void SingleAnimationClock::init()
{
    AnimationClock::init(AnimationClockAffiliation::ActiveWorld);
}

void SingleAnimationClock::setTrack(AnimationTrack* track)
{
    m_track = track;
}

} // namespace ln