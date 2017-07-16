
#pragma once
#include "UIInjectedInputReceiver.h"
#include "UIItemsControl.h"

LN_NAMESPACE_BEGIN
class DrawingContext;
class PlatformWindow;
class UIPopup;

namespace detail {


class UIPopuoContainer
	: public Object
{
public:
	void SetPopup(UIPopup* popup);
	UIPopup* GetPopup() const;

LN_CONSTRUCT_ACCESS:
	UIPopuoContainer();
	virtual ~UIPopuoContainer();
	void initialize();

private:
	Ref<UIPopup>	m_popup;
};

} // namespace detail

/**
	@brief		
	@details	UILayoutView はネイティブウィンドウと UI システムの接合点となり、
				UI レイアウトのルート要素となります。
*/
class UILayoutView
	: public UIControl
	//, public IUIInjectedInputReceiver
{
public:

	/** この要素が関連付けられている UIContext を取得します。*/
	UIContext* getOwnerContext() const { return m_ownerContext; }



LN_INTERNAL_ACCESS:
	void updateLayout(const Size& viewSize);
	void render(DrawingContext* g);
	
	//const Size& getViewPixelSize() const { return m_viewPixelSize; }
	bool updateMouseHover(const Point& mousePos);
	//void captureMouse(UIElement* element);
	//void releaseMouseCapture(UIElement* element);


	// Popup
	void openPopup(UIPopup* popup);
	void closePopup(UIPopup* popup);


	bool injectMouseMove(float clientX, float clientY);
	bool injectMouseButtonDown(MouseButtons button, float clientX, float clientY);
	bool injectMouseButtonUp(MouseButtons button, float clientX, float clientY);
	bool injectMouseWheel(int delta);
	bool injectKeyDown(Keys keyCode, ModifierKeys modifierKeys);
	bool injectKeyUp(Keys keyCode, ModifierKeys modifierKeys);
	bool injectTextInput(TCHAR ch);

LN_CONSTRUCT_ACCESS:
	UILayoutView();
	virtual ~UILayoutView();
	void initialize(UIContext* ownerContext, PlatformWindow* ownerNativeWindow);

LN_INTERNAL_ACCESS:
	virtual detail::SpcialUIElementType getSpcialUIElementType() const;

private:
	friend class UIContext;

	struct MouseClickTracker
	{
		double		LastTime = 0.0;
		int			ClickCount = 0;
		UIElement*	HoverElement = nullptr;
	};


	PlatformWindow*		m_ownerNativeWindow;

	UIContext*			m_ownerContext;
	UIElement*			m_mouseHoverElement;

	Point				m_mousePosition;
	MouseClickTracker	m_mouseClickTrackers[8];

	List<Ref<detail::UIPopuoContainer>>	m_popupContainers;
};

LN_NAMESPACE_END
