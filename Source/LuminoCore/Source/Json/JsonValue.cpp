
#include "../Internal.h"
#include <Lumino/Json/JsonValue.h>

LN_NAMESPACE_BEGIN
namespace tr {

//==============================================================================
// JsonReader
//==============================================================================

static JsonValue g_InvallidValue;

//------------------------------------------------------------------------------
JsonValue::JsonValue()
	: m_type(JsonType::Null)
	, m_uint(0)
{
}
JsonValue::JsonValue(bool value)
	: m_type(JsonType::Null)
	, m_uint(0)
{
	SetBool(value);
}
JsonValue::JsonValue(double value)
	: m_type(JsonType::Null)
	, m_uint(0)
{
	SetDouble(value);
}
JsonValue::JsonValue(const TCHAR* value)
	: m_type(JsonType::Null)
	, m_uint(0)
{
	SetString(String(value));
}
JsonValue::JsonValue(const String& value)
	: m_type(JsonType::Null)
	, m_uint(0)
{
	SetString(value);
}
JsonValue::JsonValue(const JsonValue& value)
	: m_type(JsonType::Null)
	, m_uint(0)
{
	Copy(value);
}
JsonValue::JsonValue(JsonType::enum_type type)
	: m_type(JsonType::Null)
	, m_uint(0)
{
	switch (type)
	{
	case JsonType::Null:	SetNull(); break;
	case JsonType::Bool:	SetBool(false); break;
	case JsonType::Double:	SetDouble(0.0); break;
	case JsonType::String:	SetString(String::GetEmpty()); break;
	case JsonType::Array:	SetArray(); break;
	case JsonType::Object:	SetObject(); break;
	}
}

//------------------------------------------------------------------------------
JsonValue::~JsonValue()
{
	Detach();
}

//------------------------------------------------------------------------------
JsonValue& JsonValue::operator=(const JsonValue& obj)
{
	Copy(obj);
	return (*this);
}

//------------------------------------------------------------------------------
void JsonValue::SetNull()
{
	Detach();
	m_type = JsonType::Null;
}

//------------------------------------------------------------------------------
void JsonValue::SetBool(bool value)
{
	Detach();
	m_type = JsonType::Bool;
	m_bool = value;
}

//------------------------------------------------------------------------------
bool JsonValue::GetBool() const
{
	LN_CHECK_STATE(m_type == JsonType::Bool);
	return m_bool;
}

//------------------------------------------------------------------------------
void JsonValue::SetDouble(double value)
{
	Detach();
	m_type = JsonType::Double;
	m_double = value;
}

//------------------------------------------------------------------------------
double JsonValue::GetDouble() const
{
	LN_CHECK_STATE(m_type == JsonType::Double);
	return m_double;
}

//------------------------------------------------------------------------------
void JsonValue::SetString(const String& str)
{
	Detach();
	m_type = JsonType::String;
	m_string = LN_NEW String(str);
}

//------------------------------------------------------------------------------
const String& JsonValue::GetString() const
{
	LN_CHECK_STATE(m_type == JsonType::String && m_string != NULL);
	return *m_string;
}

//------------------------------------------------------------------------------
void JsonValue::SetArray()
{
	Detach();
	m_type = JsonType::Array;
	m_valueList = LN_NEW ValueList();
}

//------------------------------------------------------------------------------
int JsonValue::GetItemCount() const
{
	LN_CHECK_STATE(m_type == JsonType::Array && m_valueList != NULL);
	return m_valueList->GetCount();
}

//------------------------------------------------------------------------------
void JsonValue::AddItem(const JsonValue& value)
{
	LN_CHECK_STATE(m_type == JsonType::Array && m_valueList != NULL);
	m_valueList->Add(value);
}

//------------------------------------------------------------------------------
JsonValue& JsonValue::operator[](int index)
{
	LN_CHECK_STATE(m_type == JsonType::Array && m_valueList != NULL);
	return m_valueList->GetAt(index);
}
const JsonValue& JsonValue::operator[](int index) const
{
	LN_CHECK_STATE(m_type == JsonType::Array && m_valueList != NULL);
	return m_valueList->GetAt(index);
}

//------------------------------------------------------------------------------
void JsonValue::SetObject()
{
	Detach();
	m_type = JsonType::Object;
	m_memberList = LN_NEW MemberList();
}

//------------------------------------------------------------------------------
int JsonValue::GetMemberCount() const
{
	LN_CHECK_STATE(m_type == JsonType::Object && m_memberList != NULL);
	return m_memberList->GetCount();
}

//------------------------------------------------------------------------------
void JsonValue::AddMember(const String& name, const JsonValue& value)
{
	LN_CHECK_STATE(m_type == JsonType::Object && m_memberList != NULL);
	JsonMember m;
	m.Name = name;
	m.Value = value;
	m_memberList->Add(m);
}

//------------------------------------------------------------------------------
void JsonValue::Copy(const JsonValue& obj)
{
	if (obj.m_type == JsonType::Null) {
		SetNull();
	}
	else if (obj.m_type == JsonType::Bool) {
		SetBool(obj.GetBool());
	}
	else if (obj.m_type == JsonType::Double) {
		SetDouble(obj.GetDouble());
	}
	else if (obj.m_type == JsonType::String) {
		SetString(obj.GetString());
	}
	else if (obj.m_type == JsonType::Array)
	{
		Detach();
		m_type = JsonType::Array;
		m_valueList = LN_NEW ValueList(*obj.m_valueList);
	}
	else if (obj.m_type == JsonType::Object)
	{
		Detach();
		m_type = JsonType::Object;
		m_memberList = LN_NEW MemberList(*obj.m_memberList);
	}
	// ↑もし同じ型であれば new しないようにすればもう少しパフォーマンス上げられる
}

//------------------------------------------------------------------------------
void JsonValue::Detach()
{
	if (m_type == JsonType::String) {
		LN_SAFE_DELETE(m_string);
	}
	else if (m_type == JsonType::Array) {
		LN_SAFE_DELETE(m_valueList);
	}
	else if (m_type == JsonType::Object) {
		LN_SAFE_DELETE(m_memberList);
	}
	m_type = JsonType::Null;
}

} // namespace tr
LN_NAMESPACE_END
