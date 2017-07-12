
#pragma once
#include <Lumino/Animation/AnimationCurve.h>
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

	void initialize();

	void advanceTime(float deltaTime);

	void addAnimationClock(AnimationClock* clock);

	//AnimationClock* StartPropertyAnimation(AnimatableObject* targetObject/*AnimationClockArgs* list, int listCount*/)
	//{
	//	auto ac = RefPtr<AnimationClock>::MakeRef();
	//	ac->initialize(targetObject);
	//	m_clockList.Add(ac);

	//	targetObject->m_playingAnimationClockList.Add(ac);
	//	return ac;
	//}

	
private:
	List<RefPtr<AnimationClock>>	m_clockList;
};

} // namespace detail
LN_NAMESPACE_END
