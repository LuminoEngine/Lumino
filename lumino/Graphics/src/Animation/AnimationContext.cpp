
#include "Internal.hpp"
#include <LuminoGraphics/Animation/AnimationClock.hpp>
#include <LuminoGraphics/Animation/AnimationContext.hpp>

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
        auto clock = itr->resolve();

		if (!clock->isFinished())
		{
            clock->advanceTime(deltaTime);
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