/**
	@file	GUIContext.h	
*/
#ifndef LUMINO_GUI_GUICONTEXT_H
#define LUMINO_GUI_GUICONTEXT_H

#include "Common.h"
#include "RoutedEventArgs.h"
#include "UIInjectedInputReceiver.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GUI_BEGIN
	
/**
	@brief		
*/
class GUIContext
	: public CoreObject
	, public IUIInjectedInputReceiver
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
public:

	/**
		@brief	メインウィンドウのトップレベルに配置される GUIContext を作成します。
		*/
	static GUIContext* Create();

public:

	/**
		@brief	このコンテキストのルート GUI 要素を設定します。
		*/
	void SetRootElement(UIElement* element);

	/**
		@brief	このコンテキストのルート GUI 要素を取得します。
		*/
	UIElement* GetRootElement() const;

	// Implements IUIInjectedInputReceiver
	virtual bool InjectViewportSizeChanged(int width, int height);
	virtual bool InjectMouseMove(float clientX, float clientY);
	virtual bool InjectMouseButtonDown(MouseButton button, float clientX, float clientY);
	virtual bool InjectMouseButtonUp(MouseButton button, float clientX, float clientY);
	virtual bool InjectMouseWheel(int delta, float clientX, float clientY);
	virtual bool InjectKeyDown(Key keyCode, bool isAlt, bool isShift, bool isControl);
	virtual bool InjectKeyUp(Key keyCode, bool isAlt, bool isShift, bool isControl);
	virtual bool InjectTextInput(TCHAR ch);
	virtual void InjectElapsedTime(float elapsedTime);

LN_INTERNAL_ACCESS:
	friend class GUIManagerImpl;
	friend class GUIHelper;
	GUIContext(GUIManagerImpl* manager);
	virtual ~GUIContext();
	const Size& GetViewPixelSize() const;
	void UpdateLayout();
	void Render();

	void SetFocusElement(UIElement* element);
	void CaptureMouse(UIElement* element);
	void ReleaseMouseCapture(UIElement* element);
	bool UpdateMouseHover(const PointF& mousePos);
	void AddAnimationClock(AnimationClock* clock);
	void RemoveAnimationClock(AnimationClock* clock);
	double GetTime() const { return m_time; }

private:

	struct MouseClickTracker
	{
		double		LastTime;
		int			ClickCount;
		UIElement*	HoverElement;
	};

	GUIManagerImpl*				m_manager;
	RefPtr<Platform::Window>	m_nativeWindow;
	Size						m_viewPixelSize;
	RootFrame*					m_rootFrame;
	UIElement*					m_rootElement;

	UIElement*					m_focusElement;
	UIElement*					m_mouseHoverElement;
	UIElement*					m_capturedElement;

	PointF						m_mousePosition;
	MouseClickTracker			m_mouseClickTrackers[MouseButton::TERMINATOR];
	float						m_mouseButtonClickTimeout;

	double						m_time;
	
	Array<AnimationClock*>		m_activeAnimationClockList;	///< TODO: インデックススタックを使ったリストにしたい

	CursorImage*				m_currentCursorImage;
	float						m_cursorAnimationTime;

	bool						m_onMainWindow;
};

LN_NAMESPACE_GUI_END
LN_NAMESPACE_END

#endif // LUMINO_GUI_GUICONTEXT_H
