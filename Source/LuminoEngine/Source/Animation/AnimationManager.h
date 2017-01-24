
#pragma once
#include <Lumino/Animation/AnimationCurve.h>
#include <Lumino/Animation/AnimationClock.h>
#include <Lumino/Animation/AnimatableObject.h>

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

	AnimationClock* StartPropertyAnimation(AnimatableObject* targetObject/*AnimationClockArgs* list, int listCount*/)
	{
		auto ac = RefPtr<AnimationClock>::MakeRef();
		ac->Initialize(targetObject);
		m_clockList.Add(ac);

		targetObject->m_playingAnimationClockList.Add(ac);
		return ac;
	}

	
private:
	List<RefPtr<AnimationClock>>	m_clockList;
};

} // namespace detail
LN_NAMESPACE_END
