
#pragma once
#include "UIInjectedInputReceiver.h"
#include "UIContentsControl.h"
#include "UIItemsControl.h"	// TODO: 必要？

LN_NAMESPACE_BEGIN
class DrawingContext;
class PlatformWindow;
class UIPopup;
class UIAdornerLayer;

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
	: public UIContentsControl
	//: public UIControl
	//, public IUIInjectedInputReceiver
{
public:

	/** この要素が関連付けられている UIContext を取得します。*/
	UIContext* getOwnerContext() const { return m_ownerContext; }

	virtual void updateLayout(const Size& viewSize) override;
	virtual detail::SpcialUIElementType getSpcialUIElementType() const;


protected:
	virtual Size measureOverride(const Size& constraint) override;
	virtual Size arrangeOverride(const Size& finalSize) override;

LN_INTERNAL_ACCESS:
	void render(DrawingContext* g);
	
	//const Size& getViewPixelSize() const { return m_viewPixelSize; }
	bool updateMouseHover(const Point& mousePos);
	//void captureMouse(UIElement* element);
	//void releaseMouseCapture(UIElement* element);

	const Ref<UIAdornerLayer>& getAdornerLayer() const { return m_adornerLayer; }

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

	const Point& getMousePosition() const { return m_mousePosition; }

LN_CONSTRUCT_ACCESS:
	UILayoutView();
	virtual ~UILayoutView();
	void initialize(UIContext* ownerContext, PlatformWindow* ownerNativeWindow);

private:
	friend class UIContext;

	struct MouseClickTracker
	{
		double		LastTime = 0.0;
		int			ClickCount = 0;
		UIElement*	HoverElement = nullptr;
	};


	PlatformWindow*		m_ownerNativeWindow;
	Ref<UIAdornerLayer>	m_adornerLayer;

	UIContext*			m_ownerContext;
	UIElement*			m_mouseHoverElement;

	Point				m_mousePosition;
	MouseClickTracker	m_mouseClickTrackers[8];

	List<Ref<detail::UIPopuoContainer>>	m_popupContainers;
};

LN_NAMESPACE_END
