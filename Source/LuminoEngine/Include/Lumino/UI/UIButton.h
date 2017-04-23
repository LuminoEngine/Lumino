
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
	static RefPtr<UIButton> Create(const StringRef& text, float width, float height);

LN_CONSTRUCT_ACCESS:
	UIButton();
	virtual ~UIButton();
	void Initialize();
	void Initialize(const StringRef& text, float width, float height);
};


/**
	@brief		
*/
class UIToggleButton
	: public UIButtonBase
{
	LN_UI_TYPEINFO_DECLARE();
public:
	static const String CheckedState;
	static const String UncheckedState;

	static RefPtr<UIToggleButton> Create();

protected:
	virtual void OnClick(UIEventArgs* e);

LN_CONSTRUCT_ACCESS:
	UIToggleButton();
	virtual ~UIToggleButton();
	void Initialize();

private:
	bool	m_isChecked;
};

LN_NAMESPACE_END
