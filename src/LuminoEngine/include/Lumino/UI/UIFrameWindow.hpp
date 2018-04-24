#pragma once

#include "Common.hpp"
#include "UIEvents.hpp"
#include "../Platform/PlatformEvent.hpp"

namespace ln {
namespace detail { class PlatformWindow; }

class LN_API UIFrameWindow
	: public Object
	, public detail::IPlatforEventListener
{
public:

	void dispose();



LN_CONSTRUCT_ACCESS:
	UIFrameWindow();
	virtual ~UIFrameWindow();
	void initialize(const SizeI& size);

private:
	virtual bool onPlatformEvent(const detail::PlatformEventArgs& e) override;

	detail::UIManager* m_manager;
	Ref<detail::PlatformWindow>	m_platformWindow;

	Event<UIEventHandler> m_onClosed;
};

} // namespace ln

