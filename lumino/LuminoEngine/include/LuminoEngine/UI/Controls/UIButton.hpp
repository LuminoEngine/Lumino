#pragma once
#include "../UIContainerElement.hpp"

namespace ln {
class UIText;

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
	bool init();

	/** set text. */
	LN_METHOD()
    void setText(const StringView& text);

    void setCommand(UICommand* command);

protected:
	virtual void onClick(UIEventArgs* e);

    virtual void onRoutedEvent(UIEventArgs* e) override;

private:
    Ref<UIText> m_textContent;
    Ref<UICommand> m_command;
    bool m_isPressed;
};

/** UIButton */
LN_CLASS()
class UIButton
	: public UIButtonBase
{
	LN_OBJECT;
	LN_BUILDER;
public:
    static Ref<UIButton> create();
    static Ref<UIButton> create(const StringView& text);

	UIButton();

	/** init. */
	LN_METHOD()
	bool init();

	/** 表示文字列を指定して UIButton を作成します。 */
	LN_METHOD(OverloadPostfix = "WithText")
    void init(const StringView& text);

    /** Clicked イベントの通知を受け取るコールバックを登録します。*/
    //LN_METHOD(Event)
	//Ref<EventConnection> connectOnClicked(UIGeneralEventHandler handler);

	/** Clicked イベントの通知を受け取るコールバックを登録します。*/
	LN_METHOD(Event)
	Ref<EventConnection> connectOnClicked(Ref<UIEventHandler> handler);
	

protected:
    virtual const String& elementName() const override { static String name = _TT("UIButton"); return name; }
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
	bool init();

protected:
	virtual const String& elementName() const  override { static String name = _TT("UIToggleButton"); return name; }
	virtual void onClick(UIEventArgs* e) override;

private:
	void checkChanged();

	UICheckState m_checkState;
	Event<UIGeneralEventHandler> m_onChecked;
	Event<UIGeneralEventHandler> m_onUnchecked;
};


//==============================================================================
// UIButton::Builder

struct UIButton::BuilderDetails : public UIControl::BuilderDetails
{
	LN_BUILDER_DETAILS(UIButton);

	String text;

	void apply(UIButton* p) const;
};

template<class T, class B, class D>
struct UIButton::BuilderCore : public UIControl::BuilderCore<T, B, D>
{
	LN_BUILDER_CORE(UIControl::BuilderCore);

	B& text(StringView value) { d()->text = value; return self(); }
};

struct UIButton::Builder : public BuilderCore<UIButton, Builder, BuilderDetails>
{
	Builder() {}
	Builder(StringView text) { d()->text = text; }
};

} // namespace ln

