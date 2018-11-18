#pragma once

#include "Common.hpp"
#include "UIEvents.hpp"
#include "UIElement.hpp"
#include "../Platform/PlatformEvent.hpp"
#include "../Graphics/GeometryStructs.hpp"

namespace ln {
namespace detail { class PlatformWindow; class UIManager; }
class SwapChain;
class UIRenderView;
class UIViewport;

class LN_API UIFrameWindow
	: public UIElement
	, public detail::IPlatforEventListener
{
public:

	virtual void dispose() override;

	void renderContents();
	void present();

	SwapChain* swapChain() const;


	// TODO: UI ツリーのシステムをちゃんと作っていないが、ひとまず UIViewport を使った動きを作りたいので一時的に設けてある
	Ref<UIViewport> m_viewport;
	virtual int getVisualChildrenCount() const override { return 1; }
	virtual UIElement* getVisualChild(int index) const override;

    // TODO: inernal
    void updateLayout();

LN_CONSTRUCT_ACCESS:
	UIFrameWindow();
	virtual ~UIFrameWindow();
	void initialize(detail::PlatformWindow* platformMainWindow, const SizeI& backbufferSize);

private:
	virtual bool onPlatformEvent(const detail::PlatformEventArgs& e) override;

	detail::UIManager* m_manager;
	Ref<detail::PlatformWindow>	m_platformWindow;
	Ref<SwapChain>	m_swapChain;
	Ref<UIRenderView> m_renderView;

	Event<UIEventHandler> m_onClosed;

};

} // namespace ln

