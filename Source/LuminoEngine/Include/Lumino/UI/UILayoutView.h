﻿
#pragma once
#include "UIInjectedInputReceiver.h"
#include "UIContentControl.h"

LN_NAMESPACE_BEGIN
class DrawList;
class PlatformWindow;

/**
	@brief		
	@details	UILayoutView はネイティブウィンドウと UI システムの接合点となり、
				UI レイアウトのルート要素を保持します。
*/
class UILayoutView		// TODO: これ = UILayoutRoot でよい。
	: public UIContentControl
	, public IUIInjectedInputReceiver
{
public:

	/** この要素が関連付けられている UIContext を取得します。*/
	UIContext* GetOwnerContext() const { return m_ownerContext; }

	//UILayoutRoot* GetLayoutRoot() const { return m_rootElement; }

protected:
	// UIElement interface
	virtual void ActivateInternal(UIElement* child);

LN_INTERNAL_ACCESS:
	void UpdateLayout(const Size& viewSize);
	void Render(DrawList* g);
	
	const Size& GetViewPixelSize() const { return m_viewPixelSize; }
	bool UpdateMouseHover(const PointF& mousePos);


	// Implements IUIInjectedInputReceiver
	//virtual bool InjectViewportSizeChanged(int width, int height) override;
	virtual bool InjectMouseMove(float clientX, float clientY) override;
	virtual bool InjectMouseButtonDown(MouseButtons button, float clientX, float clientY) override;
	virtual bool InjectMouseButtonUp(MouseButtons button, float clientX, float clientY) override;
	virtual bool InjectMouseWheel(int delta) override;
	virtual bool InjectKeyDown(Keys keyCode, ModifierKeys modifierKeys) override;
	virtual bool InjectKeyUp(Keys keyCode, ModifierKeys modifierKeys) override;
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
	void Initialize(UIContext* ownerContext, UIFrameWindow* ownerNativeWindow);

	UIFrameWindow*		m_ownerNativeWindow;

	UIContext*			m_ownerContext;
	//UILayoutRoot*		m_rootElement;
	UIElement*			m_mouseHoverElement;

	PointF				m_mousePosition;
	MouseClickTracker	m_mouseClickTrackers[8];

	Size				m_viewPixelSize;
};

LN_NAMESPACE_END
