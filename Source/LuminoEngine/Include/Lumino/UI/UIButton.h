
#pragma once
#include "UIControl.h"

LN_NAMESPACE_BEGIN
class UIButton;
using UIButtonPtr = RefPtr<UIButton>;

/** ボタンのクリックイベントを発生させるタイミングを表します。*/
enum class ClickMode
{
	/** ボタンを離したときにイベントを発生させます。*/
	Release,

	/** ボタンを押したときにイベントを発生させます。*/
	Press,
};

/**
	@brief		
*/
class UIButtonBase
	: public UIControl
{
	LN_UI_TYPEINFO_DECLARE();
public:
	void SetText(const StringRef& text);

	/** OnClick イベントの通知を受け取るコールバックを登録します。*/
	LN_METHOD(Event)
	EventConnection ConnectOnGotFocus(UIEventHandler handler);

protected:

	/** ボタンがクリックされたときに呼び出されます。*/
	virtual void OnClick(UIEventArgs* e);

	// UIElement interface
	virtual void OnMouseDown(UIMouseEventArgs* e) override;
	virtual void OnMouseUp(UIMouseEventArgs* e) override;

LN_CONSTRUCT_ACCESS:
	UIButtonBase();
	virtual ~UIButtonBase();
	void Initialize();

private:
	ClickMode	m_clickMode;
	bool		m_isPressed;

	UIEventHandler::EventType	m_onClick;
};

/**
	@brief		
*/
class UIButton
	: public UIButtonBase
{
	LN_UI_TYPEINFO_DECLARE();
public:
	static RefPtr<UIButton> Create();

LN_CONSTRUCT_ACCESS:
	UIButton();
	virtual ~UIButton();
	void Initialize();
};


///**
//	@brief		
//*/
//class UIToggleButton
//	: public UIContentControl
//{
//	LN_UI_TYPEINFO_DECLARE();
//public:
//	LN_ROUTED_EVENT(UIMouseEventArgs, ClickEvent);		/**< Click ルーティングイベントの識別子 */
//
//public:
//	static UIButtonPtr Create();
//
//public:
//	UIEvent<UIMouseEventArgs>		click;		/**< */
//
//	void SetText(const StringRef& text);
//
//LN_CONSTRUCT_ACCESS:
//	UIButton();
//	virtual ~UIButton();
//	void Initialize(detail::UIManager* manager);
//
//protected:
//	virtual void OnClick();
//
//	// UIElement interface
//	virtual void OnRoutedEvent(const UIEventInfo* ev, UIEventArgs* e) override;
//	virtual void OnMouseDown(UIMouseEventArgs* e) override;
//	virtual void OnMouseUp(UIMouseEventArgs* e) override;
//
//private:
//	bool	m_isPressed;
//};

LN_NAMESPACE_END
