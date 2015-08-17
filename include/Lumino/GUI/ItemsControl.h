
#pragma once
#include "Control.h"
#include "ContentControl.h"
#include "ItemsPresenter.h"
#include "Controls/Panel.h"

namespace Lumino
{
namespace GUI
{

LN_ENUM(ListChangedAction)
{
	Add	= 0,	///< 1つ以上の要素がリストに追加された
	Remove,		///< 1つ以上の要素がリストから取り除かれた
	Replace,	///< 1つ以上の要素が変更された
	Move,		///< 1つ以上の要素がリスト内を移動した
};
LN_ENUM_DECLARE(ListChangedAction);

class EventArgs
	: public CoreObject
{

};

class ListChangedEventArgs
	: public EventArgs
{
public:
	ListChangedEventArgs(ListChangedAction action)
	{
		Action = action;
	}

public:
	ListChangedAction	Action;
	VariantList			NewItems;
};
	





///**
//	@brief
//*/
//class ListModel
//	: public CoreObject
//{
//public:
//	/// 処理の基になるリストを設定します。
//	void SetSourceList(VariantList* list) { m_list = list; }
//
//	bool CanFilter() const { return false; }	// TODO
//
//	bool CanGroup() const;
//
//	bool CanSort() const { return false; }		// TODO
//
//
//	// ユーティリティ
//	void AddGroup(const String& name);
//
//private:
//	RefPtr<VariantList>	m_list;
//};
//
//// ItemSource が NULL の時、内部的に使われる。
//class InternalItemViewModel
//{
//
//};


/**
	@brief
*/
class ItemList
	: public GenericVariantList<UIElement*>
{
public:
	Event01<ListChangedEventArgs*>	ItemsChanged;

protected:
	virtual void OnItemAdded(const Variant& item)	// TODO インデックスも取りたいかも
	{
		auto args = RefPtr<ListChangedEventArgs>::Create(ListChangedAction::Add);	// TODO キャッシュしたい
		args->NewItems.AddVariant(item);
		ItemsChanged(args);
	}
	virtual void OnItemRemoved(const Variant& item) 
	{
		//auto args = RefPtr<ListChangedEventArgs>::Create(ListChangedAction::Remove);
		//ItemsChanged(args);
	}

private:
};


/**
	@brief
*/
class ItemsControlItem
	: public ContentControl
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
	LN_UI_ELEMENT_SUBCLASS_DECL(ItemsControlItem);
public:
	//void SetGroup(const Variant& group);
	//const Variant& GetGroup() const;

protected:
	ItemsControlItem(GUIManager* manager);
	virtual ~ItemsControlItem();

private:
	
};


/**
	@brief
*/
class GroupItemList
	: public GenericVariantList<GroupItem*>
{
protected:
	GroupItemList(ItemsControl* owner);

	virtual void OnItemAdded(const Variant& item);
	virtual void OnItemRemoved(const Variant& item);

private:
	ItemsControl* m_owner;

	friend class ItemsControl;
};


/**
	@brief
*/
class ItemsControl
	: public Control
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
	LN_UI_ELEMENT_SUBCLASS_DECL(ItemsControl);
public:
	LN_PROPERTY(ControlTemplate*,	ItemsPanelTemplateProperty);
	LN_PROPERTY(Style*,				GroupStyleProperty);			// TODO: StylePtr にできない？

public:
	static ItemsControl* Create(GUIManager* manager);

	/**
		@brief
	*/
	GroupItemPtr AddGroup(const String& caption);
	//void InsertGroup(int index, const GroupItem& group);





	ItemsControl(GUIManager* manager);
	virtual ~ItemsControl();

	void SetItemsPanelTemplate(ControlTemplate* controlTemplate) { m_itemsPanelTemplate = controlTemplate; }
	ControlTemplate* GetItemsPanelTemplate() const { return m_itemsPanelTemplate; }

	//void SetGroupStyle(Style* style) { SetTypedPropertyValue(GroupStyleProperty, style); }

	ItemListPtr GetItems() const { return m_itemList; }

private:

	virtual void OnApplyTemplate(CombinedLocalResource* localResource);
	virtual void PollingTemplateChildCreated(UIElement* newElement);
	//virtual SizeF MeasureOverride(const SizeF& constraint);
	//virtual SizeF ArrangeOverride(const SizeF& finalSize);

	void Items_ListChanged(ListChangedEventArgs* e);

	void OnItemGroupKeyChanged(ItemsControlItem* item);


	void OnGroupItemAdded(GroupItem* groupItem);
	void OnGroupItemRemoved(GroupItem* groupItem);
	void RefreshHostPanel();
	void RefreshHostPanelItems();

	bool IsGrouping() const { return !m_groupItemList->IsEmpty(); }

private:
	friend class GroupItemList;
	RefPtr<ControlTemplate>	m_itemsPanelTemplate;
	ItemsPresenter*			m_visualItemsPresenter;	///< VisualTree 内の ItemsPresenter
	RefPtr<Panel>			m_hostPanel;			///< アイテムコントロールの追加先
	ItemListPtr				m_itemList;
	RefPtr<GroupItemList>	m_groupItemList;

	StylePtr				m_groupStyle;

	class ItemInfo
	{

	};
};

/**
	@brief
*/
class GroupItem
	: public ItemsControl	// TODO: HeaderedItemsControl へ
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
	LN_UI_ELEMENT_SUBCLASS_DECL(GroupItem);
public:
	LN_PROPERTY(Variant,	HeaderProperty);	// TODO: HeaderedItemsControl へ

public:
	static GroupItemPtr Create();

public:
	void SetHeader(const Variant& text) { SetTypedPropertyValue<Variant>(HeaderProperty, text); }
	Variant GetHeader() const { return GetTypedPropertyValue<Variant>(HeaderProperty); }


	void AddItem(ItemsControlItem* item) { GetItems()->AddVariant(item); }
	

protected:
	GroupItem(GUIManager* manager);
	virtual ~GroupItem();

private:
	Variant		m_header;
};

} // namespace GUI
} // namespace Lumino
