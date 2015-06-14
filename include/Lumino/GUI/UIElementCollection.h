
#pragma once
#include "../CoreObjectList.h"
#include "UIElement.h"

namespace Lumino
{
namespace GUI
{

/**
	@brief		
*/
class UIElementCollection
	: public CoreObjectList<UIElement>
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
public:
	UIElementCollection() {}
	virtual ~UIElementCollection() {}

	Event01<UIElement*>	ItemAdded;
	Event01<UIElement*>	ItemRemoved;

protected:
	virtual void OnItemAdded(UIElement* item) { ItemAdded.Raise(item); }
	virtual void OnItemRemoved(UIElement* item) { ItemRemoved.Raise(item); }


#if 0
	LN_CORE_OBJECT_TYPE_INFO_DECL();
public:
	UIElementCollection(UIElement* logicalParent)
		: m_logicalParent(logicalParent)
	{}
	virtual ~UIElementCollection(){}

	/// 末尾に要素を追加します。
	void Add(UIElement* item)
	{
		if (item != NULL) {
			item->SetParent(m_logicalParent);
			m_visualChildren.Add(item);

			// 子要素のテーマを直ちに更新
			item->ApplyTemplate();
		}
	}

public:
	//typedef ArrayList<UIElement*>::iterator	iterator;
	//typedef ArrayList<UIElement*>::const_iterator	const_iterator;
	typedef ArrayList< RefPtr<UIElement> >::iterator	iterator;
	typedef ArrayList< RefPtr<UIElement> >::const_iterator	const_iterator;
	iterator		begin()			{ return m_visualChildren.begin(); }
	const_iterator	begin() const	{ return m_visualChildren.begin(); }
	iterator		end()			{ return m_visualChildren.end(); }
	const_iterator	end() const		{ return m_visualChildren.end(); }

public:
	UIElement*						m_logicalParent;
	ArrayList< RefPtr<UIElement> >	m_visualChildren;	// ArrayList<RefPtr<UIElement> >  はやめた。イテレートするときに冗長になる。
#endif
};

//template<class >
//class ObservedUIElementCollection
//{
//
//};

} // namespace GUI
} // namespace Lumino
