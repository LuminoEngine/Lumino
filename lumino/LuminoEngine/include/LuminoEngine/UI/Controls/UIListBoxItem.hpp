#pragma once
#include "../Common.hpp"
#include "../UIItemsElement.hpp"

namespace ln {


enum class UIListSelectionMoveMode
{
	EdgeStop,
	Cyclic,
};

/** UIListSubmitMode */
LN_ENUM();
enum class UIListSubmitMode
{
	/** ゲームUI用。Hover で選択状態、シングルクリックで Submit. Hover 状態は使用されない。 */
	Single,

	/** エディタUI用。シングルクリックで選択状態、ダブルクリックで Submit. */
	Double,
};
	
/**
 * UIListItemsControl 内の選択可能な項目を表します。
 */
LN_CLASS()
class UIListItem
	: public UIControl
{
	LN_OBJECT;
public:
	// TODO: group

	/** Submit イベントの通知を受け取るコールバックを登録します。*/
	LN_METHOD(Event)
	Ref<EventConnection> connectOnSubmit(Ref<UIGeneralEventHandler> handler);


protected:
	virtual void onSubmit();
	virtual void onSelected(UIEventArgs* e);
	virtual void onUnselected(UIEventArgs* e);

	// base interface
	void onRoutedEvent(UIEventArgs* e) override;

LN_CONSTRUCT_ACCESS:
	UIListItem();
	bool init();

private:
	void setSelectedInternal(bool selected);

	UIListItemsControl* m_ownerListControl;
	Event<UIGeneralEventHandler> m_onSubmit;
	bool m_isSelected;

	friend class UIListItemsControl;
};


/**
 * UIListBox 内の選択可能な項目を表します。
 */
LN_CLASS()
class UIListBoxItem
    : public UIListItem
{
	LN_OBJECT;
	LN_BUILDER;
public:
	static Ref<UIListBoxItem> create(StringView text);


    // TODO: group


	void bind(ObservablePropertyBase* prop);

protected:
	// base interface
	virtual const String& elementName() const  override { static String name = _TT("UIListBoxItem"); return name; }

LN_CONSTRUCT_ACCESS:
    UIListBoxItem();
    bool init();
	bool init(StringView text);

	/** init */
	LN_METHOD()
	bool init(UIElement* content);
	

private:
};

struct UIListBoxItem::BuilderDetails : public UIListItem::BuilderDetails
{
	LN_BUILDER_DETAILS(UIListBoxItem);

	String text;
	Ref<UIGeneralEventHandler> onSubmit;

	void apply(UIListBoxItem* p) const;
};

template<class T, class B, class D>
struct UIListBoxItem::BuilderCore : public UIListItem::BuilderCore<T, B, D>
{
	LN_BUILDER_CORE(UIListItem::BuilderCore);

	B& text(StringView value) { d()->text = value; return self(); }

	B& onSubmit(Ref<UIGeneralEventHandler> value) { d()->onSubmit = value; return self(); }
};

LN_BUILDER_IMPLEMENT(UIListBoxItem);

} // namespace ln

