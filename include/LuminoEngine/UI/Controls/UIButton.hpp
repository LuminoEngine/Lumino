#pragma once
#include "../UIContainerElement.hpp"

namespace ln {
class UITextBlock;

enum class UICheckState
{
	Unchecked,
	Indeterminate,
	Checked,
};

/** UIButtonBase */
LN_CLASS()
class UIButtonBase
	: public UIControl
{
	LN_OBJECT;
public:
	UIButtonBase();
	bool init() { return init(UICreationContext::Default); }
	bool init(const UICreationContext* context);

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

/** UIButton */
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
	bool init() { return init(UICreationContext::Default); }
	bool init(const UICreationContext* context);

	/** 表示文字列を指定して UIButton を作成します。 */
	LN_METHOD(OverloadPostfix = "WithText")
    void init(const StringRef& text);

    /** Clicked イベントの通知を受け取るコールバックを登録します。*/
    //LN_METHOD(Event)
	//Ref<EventConnection> connectOnClicked(UIGeneralEventHandler handler);

	/** Clicked イベントの通知を受け取るコールバックを登録します。*/
	LN_METHOD(Event)
	Ref<EventConnection> connectOnClicked(Ref<UIEventHandler> handler);
	

protected:
    virtual const String& elementName() const override { static String name = u"UIButton"; return name; }
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
	//LN_METHOD(Event)
	Ref<EventConnection> connectOnChecked(Ref<UIGeneralEventHandler> handler);

	/** Unchecked イベントの通知を受け取るコールバックを登録します。*/
	//LN_METHOD(Event)
	Ref<EventConnection> connectOnUnchecked(Ref<UIGeneralEventHandler> handler);

LN_CONSTRUCT_ACCESS:
	UIToggleButton();
	bool init() { return init(UICreationContext::Default); }
	bool init(const UICreationContext* context);

protected:
	virtual const String& elementName() const  override { static String name = u"UIToggleButton"; return name; }
	virtual void onClick(UIEventArgs* e) override;

private:
	void checkChanged();

	UICheckState m_checkState;
	Event<UIGeneralEventHandler> m_onChecked;
	Event<UIGeneralEventHandler> m_onUnchecked;
};

} // namespace ln

