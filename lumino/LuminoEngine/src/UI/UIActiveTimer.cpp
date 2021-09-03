
#include "Internal.hpp"
#include <LuminoEngine/UI/Controls/UIControl.hpp>
#include <LuminoEngine/UI/UIActiveTimer.hpp>

namespace ln {
    
//==============================================================================
// UIActiveTimer
// https://docs.unrealengine.com/ja/Programming/Slate/Sleeping/index.html

UIActiveTimer::UIActiveTimer()
	: m_interval(1.0f)
	, m_lastTime(0.0f)
	, m_owner(nullptr)
{
}

void UIActiveTimer::init()
{
	Object::init();
}

Ref<EventConnection> UIActiveTimer::connectOnTick(Ref<UITimerEventHandler> handler)
{
	return m_onTick.connect(handler);
}

void UIActiveTimer::onTick(UITimerEventArgs* e)
{
	m_onTick.raise(e);
}

void UIActiveTimer::tick(float elapsedTimer)
{
	m_lastTime += elapsedTimer;
	if (m_lastTime >= m_interval) {
		onTick(UITimerEventArgs::create(m_owner, UIEvents::Timer, m_lastTime));
		m_lastTime = 0;
		m_owner->invalidateVisual();
	}
}

} // namespace ln

