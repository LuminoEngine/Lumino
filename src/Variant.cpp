/*
	■ Cast について
		LN_ENUM で定義した拡張列挙型は、Enum クラスの派生。
		これらは全て m_enum を派生にキャストして返したいのだが、
		普通の特殊化では派生クラスの面倒を見てくれない。

		例えば、以下の定義があるとする。

			template<typename T>
			T Cast() const { return static_cast<T>(GetObject()); }
			template<> Enum Cast() const { return m_enum; }

		このとき、Enum の派生である Orientation にキャストしようと思って

			auto v = Variant::Cast<Orientation>(e);

		とか書いても、正しくコンパイルできない。GetObject() が呼ばれてしまう。
		派生クラスでも m_enum が返されるようにするには、部分特殊化を駆使する必要がある。

		
*/
#include "Internal.h"
#include <Lumino/Variant.h>
#include <Lumino/CoreObject.h>
#include <Lumino/Property.h>
#include <Lumino/RoutedEvent.h>

#include <Lumino/Graphics/Color.h>	// TODO

namespace Lumino
{

//=============================================================================
// TypeInfo
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
TypeInfo::TypeInfo()
	: m_fullName()
	, m_baseClass(NULL)
	, m_propertyList()
	, m_routedEventList()
	, m_routedEventHandlerList()
{

}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void TypeInfo::RegisterProperty(Property* prop)
{
	LN_VERIFY_RETURN(!prop->m_registerd);
	LN_VERIFY_RETURN(m_propertyList.GetCount() < 32);

	//if (!prop->m_registerd)
	{
		prop->m_localIndex = m_propertyList.GetCount();
		m_propertyList.Add(prop);
		prop->m_registerd = true;
	}
}
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Property* TypeInfo::FindProperty(const String& name) const
{
	// とりあえず線形探索。現在の使用用途としてそれほど大量に追加しないため。
	for (auto prop : m_propertyList)
	{
		if (prop->GetName() == name) {
			return prop;
		}
	}
	// ベースクラスも探してみる
	if (m_baseClass != NULL) {
		return m_baseClass->FindProperty(name);
	}
	//Property* prop;
	//if (m_propertyList.TryGetValue(name, &prop)) {
	//	return prop;
	//}
	return NULL;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Property* TypeInfo::FindInheritanceProperty(const Property* childObjProp, CoreObject* childObj) const
{
	Property* subKey = childObjProp->GetPropertyInstanceData(childObj)->InheritanceKey;
	for (auto prop : m_propertyList)
	{
		if (prop == childObjProp || prop == subKey) {
			return prop;
		}
	}

	// ベースクラスも探してみる
	if (m_baseClass != NULL) {
		return m_baseClass->FindInheritanceProperty(childObjProp, childObj);
	}

	return NULL;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void TypeInfo::RegisterRoutedEvent(RoutedEvent* ev)
{
	LN_VERIFY_RETURN(!ev->m_registerd);
	//if (!ev->m_registerd)
	{
		m_routedEventList.Add(ev);
		ev->m_registerd = true;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
RoutedEvent* TypeInfo::FindRoutedEvent(const String& name) const
{
	// とりあえず線形探索。現在の使用用途としてそれほど大量に追加しないため。
	for (auto ev : m_routedEventList)
	{
		if (ev->GetName() == name) {
			return ev;
		}
	}
	// ベースクラスも探してみる
	if (m_baseClass != NULL) {
		return m_baseClass->FindRoutedEvent(name);
	}
	return NULL;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void TypeInfo::InvokeRoutedEvent(CoreObject* owner, const RoutedEvent* ev, RoutedEventArgs* e)
{
	for (RoutedEvent* dynamicEvent : m_routedEventList)
	{
		if (dynamicEvent == ev) {
			// owner に AddHandler されているイベントハンドラを呼び出す。
			dynamicEvent->CallEvent(owner, e);
			return;	// ev と同じイベントは1つしかリスト内に無いはず
		}
	}

	// さらにベースクラスを見に行く
	if (m_baseClass != NULL) {
		m_baseClass->InvokeRoutedEvent(owner, ev, e);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void TypeInfo::RegisterRoutedEventHandler(const RoutedEvent* ev, RoutedEventHandler* handler)
{
	LN_VERIFY_RETURN(ev->m_registerd);
	LN_VERIFY_RETURN(!handler->m_registerd);
	m_routedEventHandlerList.Add(ev, handler);
	handler->m_registerd = true;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
RoutedEventHandler* TypeInfo::FindRoutedEventHandler(const RoutedEvent* ev) const
{
	LN_VERIFY_RETURNV(ev != NULL, NULL);
	RoutedEventHandler* handler;
	if (m_routedEventHandlerList.TryGetValue(ev, &handler))
	{
		return handler;
	}

	// さらにベースクラスを見に行く
	if (m_baseClass != NULL) {
		return m_baseClass->FindRoutedEventHandler(ev);
	}
	return NULL;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void TypeInfo::ForEachAllProperty(const TypeInfo* typeInfo, const std::function<void(Property*)>& callback)
{
	for (Property* prop : typeInfo->m_propertyList)
	{
		callback(prop);
	}

	if (typeInfo->m_baseClass != NULL) {
		ForEachAllProperty(typeInfo->m_baseClass, callback);
	}
}


//=============================================================================
// CoreObjectCollection
//=============================================================================

CoreObjectCollection::CoreObjectCollection(CoreObject* owner)
	: m_owner(owner)
{

}
void CoreObjectCollection::InsertItem(int index, const value_type& item)
{
	LN_CHECK_STATE(item->GetInheritanceParent() == NULL);
	Collection::InsertItem(index, item);
	item->SetInheritanceParent(m_owner);
}
void CoreObjectCollection::ClearItems()
{
	for (CoreObject* obj : *this) {
		obj->SetInheritanceParent(NULL);
	}
	Collection::ClearItems();
}
void CoreObjectCollection::RemoveItem(int index)
{
	GetAt(index)->SetInheritanceParent(NULL);
	Collection::RemoveItem(index);
}
void CoreObjectCollection::SetItem(int index, const value_type& item)
{
	LN_CHECK_STATE(item->GetInheritanceParent() == NULL);
	Collection::SetItem(index, item);
	item->SetInheritanceParent(m_owner);
}

//=============================================================================
// CoreObject
//=============================================================================

//LN_DEFINE_ROUTED_EVENT(CoreObject, PropertyChangedEventArgs, PropertyChangedEvent, "PropertyChanged", PropertyChanged);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
CoreObject::CoreObject()
	: m_inheritanceParent(NULL)
	, m_inheritanceChildren(this)
	, m_userData(NULL)
	, m_propertyDataStore(NULL)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
CoreObject::~CoreObject()
{
	if (m_inheritanceParent != NULL) {
		m_inheritanceParent->GetInheritanceChildren().Remove(this);
	}

	LN_SAFE_DELETE(m_propertyDataStore);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
//void CoreObject::SetPropertyValue(const String& propertyName, const Variant& value)
//{
//	Property* prop = GetThisTypeInfo()->FindProperty(propertyName);
//	if (prop != NULL)
//	{
//		SetPropertyValue(prop, value);
//		return;
//	}
//	// キーが無ければ例外
//	LN_THROW(0, KeyNotFoundException);
//}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CoreObject::SetPropertyValue(const Property* prop, const Variant& value)
{
	SetPropertyValueInternal(prop, value, false);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
//Variant CoreObject::GetPropertyValue(const String& propertyName) const
//{
//	Property* prop = GetThisTypeInfo()->FindProperty(propertyName);
//	if (prop != NULL)
//	{
//		//return prop->GetValue(this);
//		return GetPropertyValue(prop);
//	}
//
//	//Variant value;
//	//if (m_propertyDataStore.TryGetValue(propertyName, &value))
//	//{
//	//	return value;
//	//}
//	LN_THROW(0, ArgumentException);
//}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Variant CoreObject::GetPropertyValue(const Property* prop) const
{
	if (prop->IsStored())
	{
		if (m_propertyDataStore == NULL) {
			return prop->GetMetadata()->GetDefaultValue();
		}
		//LN_THROW(m_propertyDataStore != NULL, KeyNotFoundException);
		Variant v;
		if (m_propertyDataStore->TryGetValue(prop, &v)) {
			return v;
		}
		return prop->GetMetadata()->GetDefaultValue();
	}
	else
	{
		// この const_cast は、外に公開する Getter はとにかく const 関数にしたかったためのもの。
		UpdateInheritanceProperty(const_cast<CoreObject*>(this), prop);
		return prop->GetValue(this);
	}

	//return GetPropertyValue(prop->GetName());	//TODO
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
String CoreObject::ToString()
{
	return String::GetEmpty();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
//void CoreObject::RegisterProperty(const String& propertyName, const Variant& defaultValue)
//void CoreObject::RegisterProperty(Property* prop)
//{
//	m_propertyList.Add(prop->GetName(), prop);
//	//m_propertyDataStore.Add(propertyName, defaultValue);
//}
//

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CoreObject::NotifyPropertyChange(const Property* prop, const Variant& newValue, const Variant& oldValue/*PropertyChangedEventArgs* e*/)
{
	// TODO: スタックに確保するのは危険。言語バインダで使えなくなる。
	PropertyChangedEventArgs e(prop, newValue, oldValue);
	OnPropertyChanged(&e);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CoreObject::SetInheritanceParent(CoreObject* obj)
{
	// 新しく追加した場合や取り外した場合、this 以下の全ての子オブジェクトは、
	// Inherits なプロパティの参照パス (どの親のどのプロパティを元に継承するか) を更新しなければならない。
	// ここでは Inherits なプロパティの参照パスを全て NULL (初期状態) にして、後で必要になったときに更新されるようにしている。
	if (m_inheritanceParent != obj)
	{
		if (m_inheritanceParent != NULL)	// 新しく追加されたとき or 別のオブジェクトの子に移動するとき
		{
			const auto onObj = [](CoreObject* obj)
			{
				const auto onProp = [obj](Property* prop)
				{
					if (prop->GetMetadata()->GetPropertyOptions().TestFlag(PropertyOptions::Inherits))
					{
						PropertyInstanceData* data = prop->GetPropertyInstanceData(obj);
						data->InheritanceParent = NULL;
						data->InheritanceTarget = NULL;
					}
				};

				TypeInfo::ForEachAllProperty(GetTypeInfo(obj), onProp);
			};

			// 自分自身
			onObj(this);
			// と、子オブジェクト
			ForEachInheritanceChildrenHierarchical(this, onObj);
		}

		m_inheritanceParent = obj;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CoreObject::ForEachInheritanceChildrenHierarchical(CoreObject* parent, const std::function<void(CoreObject*)>& callback)
{
	for (CoreObject* obj : parent->m_inheritanceChildren)
	{
		callback(obj);
		ForEachInheritanceChildrenHierarchical(obj, callback);
	}
}

//-----------------------------------------------------------------------------
// obj の prop プロパティが継承設定であれば、直近の親から値を持ってきてセットする。
//-----------------------------------------------------------------------------
void CoreObject::UpdateInheritanceProperty(CoreObject* obj, const Property* prop)
{
	assert(prop != NULL);
	if (prop->GetMetadata()->GetPropertyOptions().TestFlag(PropertyOptions::Inherits))
	{
		PropertyInstanceData* data = prop->GetPropertyInstanceData(obj);
		if (data->IsDefault)
		{
			bool pathUpdate = false;
			bool revisionUpdate = false;

			PropertyInstanceData* targetPropData = NULL;
			if (data->InheritanceParent != NULL && data->InheritanceTarget != NULL) {
				targetPropData = data->InheritanceTarget->GetPropertyInstanceData(data->InheritanceParent);
			}

			if (data->InheritanceParent == NULL && data->InheritanceTarget == NULL) {
				pathUpdate = true;		// 親要素が変わった後の最初の Update
			}
			else if (targetPropData != NULL && data->PathRevisionCount != targetPropData->PathRevisionCount) {
				pathUpdate = true;		// 継承元の再設定が必要
			}
			if (targetPropData != NULL && data->RevisionCount != targetPropData->RevisionCount) {
				revisionUpdate = true;	// 自分と継承元の RevisionCount が異なっている。値を再設定する必要あり。
			}

			bool updated = false;
			if (pathUpdate)
			{
				// まず、継承できるプロパティを持つ最も近い親を更新しに行く。
				// (親に対してもこの関数、UpdateInheritanceProperty() を呼び出し、値を更新する)
				if (obj->m_inheritanceParent != NULL)
				{
					CoreObject* nearParent = obj->m_inheritanceParent;
					Property* targetProp = NULL;
					while (true)
					{
						targetProp = GetTypeInfo(nearParent)->FindInheritanceProperty(prop, obj);
						if (targetProp != NULL) {
							break;	// 見つかった。この targetProp から継承できる。
						}
						if (nearParent->m_inheritanceParent == NULL)
						{
							// ルート要素まで上ってしまった。ルート要素を継承元とする。
							nearParent = nearParent->m_inheritanceParent;
							break;
						}
						nearParent = nearParent->m_inheritanceParent;
					};

					if (nearParent != NULL)
					{
						// 見つかった直近の親を更新する
						UpdateInheritanceProperty(nearParent, prop);
						// 更新が無事終われば、targetProp から継承元の値が取れる
						obj->SetPropertyValueInternal(prop, nearParent->GetPropertyValue(targetProp), true);
						updated = true;
						// 継承元を覚えておく
						data->InheritanceParent = nearParent;
						data->InheritanceTarget = targetProp;
						PropertyInstanceData* d = targetProp->GetPropertyInstanceData(nearParent);
						data->RevisionCount = d->RevisionCount;
						data->PathRevisionCount = d->PathRevisionCount;
					}
				}
			}
			else if (revisionUpdate)
			{
				obj->SetPropertyValueInternal(prop, data->InheritanceParent->GetPropertyValue(data->InheritanceTarget), true);
				updated = true;
				data->RevisionCount = targetPropData->RevisionCount;
			}

			// いろいろやったけど継承元が見つからなかった。普通に規定値を使う
			if (!updated && data->InheritanceTarget == NULL)
			{
				// もう↑の処理をまわす必要は無い。「処理済」をマークするために値を入れておく。
				data->InheritanceTarget = prop;
				// 規定値
				obj->SetPropertyValueInternal(prop, prop->GetMetadata()->GetDefaultValue(), true);
			}
		}
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CoreObject::SetPropertyValueInternal(const Property* prop, const Variant& value, bool reset)
{
	if (prop->IsStored())
	{
		// 必要になったので作る
		if (m_propertyDataStore == NULL) { m_propertyDataStore = LN_NEW PropertyDataStore(); }
		m_propertyDataStore->SetValue(prop, value);
	}
	else {
		prop->SetValue(this, value);
	}

	PropertyInstanceData* data = prop->GetPropertyInstanceData(this);
	if (data != NULL)
	{
		if (reset) {
			data->IsDefault = true;
		}
		else
		{
			if (data->IsDefault == true)
			{
				// 新しく設定される瞬間、これまで継承元として参照していたプロパティと this に対して
				// プロパティ参照更新値を1つ進める。子は Get しようとしたとき、継承元を再検索する。
				// TODO: SetTypedPropertyValue にも同じようなのがある。共通化したい。あとテスト
				if (data->InheritanceParent != NULL) {
					data->InheritanceTarget->GetPropertyInstanceData(data->InheritanceParent)->PathRevisionCount++;
				}
				data->PathRevisionCount++;
			}
			data->IsDefault = false;
			data->RevisionCount++;
		}
	}

	//SetPropertyValue(prop->GetName(), value);	// TODO: GetName じゃなくて、型情報も考慮するように。あるいは生ポインタ
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CoreObject::RaiseEvent(EventSlotBase& eventSlot, EventArgs* e)
{
	eventSlot.Raise(e);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CoreObject::RaiseEventInternal(const RoutedEvent* ev, RoutedEventArgs* e)
{
	LN_VERIFY_RETURN(ev != NULL);
	LN_VERIFY_RETURN(e != NULL);

	TypeInfo* thisType = GetThisTypeInfo();

	// 内部的、および派生クラスで使用する private なハンドラ。
	// これらは ユーザーが AddHandler() できるイベントよりも優先して実行する。
	RoutedEventHandler* handler = thisType->FindRoutedEventHandler(ev);	// TOOD: 1つだけ検索じゃなくて、for で回して全部見た方が丁寧かも
	if (handler != NULL)
	{
		handler->Call(this, e);
		if (e->Handled) {
			return;
		}
	}

	// this に AddHandler されているイベントハンドラを呼び出す。
	thisType->InvokeRoutedEvent(this, ev, e);

	//if (ev->GetOwnerClassTypeInfo() == )
	//ev->CallEvent(this, e);
	//for (RoutedEvent* dynamicEvent : m_routedEventList)
	//{
	//	if (pair.first == eventName) {
	//		pair.second->CallEvent(this/*, sender*/, e);
	//		break;	// ev と同じイベントは1つしかリスト内に無いはず
	//	}
	//}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CoreObject::OnPropertyChanged(PropertyChangedEventArgs* e)
{
	// e->Property を持つクラスのコールバックを呼び出す
	e->ChangedProperty->NotifyPropertyChange(this, e);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool CoreObject::HasLocalPropertyValue(const Property* prop)
{
	LN_VERIFY_RETURNV(prop != NULL, false);

	uint32_t f = 0x1 << prop->GetLocalIndex();
	uint32_t flags = *prop->GetOwnerClassType()->GetHasLocalValueFlags(this);
	if ((flags & f) != 0) {
		return true;
	}
	return false;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
TypeInfo CoreObject::m_typeInfo(_T("CoreObject"), NULL, &CoreObject::GetHasLocalValueFlags);
TypeInfo* CoreObject::GetThisTypeInfo() const { return &m_typeInfo; }
TypeInfo* CoreObject::GetClassTypeInfo() { return &m_typeInfo; }
	
//=============================================================================
// Variant
//=============================================================================

const Variant Variant::Null;

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Variant::Variant()
	: m_type(VariantType_Unknown)
	, m_uint(0)	// union 全てを初期化
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Variant::Variant(const Variant& obj)
	: m_type(VariantType_Unknown)
	, m_uint(0)	// union 全てを初期化
{
	Copy(obj);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Variant::Variant(bool value)
	: m_type(VariantType_Bool)
	, m_bool(value)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Variant::Variant(int value)
	: m_type(VariantType_Int)
	, m_int(value)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Variant::Variant(float value)
	: m_type(VariantType_Float)
	, m_float(value)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Variant::Variant(const String& value)
	: m_type(VariantType_String)
	, m_string(value)
{

}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Variant::Variant(CoreList* value)
	: m_type(VariantType_Unknown)
	, m_uint(0)
{
	SetList(value);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Variant::Variant(CoreObject* obj)
	: m_type(VariantType_Unknown)
	, m_uint(0)	// union 全てを初期化
{
	Set(obj);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Variant::Variant(const Enum& value)
	: m_type(VariantType_Unknown)
	, m_uint(0)	// union 全てを初期化
{
	SetInt(value);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Variant::Variant(const SizeF& value)
	: m_type(VariantType_Unknown)
	, m_uint(0)
{
	SetSizeF(value);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Variant::Variant(const Rect& value)
	: m_type(VariantType_Unknown)
	, m_uint(0)
{
	SetRect(value);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Variant::Variant(const ThicknessF& value)
	: m_type(VariantType_Unknown)
	, m_uint(0)
{
	SetThicknessF(value);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Variant::Variant(const ToneF& value)
	: m_type(VariantType_Unknown)
	, m_uint(0)
{
	SetToneF(value);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Variant::~Variant()
{
	Release();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool Variant::GetBool() const
{
	LN_CHECK_STATE_RETURNV(m_type == VariantType_Bool, false);
	return m_bool;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Variant::SetInt(int value)
{
	Release();
	m_type = VariantType_Int;
	m_int = value;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
int Variant::GetInt() const
{
	LN_CHECK_STATE_RETURNV(m_type == VariantType_Int, 0);
	return (int)m_int;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Variant::SetFloat(float value)
{
	Release();
	m_type = VariantType_Float;
	m_float = value;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
float Variant::GetFloat() const
{
	LN_CHECK_STATE_RETURNV(m_type == VariantType_Float, 0.0f);
	return m_float;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Variant::SetString(const String& value)
{
	Release();
	m_type = VariantType_String;
	m_string = value;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
String Variant::GetString() const
{
	LN_CHECK_STATE_RETURNV(m_type == VariantType_String, String::GetEmpty());
	return m_string;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Variant::SetList(CoreList* value)
{
	Release();
	m_type = VariantType_List;
	LN_REFOBJ_SET(m_valueList, value);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
CoreList* Variant::GetList() const
{
	LN_CHECK_STATE_RETURNV(m_type == VariantType_List, NULL);
	return m_valueList;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Variant::Set(CoreObject* obj)
{
	Release();
	m_type = VariantType_Object;
	LN_REFOBJ_SET(m_object, obj);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
CoreObject* Variant::GetObject() const
{
	LN_CHECK_STATE(m_type == VariantType_Object || m_type == VariantType_List);	// List も Object の一部。
	return m_object;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Variant::SetSizeF(const SizeF& value)
{
	Release();
	m_type = VariantType_SizeF;
	*((SizeF*)m_sizeF) = value;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
const SizeF& Variant::GetSizeF() const
{
	LN_CHECK_STATE_RETURNV(m_type == VariantType_SizeF, SizeF::Zero);
	return *((SizeF*)m_sizeF);
}
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Variant::SetRect(const Rect& value)
{
	Release();
	m_type = VariantType_Rect;
	*((Rect*)m_rect) = value;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
const Rect& Variant::GetRect() const
{
	LN_CHECK_STATE_RETURNV(m_type == VariantType_Rect, Rect::Zero);
	return *((Rect*)m_rect);
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Variant::SetThicknessF(const ThicknessF& value)
{
	Release();
	m_type = VariantType_ThicknessF;
	*((ThicknessF*)m_thicknessF) = value;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
const ThicknessF& Variant::GetThicknessF() const
{
	LN_CHECK_STATE_RETURNV(m_type == VariantType_ThicknessF, ThicknessF::Zero);
	return *((ThicknessF*)m_thicknessF);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Variant::SetToneF(const ToneF& value)
{
	Release();
	m_type = VariantType_ToneF;
	*((ToneF*)m_toneF) = value;
}
const ToneF& Variant::GetToneF() const
{
	LN_CHECK_STATE_RETURNV(m_type == VariantType_ToneF, ToneF::Zero);
	return *((ToneF*)m_toneF);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool Variant::operator == (const Variant& right) const
{
	if (m_type != right.m_type) { return false; }
	switch (m_type)
	{
	case Lumino::VariantType_Unknown:	return true;
	case Lumino::VariantType_Bool:		return m_bool == right.m_bool;
	case Lumino::VariantType_Int:		return m_int == right.m_int;
	case Lumino::VariantType_Float:		return m_float == right.m_float;
	case Lumino::VariantType_String:	return m_string == right.m_string;
	case Lumino::VariantType_List:		return m_valueList == right.m_valueList;
	case Lumino::VariantType_Object:	return m_object == right.m_object;
	case Lumino::VariantType_SizeF:		return *((SizeF*)m_sizeF) == *((SizeF*)right.m_sizeF);
	case Lumino::VariantType_Rect:		return *((Rect*)m_rect) == *((Rect*)right.m_rect);
	case Lumino::VariantType_ThicknessF:return *((ThicknessF*)m_thicknessF) == *((ThicknessF*)right.m_thicknessF);
	default: LN_ASSERT(0); return false;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Variant::Copy(const Variant& obj)
{
	Release();
	m_type = obj.m_type;
	switch (m_type)
	{
	case Lumino::VariantType_Unknown:
		break;
	case Lumino::VariantType_Bool:
		m_bool = obj.m_bool;
		break;
	case Lumino::VariantType_Int:
		m_int = obj.m_int;
		break;
	case Lumino::VariantType_Float:
		m_float = obj.m_float;
		break;
	case Lumino::VariantType_String:
		m_string = obj.m_string;
		break;
	case Lumino::VariantType_List:
		LN_REFOBJ_SET(m_valueList, obj.m_valueList);
		break;
	case Lumino::VariantType_Object:
		LN_REFOBJ_SET(m_object, obj.m_object);
		break;
	case Lumino::VariantType_SizeF:
		memcpy(m_sizeF, obj.m_sizeF, sizeof(m_sizeF));
		break;
	case Lumino::VariantType_Rect:
		memcpy(m_rect, obj.m_rect, sizeof(m_rect));
		break;
	case Lumino::VariantType_ThicknessF:
		memcpy(m_thicknessF, obj.m_thicknessF, sizeof(m_thicknessF));
		break;
	case Lumino::VariantType_ToneF:
		memcpy(m_toneF, obj.m_toneF, sizeof(m_toneF));
		break;
	default:
		LN_ASSERT(0);
		break;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Variant::Release()
{
	if (m_type == VariantType_List) {
		LN_SAFE_RELEASE(m_valueList);
	}
	else if (m_type == VariantType_Object) {
		LN_SAFE_RELEASE(m_object);
	}
	m_uint = 0;	// union 全てを初期化
}

} // namespace Lumino
