
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
	setString(String(value));
}
JsonValue::JsonValue(const String& value)
	: m_type(JsonType::Null)
	, m_uint(0)
{
	setString(value);
}
JsonValue::JsonValue(const JsonValue& value)
	: m_type(JsonType::Null)
	, m_uint(0)
{
	copy(value);
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
	case JsonType::String:	setString(String::getEmpty()); break;
	case JsonType::Array:	SetArray(); break;
	case JsonType::Object:	SetObject(); break;
	}
}

//------------------------------------------------------------------------------
JsonValue::~JsonValue()
{
	detach();
}

//------------------------------------------------------------------------------
JsonValue& JsonValue::operator=(const JsonValue& obj)
{
	copy(obj);
	return (*this);
}

//------------------------------------------------------------------------------
void JsonValue::SetNull()
{
	detach();
	m_type = JsonType::Null;
}

//------------------------------------------------------------------------------
void JsonValue::SetBool(bool value)
{
	detach();
	m_type = JsonType::Bool;
	m_bool = value;
}

//------------------------------------------------------------------------------
bool JsonValue::GetBool() const
{
	if (LN_CHECK_STATE(m_type == JsonType::Bool)) return false;
	return m_bool;
}

//------------------------------------------------------------------------------
void JsonValue::SetDouble(double value)
{
	detach();
	m_type = JsonType::Double;
	m_double = value;
}

//------------------------------------------------------------------------------
double JsonValue::GetDouble() const
{
	if (LN_CHECK_STATE(m_type == JsonType::Double)) return 0.0;
	return m_double;
}

//------------------------------------------------------------------------------
void JsonValue::setString(const String& str)
{
	detach();
	m_type = JsonType::String;
	m_string = LN_NEW String(str);
}

//------------------------------------------------------------------------------
const String& JsonValue::getString() const
{
	if (LN_CHECK_STATE(m_type == JsonType::String && m_string != NULL)) return String::getEmpty();
	return *m_string;
}

//------------------------------------------------------------------------------
void JsonValue::SetArray()
{
	detach();
	m_type = JsonType::Array;
	m_valueList = LN_NEW ValueList();
}

//------------------------------------------------------------------------------
int JsonValue::GetItemCount() const
{
	if (LN_CHECK_STATE(m_type == JsonType::Array && m_valueList != NULL)) return 0;
	return m_valueList->getCount();
}

//------------------------------------------------------------------------------
void JsonValue::AddItem(const JsonValue& value)
{
	if (LN_CHECK_STATE(m_type == JsonType::Array && m_valueList != NULL)) return;
	m_valueList->add(value);
}

//------------------------------------------------------------------------------
JsonValue& JsonValue::operator[](int index)
{
	LN_VERIFY_STATE(m_type == JsonType::Array && m_valueList != NULL);
	return m_valueList->getAt(index);
}
const JsonValue& JsonValue::operator[](int index) const
{
	LN_VERIFY_STATE(m_type == JsonType::Array && m_valueList != NULL);
	return m_valueList->getAt(index);
}

//------------------------------------------------------------------------------
void JsonValue::SetObject()
{
	detach();
	m_type = JsonType::Object;
	m_memberList = LN_NEW MemberList();
}

//------------------------------------------------------------------------------
int JsonValue::GetMemberCount() const
{
	if (LN_CHECK_STATE(m_type == JsonType::Object && m_memberList != NULL)) return 0;
	return m_memberList->getCount();
}

//------------------------------------------------------------------------------
void JsonValue::AddMember(const String& name, const JsonValue& value)
{
	if (LN_CHECK_STATE(m_type == JsonType::Object && m_memberList != NULL)) return;
	JsonMember m;
	m.Name = name;
	m.Value = value;
	m_memberList->add(m);
}

//------------------------------------------------------------------------------
void JsonValue::copy(const JsonValue& obj)
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
		setString(obj.getString());
	}
	else if (obj.m_type == JsonType::Array)
	{
		detach();
		m_type = JsonType::Array;
		m_valueList = LN_NEW ValueList(*obj.m_valueList);
	}
	else if (obj.m_type == JsonType::Object)
	{
		detach();
		m_type = JsonType::Object;
		m_memberList = LN_NEW MemberList(*obj.m_memberList);
	}
	// ↑もし同じ型であれば new しないようにすればもう少しパフォーマンス上げられる
}

//------------------------------------------------------------------------------
void JsonValue::detach()
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
