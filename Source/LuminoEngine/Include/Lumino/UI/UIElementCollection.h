
#pragma once
#include <Lumino/Reflection/ReflectionObject.h>
#include "UIElement.h"

LN_NAMESPACE_BEGIN
namespace tr
{

enum class NotifyCollectionChangedAction
{
	Add,		/**< 1 つ以上の項目がコレクションに追加されました。*/
	Move,		/**< コレクション内で 1 つ以上の項目が移動されました。*/
	Remove,		/**< 1 つ以上の項目がコレクションから削除されました。*/
	replace,	/**< コレクション内で 1 つ以上の項目が置き換えられました。*/
	reset,		/**< コレクションの内容が大幅に変更されました。*/
};

struct ChildCollectionChangedArgs
{
	NotifyCollectionChangedAction	action;
	const List<RefPtr<UIElement>>&	newItems;
	int								newStartingIndex;
	const List<RefPtr<UIElement>>&	oldItems;			/**< Move、Remove、または Replace アクションの影響を受ける項目の一覧を取得します。*/
	int								oldStartingIndex;	/**< Move、Remove、または Replace アクションが発生した位置のインデックスを取得します。*/
};

class IUIElementCollectionOwner
{
public:
	virtual void onChildCollectionChanged(const ChildCollectionChangedArgs& e) = 0;
};

} // namespace tr

class UIElementCollection;
using UIElementCollectionPtr = RefPtr<UIElementCollection>;

/**
	@brief		
*/
class UIElementCollection
	: public tr::ReflectionObjectList<UIElement*>
{
	//LN_OBJECT();	// TODO
public:
	typedef tr::ReflectionObjectList<UIElement*>::value_type value_type;

protected:
	virtual void insertItem(int index, const value_type& item) override;
	virtual void clearItems() override;
	virtual void removeItem(int index) override;
	virtual void setItem(int index, const value_type& item) override;

LN_INTERNAL_ACCESS:
	UIElementCollection(tr::IUIElementCollectionOwner* owner);
	virtual ~UIElementCollection();

private:
	tr::IUIElementCollectionOwner*	m_owner;
	List<RefPtr<UIElement>>		m_newItemsCache;
	List<RefPtr<UIElement>>		m_oldItemsCache;
};

LN_NAMESPACE_END
