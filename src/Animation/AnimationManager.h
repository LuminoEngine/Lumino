
#pragma once
#include "AnimationCurve.h"
#include <Lumino/Animation/AnimationClock.h>

LN_NAMESPACE_BEGIN
namespace detail
{



class AnimationManager
	: public Object
{
public:
	AnimationManager();
	virtual ~AnimationManager();

	void Initialize();

	void AdvanceTime(float deltaTime);

	AnimationClock* StartPropertyAnimation(/*AnimationClockArgs* list, int listCount*/)
	{
		auto ac = RefPtr<AnimationClock>::MakeRef();
		//ac->Initialize(list, listCount);
		m_clockList.Add(ac);
		return ac;
	}

	
private:
	Array<RefPtr<AnimationClock>>	m_clockList;
};

} // namespace detail
LN_NAMESPACE_END
