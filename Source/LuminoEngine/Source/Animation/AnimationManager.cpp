
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
	//	auto obj = ac->GetTargetObject().Resolve();
	//	if (!obj.IsNull())
	//	{
	//		auto* data = tr::ReflectionHelper::RequestAnimationData<Object, detail::RefrectionObjectAnimationData>(obj);
	//		data->playingAnimationClockList.Remove(ac);
	//	}
	//}
}

//------------------------------------------------------------------------------
void AnimationManager::Initialize()
{
}

//------------------------------------------------------------------------------
void AnimationManager::AdvanceTime(float deltaTime)
{
	auto itr = m_clockList.begin();
	auto end = m_clockList.end();
	while (itr != end)
	{
		if (!(*itr)->IsFinished())
		{
			(*itr)->AdvanceTime(deltaTime);
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
void AnimationManager::AddAnimationClock(AnimationClock* clock)
{
	m_clockList.Add(clock);
}

//------------------------------------------------------------------------------
//AnimationClock* AnimationManager::StartPropertyAnimation(AnimationClockArgs* list, int listCount)
//{
//	auto ac = RefPtr<AnimationClock>::MakeRef();
//	ac->Initialize(list, listCount);
//	m_clockList.Add(ac);
//	return ac;
//}

} // namespace detail
LN_NAMESPACE_END
