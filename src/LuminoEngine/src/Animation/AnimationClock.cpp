
#include "Internal.hpp"
#include <LuminoEngine/Animation/AnimationClock.hpp>

namespace ln {

//==============================================================================
// AnimationClock

AnimationClock::AnimationClock()
{
}

AnimationClock::~AnimationClock()
{
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

} // namespace ln