
#include "Internal.h"
#include "AnimationManager.h"

LN_NAMESPACE_BEGIN
namespace detail
{

//=============================================================================
// AnimationManager
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
AnimationManager::AnimationManager()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
AnimationManager::~AnimationManager()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void AnimationManager::Initialize()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void AnimationManager::AdvanceTime(float deltaTime)
{
	for (RefPtr<AnimationClock>& ac : m_clockList)
	{
		ac->AdvanceTime(deltaTime);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
//AnimationClock* AnimationManager::StartPropertyAnimation(AnimationClockArgs* list, int listCount)
//{
//	auto ac = RefPtr<AnimationClock>::MakeRef();
//	ac->Initialize(list, listCount);
//	m_clockList.Add(ac);
//	return ac;
//}

} // namespace detail
LN_NAMESPACE_END
