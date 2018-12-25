
#include "Internal.hpp"
#include <LuminoEngine/Animation/AnimationClock.hpp>
#include <LuminoEngine/Animation/AnimationContext.hpp>

namespace ln {

//==============================================================================
// AnimationContext

AnimationContext::AnimationContext()
{
}

AnimationContext::~AnimationContext()
{
}

void AnimationContext::addAnimationClock(AnimationClock* clock)
{
	m_clockList.add(clock);
}

void AnimationContext::advanceTime(float deltaTime)
{
	auto itr = m_clockList.begin();
	auto end = m_clockList.end();
	while (itr != end)
	{
		if (!(*itr)->isFinished())
		{
			(*itr)->advanceTime(deltaTime);
			++itr;
		}
		else
		{
			itr = m_clockList.erase(itr);
			end = m_clockList.end();
		}
	}
}

} // namespace ln