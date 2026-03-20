#pragma once
#include "../Common.hpp"
#include "../UIItemsElement.hpp"
#include "UIListBoxItem.hpp"

namespace ln {
class UITreeBox;

/**
 * 
 * 
 * UITreeBoxItem の LogicalParent 及び VisualParent は UITreeBox です。
 * 親 Item を取得する場合は parentItem() を使用してください。
 */
LN_CLASS()
class UITreeBoxItem : public UIControl
{
	LN_OBJECT;
public:

	UITreeBoxItem* addItem(const ln::String& text);
	
	/** Submit イベントの通知を受け取るコールバックを登録します。*/
	LN_METHOD(Event)
	Ref<EventConnection> connectOnSubmit(Ref<UIGeneralEventHandler> handler);

protected:
	const String& elementName() const override { static String name = _TT("UITreeBoxItem"); return name; }
	Size measureOverride(UILayoutContext* layoutContext, const Size& constraint) override;
	Size arrangeOverride(UILayoutContext* layoutContext, const Rect& finalArea) override;
	void onRoutedEvent(UIEventArgs* e) override;

	virtual void onSubmit();

LN_CONSTRUCT_ACCESS:
	UITreeBoxItem();
	bool init();

private:
	void addItemInternal(UITreeBoxItem* item);
	void traverse(int depth, const std::function<void(UITreeBoxItem*)> func);

	UITreeBox* m_ownerTreeBox;
	UITreeBoxItem* m_parentItem;
	List<Ref<UITreeBoxItem>> m_childItems;
	int m_layoutDepth;
	Event<UIGeneralEventHandler> m_onSubmit;

	friend class UITreeBox;
};

// 任意サイズの Item をリスト形式で扱う。
// サイズがばらばらでもいいので、仮想化は非対応。少量のフレキシブルなリストに使う。
// 固定サイズで仮想化対応するのは UIListView
/**
 * UITreeBox
 * 
 * - 仮想化は行わない。全てのアイテムに対応する UITreeBoxItem のインスタンスを保持する。
 * - 可変サイズの UITreeBoxItem に対応可能。
 * - 少量のアイテムに適する。
 * - レイアウトは Stack 固定
 */
LN_CLASS()
class UITreeBox
	: public UIControl
{
	LN_OBJECT;
public:
	/** UITreeBoxItem を追加し、そのインスタンスを返します。 */
	UITreeBoxItem* addItem(const ln::String& text, Ref<Variant> data = nullptr);

	/** UITreeBoxItem を追加し、そのインスタンスを返します。 */
	//UITreeBoxItem* addItem(UIElement* content);

protected:
	// base interface
	const String& elementName() const  override { static String name = _TT("UITreeBox"); return name; }
	Size measureOverride(UILayoutContext* layoutContext, const Size& constraint) override;
	Size arrangeOverride(UILayoutContext* layoutContext, const Rect& finalArea) override;
	//void onRoutedEvent(UIEventArgs* e) override;

	//virtual void render(UIRenderingContext* context, const Matrix& parentTransform)
	//{
	//	UIListItemsControl::render(context, parentTransform);
	//}
	//void onRender(UIRenderingContext* context) override
	//{

	//}

LN_CONSTRUCT_ACCESS:
	UITreeBox();

	/** init */
	LN_METHOD()
	bool init();

private:
	void requireRefresh();
	void addItemInternal(UITreeBoxItem* item);
	void refresh();
	void notifyItemClicked(UITreeBoxItem* item, int clickCount);

	Ref<UIScrollViewHelper> m_scrollViewHelper;
	List<Ref<UITreeBoxItem>> m_rootItems;
	bool m_needRefresh;

	friend class UITreeBoxItem;
};


} // namespace ln

