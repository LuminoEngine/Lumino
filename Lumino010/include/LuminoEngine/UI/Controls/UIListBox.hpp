#pragma once
#include "../Common.hpp"
#include "../UIItemsElement.hpp"
#include "UIListBoxItem.hpp"

namespace ln {

/**
 * 要素を線形に保持し、選択できるようにするシーケンスコンテナのベースクラスです。
 *
 * LogicalChildren を UIListItem のリストとして扱います。
 *
 * @note
 *   UIListBox, UIListView, UIMenuItem, UIComboBox, UITabBar
 */
LN_CLASS()
class UIListItemsControl
	: public UIControl
{
	LN_OBJECT;
	LN_BUILDER;
public:
	void selectItem(UIListItem* item);
	UIListItem* selectedItem() const;

	/** setItemsLayoutPanel */
	LN_METHOD(Property)
	void setItemsLayoutPanel(UILayoutPanel* layout);

	/** itemsLayoutPanel */
	LN_METHOD(Property)
	UILayoutPanel* itemsLayoutPanel() const;

    /** UIListSubmitMode (default: Single) */
	LN_METHOD(Property)
    void setSubmitMode(UIListSubmitMode value);

    /** UIListSubmitMode */
	LN_METHOD(Property)
	UIListSubmitMode submitMode() const;

	/** Submit イベントの通知を受け取るコールバックを登録します。*/
	LN_METHOD(Event)
	Ref<EventConnection> connectOnSubmit(Ref<UIGeneralEventHandler> handler);


protected:
	void addListItem(UIListItem* item);
	void removeListItem(UIListItem* item);
	void removeAllItems();
	virtual void submitItem(UIListItem* item);

	// base interfaces
	void onRoutedEvent(UIEventArgs* e) override;
	Size measureOverride(UILayoutContext* layoutContext, const Size& constraint) override;
	Size arrangeOverride(UILayoutContext* layoutContext, const Rect& finalArea) override;

LN_CONSTRUCT_ACCESS:
    UIListItemsControl();
    bool init();

private:
	void notifyItemClicked(UIListItem* item, int clickCount);
	void selectItemExclusive(UIListItem* item);

	Ref<UILayoutPanel> m_itemsHostLayout;
	List<UIListItem*> m_selectedItems;
	UIListSelectionMoveMode m_selectionMoveMode;
	UIListSubmitMode m_submitMode;
	Event<UIGeneralEventHandler> m_onSubmit;

	friend class UIListItem;
};

struct UIListItemsControl::BuilderDetails : public UIControl::BuilderDetails
{
	LN_BUILDER_DETAILS(UIListItemsControl);

	Ref<UIGeneralEventHandler> onSubmit;

	void apply(UIListItemsControl* p) const;
};

template<class T, class B, class D>
struct UIListItemsControl::BuilderCore : public UIControl::BuilderCore<T, B, D>
{
	LN_BUILDER_CORE(UIControl::BuilderCore);

	B& onSubmit(Ref<UIGeneralEventHandler> value) { d()->onSubmit = value; return self(); }
};

LN_BUILDER_IMPLEMENT(UIListItemsControl);


// 任意サイズの Item をリスト形式で扱う。
// サイズがばらばらでもいいので、仮想化は非対応。少量のフレキシブルなリストに使う。
// 固定サイズで仮想化対応するのは UIListView
/**
 * UIListBox
 */
LN_CLASS()
class UIListBox
	: public UIListItemsControl
{
	LN_OBJECT;
	LN_BUILDER;
public:
    static Ref<UIListBox> create();

	/** UIListBoxItem を追加し、そのインスタンスを返します。 */
	UIListBoxItem* addItem(const ln::String& text, Ref<Variant> data = nullptr);

	/** UIListBoxItem を追加し、そのインスタンスを返します。 */
	LN_METHOD()
	UIListBoxItem* addItem(UIElement* content);

	// この UIElement のメイン要素と bind する Property を設定する。
	// 具体的にどのような表示要素と bind するのかは実装により異なる。
	// 例えば ListBox なら、Collection を bind することで ListBoxItem を生成する。
	// LsitBoxItem なら、デフォルトで作成される TextBlock に bind する。
	// ListBoxItem をカスタマイズしたいときは、今は未実装だが、generate() みたいなのをユーザープログラムで実装し、そのなかで main の prop を参照することになりそう。
	// WPF でいうところの DataContext = prop.
	void bind(ObservablePropertyBase* prop);

protected:
	// base interface
	const String& elementName() const  override { static String name = u"UIListBox"; return name; }
    void onAddChild(UIElement* child) override;
	Size measureOverride(UILayoutContext* layoutContext, const Size& constraint) override;
	Size arrangeOverride(UILayoutContext* layoutContext, const Rect& finalArea) override;
	void onRoutedEvent(UIEventArgs* e) override;

	//virtual void render(UIRenderingContext* context, const Matrix& parentTransform)
	//{
	//	UIListItemsControl::render(context, parentTransform);
	//}
	//void onRender(UIRenderingContext* context) override
	//{

	//}

LN_CONSTRUCT_ACCESS:
	UIListBox();

	/** init */
	LN_METHOD()
	bool init();

private:
	Ref<UIScrollViewHelper> m_scrollViewHelper;
};

struct UIListBox::BuilderDetails : public UIListItemsControl::BuilderDetails
{
	LN_BUILDER_DETAILS(UIListBox);

	void apply(UIListBox* p) const;
};

template<class T, class B, class D>
struct UIListBox::BuilderCore : public UIListItemsControl::BuilderCore<T, B, D>
{
	LN_BUILDER_CORE(UIListItemsControl::BuilderCore);
};

LN_BUILDER_IMPLEMENT(UIListBox);

} // namespace ln

