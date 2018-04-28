﻿#pragma once

#include "Common.hpp"
#include "UIEvents.hpp"
#include "../Platform/PlatformEvent.hpp"

namespace ln {
namespace detail { class PlatformWindow; }
class SwapChain;

class LN_API UIFrameWindow
	: public Object
	, public detail::IPlatforEventListener
{
public:

	void dispose();

	void renderContents();
	void present();


LN_CONSTRUCT_ACCESS:
	UIFrameWindow();
	virtual ~UIFrameWindow();
	void initialize(const SizeI& size);

private:
	virtual bool onPlatformEvent(const detail::PlatformEventArgs& e) override;

	detail::UIManager* m_manager;
	Ref<detail::PlatformWindow>	m_platformWindow;
	Ref<SwapChain>	m_swapChain;

	Event<UIEventHandler> m_onClosed;
};

} // namespace ln

