
#include "Internal.h"
#include <Lumino/UI/UITypeInfo.h>

LN_NAMESPACE_BEGIN

//==============================================================================
// UITypeInfo
//==============================================================================
//static const intptr_t UITypeInfo_InternalGroup = 1;
//
////------------------------------------------------------------------------------
//UITypeInfo::UITypeInfo(
//	const Char* className,
//	TypeInfo* baseClass,
//	HasLocalValueFlagsGetter getter,
//	BindingTypeInfoSetter bindingTypeInfoSetter,
//	BindingTypeInfoGetter bindingTypeInfoGetter)
//	: tr::TypeInfo(className, baseClass, getter, bindingTypeInfoSetter, bindingTypeInfoGetter)
//{
//	setInternalGroup(UITypeInfo_InternalGroup);
//}
//
////------------------------------------------------------------------------------
////
////------------------------------------------------------------------------------
//void UITypeInfo::RegisterRoutedEvent(UIEventInfo* ev)
//{
//	LN_VERIFY_RETURN(!ev->m_registerd);
//	m_routedEventList.Add(ev);
//	ev->m_registerd = true;
//}
//
////------------------------------------------------------------------------------
////
////------------------------------------------------------------------------------
//bool UITypeInfo::InvokeRoutedEvent(UIElement* owner, const UIEventInfo* ev, UIEventArgs* e)
//{
//	for (UIEventInfo* dynamicEvent : m_routedEventList)
//	{
//		if (dynamicEvent == ev)
//		{
//			// owner に addHandler されているイベントハンドラを呼び出す。
//			dynamicEvent->callEvent(owner, e);
//			return e->handled;	// ev と同じイベントは1つしかリスト内に無いはずなのですぐ return
//		}
//	}
//
//	// ベースクラスが UITypeInfo なら、さらにベースクラスを見に行く
//	if (getBaseClass()->getInternalGroup() == 1)
//	{
//		return static_cast<UITypeInfo*>(getBaseClass())->InvokeRoutedEvent(owner, ev, e);
//	}
//	return false;
//}

LN_NAMESPACE_END
