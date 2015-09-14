
#include "../Internal.h"
#include <Lumino/GUI/ItemsControl.h>
#include "GUIManagerImpl.h"
#include "GUIHelper.h"

namespace Lumino
{
LN_NAMESPACE_GUI_BEGIN
	

//=============================================================================
// ItemList
//=============================================================================
void ItemList::InsertItem(int index, const value_type& item)
{
	auto args = RefPtr<ListChangedEventArgs>::Create(ListChangedAction::Add);	// TODO キャッシュしたい
	args->NewItems.Add(Variant(item));
	ItemsChanged(args);
	GenericCoreList<UIElement*>::InsertItem(index, item);
}
void ItemList::ClearItems()
{
	LN_THROW(0, NotImplementedException);
	//for (UIElement* obj : *this) {
	//	m_owner->OnInlineRemoved(obj);
	//}
	GenericCoreList<UIElement*>::ClearItems();
}
void ItemList::RemoveItem(int index)
{
	LN_THROW(0, NotImplementedException);
	//const Variant& v = VariantList::GetAt(index);
	//m_owner->OnInlineRemoved(static_cast<UIElement*>(v.GetObject()));
	GenericCoreList<UIElement*>::RemoveItem(index);
}
void ItemList::SetItem(int index, const value_type& item)
{
	LN_THROW(0, NotImplementedException);
	//m_owner->OnInlineAdded(static_cast<UIElement*>(item.GetObject()));
	GenericCoreList<UIElement*>::SetItem(index, item);
}

//=============================================================================
// ItemsControlItem
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(ItemsControlItem, ContentControl);
LN_UI_ELEMENT_SUBCLASS_IMPL(ItemsControlItem);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ItemsControlItem::ItemsControlItem(GUIManagerImpl* manager)
	: ContentControl(manager)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ItemsControlItem::~ItemsControlItem()
{

}

//=============================================================================
// GroupItemList
//=============================================================================
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GroupItemList::GroupItemList(ItemsControl* owner)
	: m_owner(owner)
{
}

void GroupItemList::InsertItem(int index, const value_type& item)
{
	m_owner->OnGroupItemAdded(item);
	GenericCoreList<GroupItem*>::InsertItem(index, item);
}
void GroupItemList::ClearItems()
{
	for (GroupItem* obj : *this) {
		m_owner->OnGroupItemRemoved(obj);
	}
	GenericCoreList<GroupItem*>::ClearItems();
}
void GroupItemList::RemoveItem(int index)
{
	m_owner->OnGroupItemRemoved(GetAt(index));
	GenericCoreList<GroupItem*>::RemoveItem(index);
}
void GroupItemList::SetItem(int index, const value_type& item)
{
	m_owner->OnGroupItemAdded(item);
	GenericCoreList<GroupItem*>::SetItem(index, item);
}

////-----------------------------------------------------------------------------
////
////-----------------------------------------------------------------------------
//void GroupItemList::OnItemAdded(const Variant& item)
//{
//	m_owner->OnGroupItemAdded(Variant::Cast<GroupItem*>(item));
//}
//
////-----------------------------------------------------------------------------
////
////-----------------------------------------------------------------------------
//void GroupItemList::OnItemRemoved(const Variant& item)
//{
//	m_owner->OnGroupItemRemoved(Variant::Cast<GroupItem*>(item));
//}
//
//=============================================================================
// ItemsControl
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(ItemsControl, Control);
LN_UI_ELEMENT_SUBCLASS_IMPL(ItemsControl); 

// Register property
LN_PROPERTY_IMPLEMENT(ItemsControl, ControlTemplate*, ItemsPanelTemplateProperty, "ItemsPanelTemplate", m_itemsPanelTemplate, PropertyMetadata(NULL));
LN_PROPERTY_IMPLEMENT(ItemsControl, Style*, GroupStyleProperty, "GroupStyle", m_groupStyle, PropertyMetadata(NULL));

/*	Measure/Arrange はベースの Control クラスに任せるので必要ない
*/

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ItemsControl* ItemsControl::Create(GUIManagerImpl* manager)
{
	auto obj = RefPtr<ItemsControl>::Create(manager);
	obj->InitializeComponent();
	obj.SafeAddRef();
	return obj;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ItemsControl::ItemsControl(GUIManagerImpl* manager)
	: Control(manager)
	, m_visualItemsPresenter(NULL)
	, m_itemList()
{
	m_itemList.Attach(LN_NEW ItemList());
	m_itemList->ItemsChanged += LN_CreateDelegate(this, &ItemsControl::Items_ListChanged);

	m_groupItemList.Attach(LN_NEW GroupItemList(this));
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ItemsControl::~ItemsControl()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GroupItemPtr ItemsControl::AddGroup(const String& caption)
{
	auto group = GUIHelper::CreateUIElemenInstance<GroupItem>(m_manager);
	group->SetHeader(caption);
	m_groupItemList->Add(group);
	return group;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ItemsControl::OnApplyTemplate(CombinedLocalResource* localResource)
{
	Control::OnApplyTemplate(localResource);

	// m_itemsPanelTemplate と m_visualItemsPresenter は必ず何かなければならない
	LN_THROW(m_itemsPanelTemplate != NULL, InvalidOperationException);
	LN_THROW(m_visualItemsPresenter != NULL, InvalidOperationException);



	RefreshHostPanel();
	RefreshHostPanelItems();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ItemsControl::PollingTemplateChildCreated(UIElement* newElement)
{
	auto ip = dynamic_cast<ItemsPresenter*>(newElement);
	if (ip != NULL) {
		m_visualItemsPresenter = ip;
	}
	Control::PollingTemplateChildCreated(newElement);
}

////-----------------------------------------------------------------------------
////
////-----------------------------------------------------------------------------
//SizeF ItemsControl::MeasureOverride(const SizeF& constraint)
//{
//	// TODO ちょっと適当
//	if (!m_visualChildren.IsEmpty()) {
//		m_visualChildren[0]->MeasureLayout(constraint);
//	}
//	return Control::MeasureOverride(constraint);
//}
//
////-----------------------------------------------------------------------------
////
////-----------------------------------------------------------------------------
//SizeF ItemsControl::ArrangeOverride(const SizeF& finalSize)
//{
//	// TODO ちょっと適当
//	if (!m_visualChildren.IsEmpty()) {
//		m_visualChildren[0]->ArrangeOverride(finalSize);
//	}
//	return Control::ArrangeOverride(finalSize);
//}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ItemsControl::Items_ListChanged(ListChangedEventArgs* e)
{
	if (e->Action == ListChangedAction::Add)
	{
		RefreshHostPanelItems();	// TODO: 遅延評価にしたい
		//m_hostPanel->AddChild(e->NewItems.GetAtVariant(0));
	}
	else
	{
		LN_THROW(0, NotImplementedException);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ItemsControl::OnItemGroupKeyChanged(ItemsControlItem* item)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ItemsControl::OnGroupItemAdded(GroupItem* groupItem)
{
	RefreshHostPanelItems();	// TODO: 遅延評価にしたい
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ItemsControl::OnGroupItemRemoved(GroupItem* groupItem)
{
	RefreshHostPanelItems();	// TODO: 遅延評価にしたい
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ItemsControl::RefreshHostPanel()
{
	// 古いのを取り除く
	if (m_hostPanel != NULL) {
		m_visualItemsPresenter->AttachHostPanel(NULL);
	}

	// アイテムを保持する Panel を作る
	// TODO: Template に関数ポインタ持つようにしておけば少し早くできる
	RefPtr<CoreObject> obj(m_manager->CreateObject(m_itemsPanelTemplate->GetTargetType()));
	auto panel = dynamic_cast<Panel*>(obj.GetObjectPtr());
	LN_THROW(panel != NULL, InvalidOperationException);		// Panel のサブクラスでなければならない
	m_hostPanel = panel;

	if (m_hostPanel != NULL) {
		m_visualItemsPresenter->AttachHostPanel(m_hostPanel);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ItemsControl::RefreshHostPanelItems()
{
	if (m_hostPanel != NULL)
	{
		auto childlen = m_hostPanel->GetChildren();
		childlen->Clear();

		if (IsGrouping())
		{
			for (GroupItem* group : *m_groupItemList)
			{
				childlen->Add(group);
			}
		}
		else
		{
			for (UIElement* element : *m_itemList)
			{
				childlen->Add(element);
			}
		}
	}
}

//=============================================================================
// GroupItem
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(GroupItem, ItemsControl);
LN_UI_ELEMENT_SUBCLASS_IMPL(GroupItem);

// Register property
LN_PROPERTY_IMPLEMENT(GroupItem, Variant, HeaderProperty, "Header", m_header, PropertyMetadata(NULL));

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GroupItem::GroupItem(GUIManagerImpl* manager)
	: ItemsControl(manager)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GroupItem::~GroupItem()
{
}


LN_NAMESPACE_GUI_END
} // namespace Lumino

