#pragma once
#include "../UIItemsElement.hpp"

namespace ln {


enum class UIListSelectionMoveMode
{
	EdgeStop,
	Cyclic,
};
	
/**
 * UIListItemsControl 内の選択可能な項目を表します。
 */
class UIListItem
	: public UIControl
{
public:
	// TODO: group

	/** Submit イベントの通知を受け取るコールバックを登録します。*/
	//LN_METHOD(Event)
	Ref<EventConnection> connectOnSubmit(Ref<UIEventHandler> handler);


protected:
	virtual void onSubmit();	// TODO: double click
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
	Event<UIEventHandler> m_onSubmit;
	bool m_isSelected;

	friend class UIListItemsControl;
};


/**
 * 要素を線形に保持し、選択できるようにするシーケンスコンテナのベースクラスです。
 *
 * LogicalChildren を UIListItem のリストとして扱います。
 *
 * @note
 *   UIListBox, UIListView, UIMenuItem, UIComboBox, UITabBar
 */
class UIListItemsControl
	: public UIControl
{
public:
	void selectItem(UIListItem* item);
	UIListItem* selectedItem() const;
	void setItemsLayoutPanel(UILayoutPanel2* layout);

protected:
	void addListItem(UIListItem* item);
	void removeListItem(UIListItem* item);
	void removeAllItems();

	// base interfaces
	void onRoutedEvent(UIEventArgs* e) override;
	Size measureOverride(UILayoutContext* layoutContext, const Size& constraint) override;
	Size arrangeOverride(UILayoutContext* layoutContext, const Rect& finalArea) override;

LN_CONSTRUCT_ACCESS:
    UIListItemsControl();
    bool init();

private:
	void notifyItemClicked(UIListItem* item);
	void selectItemExclusive(UIListItem* item);

	Ref<UILayoutPanel2> m_itemsHostLayout;
	List<UIListItem*> m_selectedItems;
	UIListSelectionMoveMode m_selectionMoveMode;

	friend class UIListItem;
};


//--------------------------------------

/**
 * UIListBox 内の選択可能な項目を表します。
 */
class UIListBoxItem
    : public UIListItem
{
public:
	static Ref<UIListBoxItem> create(StringRef text);


    // TODO: group


	void bind(ObservablePropertyBase* prop);

protected:
	// base interface
	virtual const String& elementName() const  override { static String name = u"UIListBoxItem"; return name; }

LN_CONSTRUCT_ACCESS:
    UIListBoxItem();
    bool init();
	bool init(StringRef text);

private:
};


// 任意サイズの Item をリスト形式で扱う。
// サイズがばらばらでもいいので、仮想化は非対応。少量のフレキシブルなリストに使う。
// 固定サイズで仮想化対応するのは UIListView
class UIListBox
	: public UIListItemsControl
{
public:
    static Ref<UIListBox> create();

	/** UIListBoxItem を追加し、そのインスタンスを返します。 */
	UIListBoxItem* addItem(const ln::String& text);

	// この UIElement のメイン要素と bind する Property を設定する。
	// 具体的にどのような表示要素と bind するのかは実装により異なる。
	// 例えば ListBox なら、Collection を bind することで ListBoxItem を生成する。
	// LsitBoxItem なら、デフォルトで作成される TextBlock に bind する。
	// ListBoxItem をカスタマイズしたいときは、今は未実装だが、generate() みたいなのをユーザープログラムで実装し、そのなかで main の prop を参照することになりそう。
	// WPF でいうところの DataContext = prop.
	void bind(ObservablePropertyBase* prop);

protected:
	// base interface
	virtual const String& elementName() const  override { static String name = u"UIListBox"; return name; }
    virtual void onAddChild(UIElement* child) override;



LN_CONSTRUCT_ACCESS:
	UIListBox();
	bool init();

private:
};

} // namespace ln

