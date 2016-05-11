
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
	// アクティブな AnimationClock の時間を進める。
	// もし適用対象のオブジェクトが存在しなければ AnimationClock は削除する。
	auto itr = m_clockList.begin();
	auto end = m_clockList.end();
	while (itr != end)
	{
		if (!(*itr)->IsFinished() && (*itr)->GetTargetObject().IsAlive())
		{
			(*itr)->AdvanceTime(deltaTime);
			++itr;
		}
		else
		{
			auto obj = (*itr)->GetTargetObject().Resolve();
			obj->m_playingAnimationClockList.Remove((*itr));
			itr = m_clockList.erase(itr);
			end = m_clockList.end();
		}
	}
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
