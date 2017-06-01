
#include "Internal.h"
#include <Lumino/Animation/AnimationClock.h>
#include <Lumino/Animation/AnimationCurve.h>
#include "AnimationManager.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// AnimationClock
//==============================================================================

//------------------------------------------------------------------------------
RefPtr<AnimationClock> AnimationClock::Create()
{
	return ln::NewObject<AnimationClock>();
}

//------------------------------------------------------------------------------
AnimationClock::AnimationClock()
	: m_timelineInstance(nullptr)
	//, m_targetObject()
	//, m_currentTime(0)
{
}

//------------------------------------------------------------------------------
//AnimationClock::AnimationClock(AnimationTimeline* timeline, const Delegate<void(void)>& endCallback)
//	: m_timelineInstance(timeline)
//	, m_targetObject()
//	, m_currentTime(0)
//	, m_endCallback(endCallback)
//{
//}

//------------------------------------------------------------------------------
void AnimationClock::Start(float startValue, float targetValue, float duration, EasingMode easingMode, const Delegate<void(float)>& setCallback, const Delegate<void(void)>& endCallback)
{
	auto timeline = FloatEasingAnimationTimeline::Create(targetValue, duration, easingMode);
	m_timelineInstance = detail::CreateTimelineInstance<float>(timeline, startValue, setCallback, endCallback);
	AddManager();
}

//------------------------------------------------------------------------------
void AnimationClock::AdvanceTime(float deltaTime)
{
	if (m_timelineInstance != nullptr)
	{
		m_timelineInstance->AdvanceTime(deltaTime);
	}
}

//------------------------------------------------------------------------------
bool AnimationClock::IsFinished() const
{
	return m_timelineInstance == nullptr || m_timelineInstance->IsFinished();
}

//------------------------------------------------------------------------------
void AnimationClock::AddManager()
{
	detail::EngineDomain::GetAnimationManager()->AddAnimationClock(this);
}


//==============================================================================
// AnimationTimelineInstance
//==============================================================================
namespace detail {

//------------------------------------------------------------------------------
bool AnimationTimelineInstance::IsFinished() const
{
	return m_currentTime >= m_timeline->GetLastTime();
}

//------------------------------------------------------------------------------
void AnimationTimelineInstance::AdvanceTime(float deltaTime)
{
	float lastTime = m_timeline->GetLastTime();
	bool endFrame = (m_currentTime < lastTime && m_currentTime + deltaTime >= lastTime);

	m_currentTime += deltaTime;
	UpdateTime(0, m_currentTime);

	if (endFrame && m_endCallback != nullptr)
	{
		m_endCallback();
	}
}

} // namespace detail
LN_NAMESPACE_END
