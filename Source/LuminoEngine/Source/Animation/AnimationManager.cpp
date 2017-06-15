
#include "Internal.h"
#include "AnimationManager.h"

LN_NAMESPACE_BEGIN
namespace detail
{

//==============================================================================
// AnimationManager
//==============================================================================

//------------------------------------------------------------------------------
AnimationManager::AnimationManager()
{
}

//------------------------------------------------------------------------------
AnimationManager::~AnimationManager()
{
	//for (auto& ac : m_clockList)
	//{
	//	auto obj = ac->GetTargetObject().resolve();
	//	if (!obj.IsNull())
	//	{
	//		auto* data = tr::ReflectionHelper::requestAnimationData<Object, detail::RefrectionObjectAnimationData>(obj);
	//		data->playingAnimationClockList.Remove(ac);
	//	}
	//}
}

//------------------------------------------------------------------------------
void AnimationManager::initialize()
{
}

//------------------------------------------------------------------------------
void AnimationManager::advanceTime(float deltaTime)
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

//------------------------------------------------------------------------------
void AnimationManager::addAnimationClock(AnimationClock* clock)
{
	m_clockList.add(clock);
}

//------------------------------------------------------------------------------
//AnimationClock* AnimationManager::StartPropertyAnimation(AnimationClockArgs* list, int listCount)
//{
//	auto ac = RefPtr<AnimationClock>::MakeRef();
//	ac->initialize(list, listCount);
//	m_clockList.Add(ac);
//	return ac;
//}

} // namespace detail
LN_NAMESPACE_END
