
#pragma once
#include "UIEvent.h"

LN_NAMESPACE_BEGIN

///**
//	@brief		
//*/
//class UITypeInfo
//	: public tr::TypeInfo
//{
//public:
//	UITypeInfo(
//		const TCHAR* className,
//		TypeInfo* baseClass,
//		HasLocalValueFlagsGetter getter,
//		BindingTypeInfoSetter bindingTypeInfoSetter,
//		BindingTypeInfoGetter bindingTypeInfoGetter);
//	virtual ~UITypeInfo() = default;
//
//	//void RegisterRoutedEvent(UIEventInfo* ev);
//	//bool InvokeRoutedEvent(UIElement* owner, const UIEventInfo* ev, UIEventArgs* e);
//
//private:
//	//Array<UIEventInfo*>		m_routedEventList;
//};
//
//#define LN_UI_TYPEINFO_DECLARE() \
//	LN_TR_REFLECTION_TYPEINFO_DECLARE_COMMON(ln::UITypeInfo)
//
//#define LN_UI_TYPEINFO_IMPLEMENT(classType, baseClassType) \
//	LN_TR_REFLECTION_TYPEINFO_IMPLEMENT_COMMON(ln::UITypeInfo, classType, baseClassType)

LN_NAMESPACE_END
