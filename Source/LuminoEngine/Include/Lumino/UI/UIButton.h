
#pragma once
#include "UIControl.h"

LN_NAMESPACE_BEGIN
class UIButton;
using UIButtonPtr = RefPtr<UIButton>;
class UITextBlock;

enum class UICheckState
{
	Unchecked,
	Indeterminate,
	Checked,
};

/**
	@brief		
*/
class UIButtonBase
	: public UIControl
{
	LN_OBJECT();
public:
	void setText(const StringRef& text);


protected:


	// UIElement interface
	virtual void onMouseDown(UIMouseEventArgs* e) override;
	virtual void onMouseUp(UIMouseEventArgs* e) override;

LN_CONSTRUCT_ACCESS:
	UIButtonBase();
	virtual ~UIButtonBase();
	void initialize();

private:
	RefPtr<UITextBlock>	m_textContent;

};

/**
	@brief		
*/
class UIButton
	: public UIButtonBase
{
	LN_OBJECT();
public:
	static RefPtr<UIButton> create();
	static RefPtr<UIButton> create(const StringRef& text, float width, float height);

LN_CONSTRUCT_ACCESS:
	UIButton();
	virtual ~UIButton();
	void initialize();
	void initialize(const StringRef& text, float width, float height);
};


/**
	@brief		
*/
class UIToggleButton
	: public UIButtonBase
{
	LN_OBJECT();
public:
	static const String CheckedState;
	static const String UncheckedState;

	static RefPtr<UIToggleButton> create();

	void setChecked(bool checked);
	bool isChecked() const;

	/** Checked イベントの通知を受け取るコールバックを登録します。*/
	LN_METHOD(Event)
	EventConnection connectOnChecked(UIEventHandler handler);

	/** Unchecked イベントの通知を受け取るコールバックを登録します。*/
	LN_METHOD(Event)
	EventConnection connectOnUnchecked(UIEventHandler handler);

protected:
	virtual void onClick(UIEventArgs* e);

LN_CONSTRUCT_ACCESS:
	UIToggleButton();
	virtual ~UIToggleButton();
	void initialize();

private:
	void checkChanged();

	UICheckState				m_checkState;
	UIEventHandler::EventType	m_onChecked;
	UIEventHandler::EventType	m_onUnchecked;
	//Nullable<bool>			m_isChecked;
	//UIEventHandler::EventType	m_onIndeterminate; 
};

LN_NAMESPACE_END
