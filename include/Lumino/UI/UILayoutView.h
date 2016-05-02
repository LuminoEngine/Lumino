
#pragma once
#include "UIInjectedInputReceiver.h"
#include "UIElement.h"

LN_NAMESPACE_BEGIN

/**
	@brief		
	@details	UILayoutView はネイティブウィンドウと UI システムの接合点となり、
				UI レイアウトのルート要素を保持します。
*/
class UILayoutView
	: public RefObject
	, public IUIInjectedInputReceiver
{
public:

	/** この要素が関連付けられている UIContext を取得します。*/
	UIContext* GetOwnerContext() { return m_ownerContext; }



LN_INTERNAL_ACCESS:
	void UpdateLayout(const SizeF& viewSize);
	void Render(GraphicsContext* g);
	
	const SizeF& GetViewPixelSize() const { return m_viewPixelSize; }
	bool UpdateMouseHover(const PointF& mousePos);
	//void CaptureMouse(UIElement* element);
	//void ReleaseMouseCapture(UIElement* element);


	// Implements IUIInjectedInputReceiver
	//virtual bool InjectViewportSizeChanged(int width, int height) override;
	virtual bool InjectMouseMove(float clientX, float clientY) override;
	virtual bool InjectMouseButtonDown(MouseButton button, float clientX, float clientY) override;
	virtual bool InjectMouseButtonUp(MouseButton button, float clientX, float clientY) override;
	virtual bool InjectMouseWheel(int delta) override;
	virtual bool InjectKeyDown(Key keyCode, ModifierKeys modifierKeys) override;
	virtual bool InjectKeyUp(Key keyCode, ModifierKeys modifierKeys) override;
	virtual bool InjectTextInput(TCHAR ch) override;

private:
	friend class UIContext;

	struct MouseClickTracker
	{
		double		LastTime = 0.0;
		int			ClickCount = 0;
		UIElement*	HoverElement = nullptr;
	};

	UILayoutView();
	virtual ~UILayoutView();
	void Initialize(UIContext* ownerContext, PlatformWindow* ownerNativeWindow);

	PlatformWindow*		m_ownerNativeWindow;

	UIContext*			m_ownerContext;
	UILayoutRoot*		m_rootElement;
	UIElement*			m_mouseHoverElement;
	UIElement*			m_capturedElement;

	PointF				m_mousePosition;
	MouseClickTracker	m_mouseClickTrackers[MouseButton::TERMINATOR];

	SizeF				m_viewPixelSize;
};

LN_NAMESPACE_END
