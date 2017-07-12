
#include "Internal.h"
#include <Lumino/Animation/AnimationClock.h>
#include <Lumino/Animation/AnimationCurve.h>
#include "AnimationManager.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// AnimationClock
//==============================================================================

//------------------------------------------------------------------------------
RefPtr<AnimationClock> AnimationClock::create()
{
	return ln::newObject<AnimationClock>();
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
void AnimationClock::start(float startValue, float targetValue, float duration, EasingMode easingMode, const Delegate<void(float)>& setCallback, const Delegate<void(void)>& endCallback)
{
	auto timeline = FloatEasingAnimationTimeline::create(targetValue, duration, easingMode);
	m_timelineInstance = detail::createTimelineInstance<float>(timeline, startValue, setCallback, endCallback);
	addManager();
}

//------------------------------------------------------------------------------
void AnimationClock::advanceTime(float deltaTime)
{
	if (m_timelineInstance != nullptr)
	{
		m_timelineInstance->advanceTime(deltaTime);
	}
}

//------------------------------------------------------------------------------
bool AnimationClock::isFinished() const
{
	return m_timelineInstance == nullptr || m_timelineInstance->isFinished();
}

//------------------------------------------------------------------------------
void AnimationClock::addManager()
{
	detail::EngineDomain::getAnimationManager()->addAnimationClock(this);
}


//==============================================================================
// AnimationTimelineInstance
//==============================================================================
namespace detail {

//------------------------------------------------------------------------------
bool AnimationTimelineInstance::isFinished() const
{
	return m_currentTime >= m_timeline->getLastTime();
}

//------------------------------------------------------------------------------
void AnimationTimelineInstance::advanceTime(float deltaTime)
{
	float lastTime = m_timeline->getLastTime();
	bool endFrame = (m_currentTime < lastTime && m_currentTime + deltaTime >= lastTime);

	m_currentTime += deltaTime;
	updateTime(0, m_currentTime);

	if (endFrame && !m_endCallback.isEmpty())
	{
		m_endCallback();
	}
}

} // namespace detail
LN_NAMESPACE_END
