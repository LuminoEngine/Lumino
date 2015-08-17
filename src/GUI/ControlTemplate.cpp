
#pragma once
#include "../Internal.h"
#include <Lumino/GUI/GUIManager.h>
#include <Lumino/GUI/Control.h>
#include <Lumino/GUI/ControlTemplate.h>
#include "GUIHelper.h"

namespace Lumino
{
namespace GUI
{

//=============================================================================
// UIElementFactory
//=============================================================================
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
UIElementFactory::UIElementFactory(GUIManager* manager)
	: m_manager(manager)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
UIElementFactory::~UIElementFactory()
{
	LN_FOREACH(UIElementFactory* f, m_children) {
		f->Release();
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIElementFactory::AddChild(UIElementFactory* child)
{
	if (child != NULL)
	{
		m_children.Add(child);
		LN_SAFE_ADDREF(child);
	}
}

#if 0
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
CoreObject* UIElementFactory::CreateInstance()
{
	CoreObject* obj = m_manager->CreateObject(m_targetTypeFullName);
	if (obj == NULL) {
		// TODO: 
		LN_THROW(0, InvalidOperationException);
	}
	return obj;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIElementFactory::BuildInstance(CoreObject* obj, UIElement* rootLogicalParent)
{
	// プロパティを設定する
	for (PropertyValueList::Pair& pair : m_propertyValueList)
	{
		if (pair.first->IsList())/*.second.GetType() == VariantType_List)*/
		{
			// リストの場合は少し特殊。オブジェクトのメンバのリストは既につくられている前提で、
			// それに対して要素を1つずつ Add していく。
#if 1
			VariantList* list = pair.second.GetList();
			for (Variant& item : *list)
			{
				UIElementFactory* factory = NULL;
				// 設定したい値がもし Factory だった場合はインスタンスを作って設定する
				if (item.GetType() == VariantType_Object &&
					(factory = dynamic_cast<UIElementFactory*>(item.GetObject())) != NULL)	// TODO: dynamic_cast じゃなくて TypeInfo 使えば少し速くなるかも？
				{
					RefPtr<CoreObject> v(factory->CreateInstance());	// 作って
					pair.first->AddItem(obj, v);						// 追加して (ローカルリソース更新)
					factory->BuildInstance(v, rootLogicalParent);		// プロパティ設定
				}
				else {
					pair.first->AddItem(obj, item);
				}
			}
#else
			Variant v = obj->GetPropertyValue(pair.first);
			LN_THROW(v.GetType() == VariantType_List, InvalidOperationException);	// ターゲットの型は必ず List でなければならない
			VariantList* targetList = v.GetList();
			VariantList* list = pair.second.GetList();
			for (Variant& item : *list) {
				targetList->AddVariant(item);
			}
#endif
		}
		else
		{
			Variant& item = pair.second;
			UIElementFactory* factory = NULL;
				
			// 設定したい値がもし Factory だった場合はインスタンスを作って設定する
			if (item.GetType() == VariantType_Object &&
				(factory = dynamic_cast<UIElementFactory*>(item.GetObject())) != NULL)	// TODO: dynamic_cast じゃなくて TypeInfo 使えば少し速くなるかも？
			{
				RefPtr<CoreObject> v(factory->CreateInstance());	// 作って
				pair.first->AddItem(obj, v);						// 追加して (ローカルリソース更新)
				factory->BuildInstance(v, rootLogicalParent);		// プロパティ設定
			}
			else {
				obj->SetPropertyValue(pair.first, item);
			}
		}
	}

	UIElement* element = dynamic_cast<UIElement*>(obj);
	if (element != NULL)
	{
		LN_FOREACH(PropertyInfoList::Pair& pair, m_propertyInfoList)
		{
			if (pair.second.Kind == PropertyKind_TemplateBinding)
			{
				const Property* prop = pair.first;//GetTypeInfo(element)->FindProperty(pair.first);
				//if (prop == NULL) {
				//	LN_THROW(0, InvalidOperationException);	// TODO: XML エラーとかいろいろ考える必要がある
				//}
				element->SetTemplateBinding(prop, pair.second.SourcePropPath, rootLogicalParent);
			}
			else {
				LN_THROW(0, NotImplementedException);
			}
		}

		//for (int i = 0; i < m_propertyInfoList.GetCount(); ++i)
		//{

		//	
		//}
		//
		// 子の処理
		LN_FOREACH(UIElementFactory* factory, m_children) {
			RefPtr<CoreObject> e(factory->CreateInstance());	// 作って
			element->AddChild(e);								// 追加して (ローカルリソース更新)
			factory->BuildInstance(e, rootLogicalParent);		// プロパティ設定
		}

		rootLogicalParent->PollingTemplateChildCreated(element);
	}
	else
	{
		// ContentElement はここに来る
	}
}
#endif
#if 1
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
CoreObject* UIElementFactory::CreateInstance(UIElement* rootLogicalParent)
{
	CoreObject* obj = m_manager->CreateObject(m_targetTypeFullName);
	if (obj == NULL) {
		// TODO: 
		LN_THROW(0, InvalidOperationException);
	}

	// プロパティを設定する
	for (const PropertyValueList::Pair& pair : m_propertyValueList)
	{
		if (pair.first->IsList())/*.second.GetType() == VariantType_List)*/
		{
			// リストの場合は少し特殊。オブジェクトのメンバのリストは既につくられている前提で、
			// それに対して要素を1つずつ Add していく。
#if 1
			VariantList* list = pair.second.GetList();
			for (Variant& item : *list) {
				if (item.GetType() == VariantType_Object &&
					dynamic_cast<UIElementFactory*>(item.GetObject()) != NULL)	// TODO: dynamic_cast じゃなくて TypeInfo 使えば少し速くなるかも？
				{
					RefPtr<CoreObject> v(static_cast<UIElementFactory*>(item.GetObject())->CreateInstance(rootLogicalParent));
					pair.first->AddItem(obj, v);
				}
				else {
					pair.first->AddItem(obj, item);
				}
			}
#else
			Variant v = obj->GetPropertyValue(pair.first);
			LN_THROW(v.GetType() == VariantType_List, InvalidOperationException);	// ターゲットの型は必ず List でなければならない
			VariantList* targetList = v.GetList();
			VariantList* list = pair.second.GetList();
			for (Variant& item : *list) {
				targetList->AddVariant(item);
			}
#endif
		}
		else
		{
			const Variant& item = pair.second;
			if (item.GetType() == VariantType_Object &&
				dynamic_cast<UIElementFactory*>(item.GetObject()) != NULL)	// TODO: dynamic_cast じゃなくて TypeInfo 使えば少し速くなるかも？
			{
				RefPtr<CoreObject> v(static_cast<UIElementFactory*>(item.GetObject())->CreateInstance(rootLogicalParent));
				pair.first->AddItem(obj, v);
			}
			else {
				obj->SetPropertyValue(pair.first, item);
			}
		}
	}

	// 生成したオブジェクトが UIElement であればバインディングなどの特殊な処理を行う
	UIElement* element = dynamic_cast<UIElement*>(obj);
	if (element != NULL)
	{
		GUIHelper::UIElement_SetKeyName(element, m_keyName);

		// TODO: これいらなかも？
		GUIHelper::UIElement_SetTemplateParent(element, rootLogicalParent);

		for(const PropertyInfoList::Pair& pair : m_propertyInfoList)
		{
			if (pair.second.Kind == PropertyKind_TemplateBinding)
			{
				const Property* prop = pair.first;//GetTypeInfo(element)->FindProperty(pair.first);
				//if (prop == NULL) {
				//	LN_THROW(0, InvalidOperationException);	// TODO: XML エラーとかいろいろ考える必要がある
				//}
				element->SetTemplateBinding(prop, pair.second.SourcePropPath);
			}
			else {
				LN_THROW(0, NotImplementedException);
			}
		}

		//for (int i = 0; i < m_propertyInfoList.GetCount(); ++i)
		//{

		//	
		//}
		//
		// 子の処理
		LN_FOREACH(UIElementFactory* c, m_children) {
			RefPtr<CoreObject> e(c->CreateInstance(rootLogicalParent));
			element->AddChild(e);
		}
		
		if (rootLogicalParent != NULL) {
			rootLogicalParent->PollingTemplateChildCreated(element);
		}
	}
	else
	{
		// ContentElement はここに来る
	}

	return obj;
}
#endif

//=============================================================================
// ControlTemplate
//=============================================================================
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ControlTemplate::ControlTemplate()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ControlTemplate::~ControlTemplate()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ControlTemplate::Apply(Control* control)
{
	if (LN_VERIFY_ASSERT(control != NULL)) { return; }

	// いろいろリセット。TemplateBinding も解除。
	control->SetTemplateChild(NULL);

	if (m_visualTreeRoot != NULL) {
#if 0
		// まずインスタンスを作成する
		RefPtr<CoreObject> obj(m_visualTreeRoot->CreateInstance());

		// 次に親要素に追加する (ローカルリソースを更新する)
		UIElement* element = dynamic_cast<UIElement*>(obj.GetObjectPtr());
		if (element != NULL) { control->SetTemplateChild(element); }

		// 最後にプロパティの設定や孫要素の作成を行う
		m_visualTreeRoot->BuildInstance(m_visualTreeRoot, control);
#else
		RefPtr<CoreObject> obj(m_visualTreeRoot->CreateInstance(control));
		UIElement* element = dynamic_cast<UIElement*>(obj.GetObjectPtr());
		if (element != NULL)
		{
			control->SetTemplateChild(element);
		}
#endif
	}

	// プロパティ適用
#if 0
	for (auto prop : m_propertyValueList)
	{
		bool isElement = false;
		if (prop.second.GetType() == VariantType_Object)
		{
			UIElementFactory* factory = dynamic_cast<UIElementFactory*>(prop.second.GetObject());
			if (factory != NULL)
			{
#if 0
				// まずインスタンスを作成する
				RefPtr<CoreObject> element(factory->CreateInstance());

				// 次に親要素に追加する (ローカルリソースを更新する)
				control->SetPropertyValue(prop.first, element);

				// 最後にプロパティの設定や孫要素の作成を行う
				factory->BuildInstance(element, control);
#else
				RefPtr<CoreObject> element(factory->CreateInstance(control));
				control->SetPropertyValue(prop.first, element);
#endif

				isElement = true;
			}
		}

		if (!isElement) {
			control->SetPropertyValue(prop.first, prop.second);
		}
	}
#endif
}


#if 0
//=============================================================================
// DataTemplate
//=============================================================================
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
DataTemplate::DataTemplate()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
DataTemplate::~DataTemplate()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DataTemplate::Apply(Control* control)
{
	if (LN_VERIFY_ASSERT(control != NULL)) { return; }

	if (m_visualTreeRoot != NULL)
	{
#if 0
		// まずインスタンスを作成する
		CoreObject* obj = m_visualTreeRoot->CreateInstance();

		// 次に親要素に追加する (ローカルリソースを更新する)
		UIElement* element = dynamic_cast<UIElement*>(obj);
		if (element != NULL)
		{
			control->SetTemplateChild(element);
		}
		else {
			// TODO: ここに来たときにメモリリークする
		}

		// 最後にプロパティの設定や孫要素の作成を行う
		m_visualTreeRoot->BuildInstance(element, control);
#else
		CoreObject* obj = m_visualTreeRoot->CreateInstance(control);
		UIElement* element = dynamic_cast<UIElement*>(obj);
		if (element != NULL)
		{
			control->SetTemplateChild(element);
		}
		else {
			// TODO: ここに来たときにメモリリークする
		}
#endif
	}

	// TODO: プロパティ適用等も。
}
#endif


//=============================================================================
// Setter
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(Setter, CoreObject);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Setter::Setter()
	: m_property(NULL)
	, m_value()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Setter::Setter(const Property* prop, const Variant& value)
	: m_property(prop)
	, m_value(value)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Setter::~Setter()
{
}

//=============================================================================
// TriggerBase
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(TriggerBase, CoreObject);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
TriggerBase::TriggerBase()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
TriggerBase::~TriggerBase()
{
}

//=============================================================================
// Trigger
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(Trigger, TriggerBase);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Trigger::Trigger()
	: m_property(NULL)
	, m_value()
	, m_setterList(RefPtr<SetterList>::Create())
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Trigger::Trigger(const Property* prop, const Variant& value)
	: m_property(prop)
	, m_value(value)
	, m_setterList(RefPtr<SetterList>::Create())
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Trigger::~Trigger()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
//void Trigger::Invoke(const RoutedEvent* routedEvent, const RoutedEventArgs* e, CoreObject* target)
//{
//	//LN_THROW(0, NotImplementedException);
//	if (routedEvent == CoreObject::PropertyChangedEvent)
//	{
//		auto e2 = static_cast<const PropertyChangedEventArgs*>(e);
//		if (m_property->GetName() == e2->PropertyName &&	// できれば文字列ではなくポインタ比較したい…
//			m_value == e2->NewValue)
//		{
//			for (Setter* setter : *m_setterList) {
//				target->SetPropertyValue(setter->GetProperty(), setter->GetValue());
//			}
//		}
//	}
//}
void Trigger::TryInvoke(CoreObject* target, PropertyChangedEventArgs* e)
{
	if (m_property == e->ChangedProperty &&
		m_value == e->NewValue)
	{
		for (Setter* setter : *m_setterList) {
			target->SetPropertyValue(setter->GetProperty(), setter->GetValue());
		}
	}
}

//=============================================================================
// Style
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(Style, CoreObject);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Style::Style()
	: m_targetType(NULL)
	, m_baseStyle()
	, m_setterList()
	, m_propertyTriggerList()
{
	m_setterList.Attach(LN_NEW SetterList());
	m_propertyTriggerList.Attach(LN_NEW PropertyTriggerList());
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Style::~Style()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Style::Apply(UIElement* element)
{
	for (Setter* setter : *m_setterList)
	{
		const Property* prop = setter->GetProperty();
		const Variant& value = setter->GetValue();

		if (value.GetType() == VariantType_Object &&
			dynamic_cast<UIElementFactory*>(value.GetObject()) != NULL)	// TODO: dynamic_cast じゃなくて TypeInfo 使えば少し速くなるかも？
		{
			// InitializeComponent() も呼ばれる
			RefPtr<CoreObject> v(static_cast<UIElementFactory*>(value.GetObject())->CreateInstance(NULL));
			element->SetPropertyValue(prop, v);
		}
		else {
			element->SetPropertyValue(prop, value);
		}
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Style::NortifyTargetObjectPropertyChanged(CoreObject* target, PropertyChangedEventArgs* e)
{
	for (Trigger* trigger : *m_propertyTriggerList)
	{
		trigger->TryInvoke(target, e);
	}
}
//void Style::InvoleTriggers(const RoutedEvent* routedEvent, const RoutedEventArgs* e, CoreObject* target)
//{
//	LN_VERIFY_RETURN(routedEvent != NULL);
//	for (TriggerBase* trigger : *m_triggerList)
//	{
//		trigger->Invoke(routedEvent, e, target);
//	}
//}

//=============================================================================
// ResourceDictionary
//=============================================================================
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ResourceDictionary::ResourceDictionary()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ResourceDictionary::~ResourceDictionary()
{
	LN_FOREACH(ItemPair p, m_items) {
		p.second->Release();
	}
	//LN_FOREACH(ControlTemplatePair p, m_controlTemplateMap) {
	//	p.second->Release();
	//}
	LN_FOREACH(Style* p, m_styleList) {
		p->Release();
	}
	//printf("");
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool ResourceDictionary::TryGetItem(const String& key, CoreObject** outObject)
{
	if (LN_VERIFY_ASSERT(outObject != NULL)) { return false; }
	ItemMap::iterator itr = m_items.find(key);
	if (itr != m_items.end()) {
		*outObject = itr->second;
		return true;
	}
	return false;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ResourceDictionary::AddItem(const String& key, CoreObject* obj)
{
	if (LN_VERIFY_ASSERT(obj != NULL)) { return; }
	m_items[key] = obj;
	obj->AddRef();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Style* ResourceDictionary::FindStyle(TypeInfo* type)
{
	Array<Style*>::iterator itr =
		std::find_if(m_styleList.begin(), m_styleList.end(), [type](Style* style){ return style->GetTargetType() == type; });
	if (itr != m_styleList.end()) {
		return *itr;
	}
	return NULL;
}
//bool ResourceDictionary::TryGetControlTemplate(const String& fullTypeName, ControlTemplate** outTemplate)
//{
//	if (LN_VERIFY_ASSERT(outTemplate != NULL)) { return false; }
//	ControlTemplateMap::iterator itr = m_controlTemplateMap.find(fullTypeName);
//	if (itr != m_controlTemplateMap.end()) {
//		*outTemplate = itr->second;
//		return true;
//	}
//	return false;
//}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
//void ResourceDictionary::AddControlTemplate(ControlTemplate* outTemplate)
//{
//	if (LN_VERIFY_ASSERT(outTemplate != NULL)) { return; }
//	m_controlTemplateMap[outTemplate->GetTargetType()] = outTemplate;
//	outTemplate->AddRef();
//}
void ResourceDictionary::AddStyle(Style* style)
{
	LN_VERIFY_RETURN(style != NULL);
	m_styleList.Add(style);
	style->AddRef();
}

//=============================================================================
// CombinedLocalResource
//=============================================================================
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
CombinedLocalResource::CombinedLocalResource()
	: m_parent(NULL)
	, m_local(NULL)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
CombinedLocalResource::~CombinedLocalResource()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CombinedLocalResource::Combine(CombinedLocalResource* parent, ResourceDictionary* local)
{
	m_parent = parent;
	m_local = local;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
CoreObject* CombinedLocalResource::GetItem(const String& key)
{
	CoreObject* obj;
	if (m_local != NULL && m_local->TryGetItem(key, &obj)) {
		return obj;
	}
	return m_parent->GetItem(key);

	// TOOD: エラーこれでいい？
	//LN_THROW(0, ArgumentException);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Style* CombinedLocalResource::FindStyle(TypeInfo* type)
{
	// local 優先
	if (m_local != NULL)
	{
		Style* style = m_local->FindStyle(type);
		if (style != NULL) { return style; }
	}
	// parent
	if (m_parent != NULL)
	{
		Style* style = m_parent->FindStyle(type);
		if (style != NULL) { return style; }
	}
	return NULL;
}

//bool CombinedLocalResource::TryGetControlTemplate(const String& fullTypeName, ControlTemplate** outTemplate)
//{
//	// local 優先
//	if (m_local != NULL && m_local->TryGetControlTemplate(fullTypeName, outTemplate)) {
//		return true;
//	}
//	if (m_parent != NULL && m_parent->TryGetControlTemplate(fullTypeName, outTemplate)) {
//		return true;
//	}
//	return false;
//}

} // namespace GUI
} // namespace Lumino

