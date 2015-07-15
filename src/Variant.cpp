
#include "Internal.h"
#include <Lumino/Variant.h>
#include <Lumino/Property.h>

namespace Lumino
{

//=============================================================================
// TypeInfo
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void TypeInfo::RegisterProperty(Property* prop)
{
	if (!prop->m_registerd)
	{
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
	
//=============================================================================
// CoreObject
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
CoreObject::CoreObject()
	: m_userData(NULL)
	, m_propertyDataStore(NULL)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
CoreObject::~CoreObject()
{
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
	if (prop->IsStored())
	{
		// 必要になったので作る
		if (m_propertyDataStore == NULL) { m_propertyDataStore = LN_NEW PropertyDataStore(); }
		m_propertyDataStore->SetValue(prop, value);
	}
	else {
		prop->SetValue(this, value);
	}

	//SetPropertyValue(prop->GetName(), value);	// TODO: GetName じゃなくて、型情報も考慮するように。あるいは生ポインタ
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
			return prop->GetDefaultValue();
		}
		//LN_THROW(m_propertyDataStore != NULL, KeyNotFoundException);
		Variant v;
		if (m_propertyDataStore->TryGetValue(prop, &v)) {
			return v;
		}
		return prop->GetDefaultValue();
	}
	else {
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
void CoreObject::OnPropertyChanged(const String& name, const Variant& newValue)
{
	PropertyChangedEventArgs e;
	e.PropertyName = name;
	e.NewValue = newValue;
	PropertyChanged.Raise(this, &e);
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
TypeInfo CoreObject::m_typeInfo(_T("CoreObject"), NULL);
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
Variant::Variant(VariantList* value)
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
Variant::~Variant()
{
	Release();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool Variant::GetBool() const
{
	if (LN_VERIFY_ASSERT(m_type == VariantType_Bool)) { return false; }
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
	if (LN_VERIFY_ASSERT(m_type == VariantType_Int)) { return 0; }
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
	if (LN_VERIFY_ASSERT(m_type == VariantType_Float)) { return 0; }
	return m_float;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Variant::SetList(VariantList* value)
{
	Release();
	m_type = VariantType_List;
	LN_REFOBJ_SET(m_valueList, value);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
VariantList* Variant::GetList() const
{
	if (LN_VERIFY_ASSERT(m_type == VariantType_List)) { return NULL; }
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
	if (LN_VERIFY_ASSERT(m_type == VariantType_Object)) { return NULL; }
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
	if (LN_VERIFY_ASSERT(m_type == VariantType_SizeF)) { return SizeF(); }
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
	if (LN_VERIFY_ASSERT(m_type == VariantType_Rect)) { return Rect(); }
	return *((Rect*)m_rect);
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
