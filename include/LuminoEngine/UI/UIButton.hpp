#pragma once
#include "UIContainerElement.hpp"

namespace ln {
class UITextBlock;

enum class UICheckState
{
	Unchecked,
	Indeterminate,
	Checked,
};

LN_CLASS()
class UIButtonBase
	: public UIControl
{
	LN_OBJECT;
public:
	UIButtonBase();
	void init();

	/** set text. */
	LN_METHOD()
    void setText(const StringRef& text);

    void setCommand(UICommand* command);

protected:
	virtual void onClick(UIEventArgs* e);

    virtual void onRoutedEvent(UIEventArgs* e) override;

private:
    Ref<UITextBlock> m_textContent;
    Ref<UICommand> m_command;
    bool m_isPressed;
};

LN_CLASS()
class UIButton
	: public UIButtonBase
{
	LN_OBJECT;
public:
    static Ref<UIButton> create();
    static Ref<UIButton> create(const StringRef& text);

	UIButton();

	/** init. */
	LN_METHOD()
	void init();

    void init(const StringRef& text);

    /** Clicked イベントの通知を受け取るコールバックを登録します。*/
    LN_METHOD(Event)
    EventConnection connectOnClicked(UIEventHandler handler);   // TODO: Click

protected:
    virtual const String& elementName() const  override { static String name = u"UIButton"; return name; }
    virtual void onClick(UIEventArgs* e) override;

private:
    Event<UIEventHandler> m_onClicked;
};

class UIToggleButton
	: public UIButtonBase
{
public:

	void setChecked(bool checked);
	bool isChecked() const;
	

	/** Checked イベントの通知を受け取るコールバックを登録します。*/
	LN_METHOD(Event)
	EventConnection connectOnChecked(UIEventHandler handler);

	/** Unchecked イベントの通知を受け取るコールバックを登録します。*/
	LN_METHOD(Event)
	EventConnection connectOnUnchecked(UIEventHandler handler);

LN_CONSTRUCT_ACCESS:
	UIToggleButton();
	void init();

protected:
	virtual const String& elementName() const  override { static String name = u"UIToggleButton"; return name; }
	virtual void onClick(UIEventArgs* e) override;

private:
	void checkChanged();

	UICheckState m_checkState;
	Event<UIEventHandler> m_onChecked;
	Event<UIEventHandler> m_onUnchecked;
};

} // namespace ln

