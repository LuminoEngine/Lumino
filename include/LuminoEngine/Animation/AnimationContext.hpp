
#pragma once
#include "Common.hpp"

namespace ln {
class AnimationClock;

class AnimationContext
	: public Object
{
public:
	void addAnimationClock(AnimationClock* clock);
	void advanceTime(float deltaTime);

protected:

LN_CONSTRUCT_ACCESS:
	AnimationContext();
	virtual ~AnimationContext();

private:
	List<WeakRefPtr<AnimationClock>> m_clockList;

};

} // namespace ln
