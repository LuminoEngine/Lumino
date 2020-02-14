#pragma once
#include "UIEvents.hpp"

namespace ln {
class UIControl;
namespace detail { class UIManager; }

class UIActiveTimer
	: public Object
{
public:
    /** Tick イベントの通知を受け取るコールバックを登録します。*/
    //LN_METHOD(Event)
	Ref<EventConnection> connectOnTick(Ref<UITimerEventHandler> handler);

protected:
	virtual void onTick(UITimerEventArgs* e);

LN_CONSTRUCT_ACCESS:
	UIActiveTimer();
	void init();

private:
	void tick(float elapsedTimer);

	Event<UITimerEventHandler> m_onTick;
	float m_interval;
	float m_lastTime;
	UIElement* m_owner;

	friend class UIControl;
	friend class detail::UIManager;
};

} // namespace ln

