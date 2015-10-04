/*
	・WPF
		ListBox
			Border	枠及び背景
				ScrollViewer
					Grid
						Rectangle	装飾
						ScrollContentPresenter
							ItemsPresenter
								VirtualizingStackPanel
									ListBoxItem
									ListBoxItem
									ListBoxItem
										Border
											ContentPresenter
												<いろいろ>
						ScrollBar	縦
						ScrollBar	横


	ListBoxItem の論理的な役目は、親の ListBox にマウス操作を伝えること。
	ListBox は ListBoxItem からの通知を受け取ったら、自分の設定 (複数選択可能か等) を考慮し、
	それぞれの Item の状態を更新する。


	ItemsControl	・・・ Items, ItemsPanel
		MenuBase
		Selector
		StatusBar
		TreeView

	Selector		・・・ SelectedIndex
		ComboBox
		ListBox
		TabControl


	また、http://pro.art55.jp/?eid=1139229 みたいにグループ化される場合は、

	ScrollViewer
		Grid
			ScrollContentPresenter
				ItemsPresenter
					StackPanel
						GroupItem
							..expanderとか...
							ContentPresenter
								ItemsPresenter
									ListBoxItem
									ListBoxItem
						GroupItem
						GroupItem
	となっていた。



	ItemsSource が NULL の場合は、
	Items への直接コントロールAdd を許可する。
	ソートも・・・

	というか、ItemsSource が NULL かどうかで
	内部的なモードがざっくり変わるようなかんじ。


	WPF のアイテム生成までの流れ

	・ItemContainerGenerator.PrepareGrouping() で、owner の ListBox.View (ItemsSourceっぽい) を取得。
	・ItemContainerGenerator は ↑で取得した View に OnCollectionChanged を登録。
	・コレクションの変更通知を受け取ると、Generator は ListBox.GetContainerForItem() を呼び出す。
	・ListBox.GetContainerForItem() は ListBoxItem を作って返す。
	・itemContainerGenerator.ItemsChanged イベントを発生させる。(普通の CLR イベント)
	・このイベントは Panel.OnItemsChanged でキャッチする。その先で 論理ツリー（Panel の _uiElementCollection）に追加される。

	・・・が、最初のうちからこんな複雑にはしたくない・・・・・。


	ちなみに、TreeView の場合は TreeViewItem が ItemsControl。
	↑の仕組みがノードの数だけネスとされることになる。


	Panel が Owner を持ち、Owner に Item を作ってもらうのはいいと思う。
	問題は Styleを誰が適用するのか。


	■Group

		ListBox
			…
				ItemsPresenter
					StackPanel
						GroupItem
						GroupItem
						GroupItem
							StackPanel
								ContentPresenter (PART_Header)
									TextBlock
								ItemsPresenter (ItemsPresenter)
									VirtualizingStackPanel
										ListBoxItem
										ListBoxItem
										ListBoxItem
									

*/
#include "../../Internal.h"
#include <Lumino/GUI/ControlTemplate.h>
#include <Lumino/GUI/Controls/ListBox.h>
#include <Lumino/GUI/Controls/StackPanel.h>
#include <Lumino/GUI/TextBlock.h>
#include <Lumino/GUI/VisualState.h>
#include "../UIManagerImpl.h"
#include "../GUIHelper.h"

namespace Lumino
{
LN_NAMESPACE_GUI_BEGIN

//=============================================================================
// ListBoxItem
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(ListBoxItem, ItemsControlItem);
LN_UI_ELEMENT_SUBCLASS_IMPL(ListBoxItem);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ListBoxItem* ListBoxItem::Create(GUIManagerImpl* manager)
{
	auto obj = RefPtr<ListBoxItem>::Create(manager);
	obj->InitializeComponent();
	obj.SafeAddRef();

	return obj;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ListBoxItem::ListBoxItem(GUIManagerImpl* manager)
	: ItemsControlItem(manager)
{
	// Register handler
	LN_REGISTER_ROUTED_EVENT_HANDLER(ListBoxItem, MouseEventArgs, UIElement::MouseEnterEvent, Handler_MouseEnter);
	LN_REGISTER_ROUTED_EVENT_HANDLER(ListBoxItem, MouseEventArgs, UIElement::MouseLeaveEvent, Handler_MouseLeave);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ListBoxItem::~ListBoxItem()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ListBoxItem::Handler_MouseEnter(MouseEventArgs* e)
{
	VisualStateManager::GoToState(this, VisualStatus::MouseOver);
	e->Handled = true;


	//auto ff = GetTypeInfo(this)->FindProperty(_T("FontFamily"));
	//printf("%p\n", ff);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ListBoxItem::Handler_MouseLeave(MouseEventArgs* e)
{
	VisualStateManager::GoToState(this, VisualStatus::Normal);
	e->Handled = true;
}

//=============================================================================
// ListBoxItemList
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(ListBoxItemList, GenericCoreList<ListBoxItem*>);

void ListBoxItemList::InsertItem(int index, const value_type& item)
{
	m_owner->OnListBoxItemAdded(item);
	GenericCoreList<ListBoxItem*>::InsertItem(index, item);
}
void ListBoxItemList::ClearItems()
{
	for (ListBoxItem* obj : *this) {
		m_owner->OnListBoxItemRemoved(obj);
	}
	GenericCoreList<ListBoxItem*>::ClearItems();
}
void ListBoxItemList::RemoveItem(int index)
{
	m_owner->OnListBoxItemRemoved(GetAt(index));
	GenericCoreList<ListBoxItem*>::RemoveItem(index);
}
void ListBoxItemList::SetItem(int index, const value_type& item)
{
	m_owner->OnListBoxItemAdded(item);
	GenericCoreList<ListBoxItem*>::SetItem(index, item);
}

////-----------------------------------------------------------------------------
////
////-----------------------------------------------------------------------------
//void ListBoxItemList::OnItemAdded(ListBoxItem* item)
//{
//	m_owner->OnListBoxItemAdded(item);
//}
//
////-----------------------------------------------------------------------------
////
////-----------------------------------------------------------------------------
//void ListBoxItemList::OnItemRemoved(ListBoxItem* item)
//{
//	m_owner->OnListBoxItemRemoved(item);
//}

//=============================================================================
// ListBoxChrome
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(ListBoxChrome, Decorator);
LN_UI_ELEMENT_SUBCLASS_IMPL(ListBoxChrome);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ListBoxChromePtr ListBoxChrome::Create()
{
	return internalCreateInstance(ApplicationContext::GetGUIManager());
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ListBoxChrome::ListBoxChrome(GUIManagerImpl* manager)
	: Decorator(manager)
	, m_frameWidth(8)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ListBoxChrome::~ListBoxChrome()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ListBoxChrome::OnApplyTemplate(CombinedLocalResource* localResource)
{
	m_frameBrush = static_cast<TextureBrush*>(localResource->GetItem(_T("ListBoxNormalFrameBrush")));
	m_bgBrush = static_cast<TextureBrush*>(localResource->GetItem(_T("ListBoxNormalBackgroundBrush")));
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ListBoxChrome::OnRender(Painter* painter)
{
	RectF bgRect = m_finalLocalRect;
	RectF rect = m_finalLocalRect;

	if (!m_isMouseOver)
	{
		bgRect.Inflate(-m_frameWidth, -m_frameWidth);
		painter->SetBrush(m_bgBrush);
		painter->DrawRectangle(bgRect);
	}

	painter->SetBrush(m_frameBrush);
	painter->DrawFrameRectangle(rect, m_frameWidth);
}


//=============================================================================
// ListBox
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(ListBox, ItemsControl);
LN_UI_ELEMENT_SUBCLASS_IMPL(ListBox);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ListBoxPtr ListBox::Create()
{
	return internalCreateInstance(ApplicationContext::GetGUIManager());
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ListBox::ListBox(GUIManagerImpl* manager)
	: ItemsControl(manager)
	//, m_listBoxItems(LN_NEW ListBoxItemList(this))
{
	//m_itemsPanel.Attach(LN_NEW StackPanel(manager));
	//AddVisualChild(m_itemsPanel);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ListBox::~ListBox()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
//void ListBox::InsertTextItem(int index, const String& text)
//{
//}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ListBoxItemPtr ListBox::AddTextItem(const String& text)
{
	auto textBlock = GUIHelper::CreateUIElemenInstance<TextBlock>(m_manager);
	textBlock->SetText(text);
	auto item = GUIHelper::CreateUIElemenInstance<ListBoxItem>(m_manager);
	item->SetContent(textBlock);
	GetItems()->Add(item);
	return item;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
//ListBoxItemPtr ListBox::AddItem(const UIElement& item)
//{
//
//}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ListBox::InsertListBoxItem(int index, UIElement* element)
{
	RefPtr<ListBoxItem> item(ListBoxItem::Create(m_manager));
	item->SetContent(element);
	GetItems()->Add(item);	// TODO: インデックス
	//m_listBoxItems->Insert(index, item);
}

#if 0
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
UIElement* ListBox::CheckMouseHoverElement(const PointF& globalPt)
{
	if (m_itemsPanel != NULL) {	// 子要素を優先
		UIElement* e = m_itemsPanel->CheckMouseHoverElement(globalPt);
		if (e != NULL) { return e; }
	}
	return UIElement::CheckMouseHoverElement(globalPt);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ListBox::ApplyTemplateHierarchy(CombinedLocalResource* parent)
{
	Control::ApplyTemplateHierarchy(parent);
	if (m_itemsPanel != NULL) {
		m_itemsPanel->ApplyTemplateHierarchy(m_combinedLocalResource);	// 再帰的に更新する
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ListBox::MeasureLayout(const SizeF& availableSize)
{
	if (m_itemsPanel != NULL) {
		m_itemsPanel->MeasureLayout(availableSize);	// 特に枠とかないのでそのままのサイズを渡せる
	}
	Control::MeasureLayout(availableSize);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ListBox::ArrangeLayout(const RectF& finalLocalRect)
{
	if (m_itemsPanel != NULL) {
		m_itemsPanel->ArrangeLayout(finalLocalRect);	// 特に枠とかないのでそのままのサイズを渡せる
	}
	Control::ArrangeLayout(finalLocalRect);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool ListBox::OnEvent(EventType type, RoutedEventArgs* args)
{
	if (m_itemsPanel != NULL) {
		if (m_itemsPanel->OnEvent(type, args)) { return true; }
	}
	return Control::OnEvent(type, args);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ListBox::Render()
{
	Control::Render();
	if (m_itemsPanel != NULL) {
		m_itemsPanel->Render();
	}
}
#endif

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ListBox::OnListBoxItemAdded(ListBoxItem* item)
{
	m_itemsPanel->GetChildren()->Add(item);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ListBox::OnListBoxItemRemoved(ListBoxItem* item)
{
	LN_THROW(0, NotImplementedException);
	//m_itemsPanel->GetChildren()->Remove(item);
}

LN_NAMESPACE_GUI_END
} // namespace Lumino

