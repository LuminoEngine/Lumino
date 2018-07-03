#pragma once

#include "Common.hpp"
#include "UIEvents.hpp"
#include "../Platform/PlatformEvent.hpp"
#include "../Graphics/GeometryStructs.hpp"

namespace ln {
	namespace detail { class PlatformWindow; class UIManager; }
class SwapChain;

class LN_API UIFrameWindow
	: public Object
	, public detail::IPlatforEventListener
{
public:

	virtual void dispose() override;

	void renderContents();
	void present();

	SwapChain* swapChain() const;

LN_CONSTRUCT_ACCESS:
	UIFrameWindow();
	virtual ~UIFrameWindow();
	void initialize(detail::PlatformWindow* platformMainWindow, const SizeI& backbufferSize);

private:
	virtual bool onPlatformEvent(const detail::PlatformEventArgs& e) override;

	detail::UIManager* m_manager;
	Ref<detail::PlatformWindow>	m_platformWindow;
	Ref<SwapChain>	m_swapChain;

	Event<UIEventHandler> m_onClosed;
};

} // namespace ln

