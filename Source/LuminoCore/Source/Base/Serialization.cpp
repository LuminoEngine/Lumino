/*


MyObject*			m_obj1;
RefPtr<MyObject>*	m_obj2;
MyObject			m_obj3;

ar & NewObjectNVP("obj1", m_obj1);	// 良くない
ar & MakeNVP("obj1", *m_obj1);		// OK (あらかじめ new)
ar & NewObjectNVP("obj2", m_obj2);	// OK	→ MakeMVP で RefPtr<> のみ特殊扱い、で。そうすると List のシリアライズが楽になる
ar & MakeNVP("obj", m_obj3);		// OK


List<MyObject*>			m_list1;	// だめ
List<RefPtr<MyObject>>	m_list2;

ar & MakeNVP("list2", m_list2);

リストのシリアライズについて・・・
non‐intrusive Object のシリアライズ扱いにする。
コレクションは push_back を実装していること。とか。




*/
#include "../Internal.h"
#include <Lumino/Reflection/TypeInfo.h>
#include <Lumino/Base/Serialization.h>
//#include <Lumino/IO/FileStream.h>

LN_NAMESPACE_BEGIN
namespace tr {

//==============================================================================
// ScVariantCore
//==============================================================================

//------------------------------------------------------------------------------
ScVariantCore::~ScVariantCore()
{
	ReleaseValue();
}

//------------------------------------------------------------------------------
void ScVariantCore::SetInt(int value)
{
	ResetType(ScVariantType::Int);
	m_int = value;
}

//------------------------------------------------------------------------------
void ScVariantCore::SetString(const StringRef& value)
{
	if (m_type != ScVariantType::String)
	{
		ResetType(ScVariantType::String);
		m_string = LN_NEW String(value);
	}
	else
	{
		*m_string = value;
	}
}

//------------------------------------------------------------------------------
void ScVariantCore::SetList()
{
	ResetType(ScVariantType::List);
	m_list = LN_NEW ln::List<ScVariant>();
}

//------------------------------------------------------------------------------
void ScVariantCore::SetMap()
{
	ResetType(ScVariantType::Map);
	m_map = LN_NEW std::unordered_map<ln::String, ScVariant>();
}

////------------------------------------------------------------------------------
//void ScVariantCore::Serialize(Archive& ar, int version)
//{
//	if (ar.IsSaving())
//	{
//
//	}
//	else
//	{
//
//	}
//}

//------------------------------------------------------------------------------
void ScVariantCore::ResetType(ScVariantType type)
{
	ReleaseValue();
	m_type = type;
}

//------------------------------------------------------------------------------
void ScVariantCore::ReleaseValue()
{
	switch (m_type)
	{
	case ScVariantType::Unknown:
		break;
	case ScVariantType::Bool:
		break;
	case ScVariantType::Int:
		break;
	case ScVariantType::Float:
		break;
	case ScVariantType::String:
		LN_SAFE_DELETE(m_string);
		break;
	case ScVariantType::List:
		LN_SAFE_DELETE(m_list);
		break;
	case ScVariantType::Map:
		LN_SAFE_DELETE(m_map);
		break;
	default:
		LN_UNREACHABLE();
		break;
	}
	m_type = ScVariantType::Unknown;
}

//==============================================================================
// ScVariant
//==============================================================================

//------------------------------------------------------------------------------
void ScVariant::SetInt(int value)
{
	if (m_core == nullptr) m_core = NewObject<ScVariantCore>();
	m_core->SetInt(value);
}

//------------------------------------------------------------------------------
void ScVariant::SetString(const StringRef& value)
{
	if (m_core == nullptr) m_core = NewObject<ScVariantCore>();
	m_core->SetString(value);
}

//------------------------------------------------------------------------------
void ScVariant::SaveInternal(ISerializeElement* value)
{

}

//------------------------------------------------------------------------------
void ScVariant::LoadInternal(ISerializeElement* value)
{
	switch (value->GetSerializationElementType())
	{
	case SerializationElementType::Value:
	{
		SerializationValueType type = value->GetSerializationValueType();
		if (type == SerializationValueType::Null)
		{
			m_core = nullptr;
		}
		else
		{
			if (m_core == nullptr)
			{
				m_core = NewObject<ScVariantCore>();
			}
			switch (type)
			{
			case SerializationValueType::Bool:
				m_core->ResetType(ScVariantType::Bool);
				m_core->m_bool = value->GetSerializeValueBool();
				break;
			case SerializationValueType::Int8:
				m_core->ResetType(ScVariantType::Int);
				m_core->m_bool = value->GetSerializeValueInt8();
				break;
			case SerializationValueType::Int16:
				m_core->ResetType(ScVariantType::Int);
				m_core->m_bool = value->GetSerializeValueInt16();
				break;
			case SerializationValueType::Int32:
				m_core->ResetType(ScVariantType::Int);
				m_core->m_bool = value->GetSerializeValueInt32();
				break;
			case SerializationValueType::Int64:
				m_core->ResetType(ScVariantType::Int);
				m_core->m_bool = value->GetSerializeValueInt64();
				break;
			case SerializationValueType::UInt8:
				m_core->ResetType(ScVariantType::Int);
				m_core->m_bool = value->GetSerializeValueUInt8();
				break;
			case SerializationValueType::UInt16:
				m_core->ResetType(ScVariantType::Int);
				m_core->m_bool = value->GetSerializeValueUInt16();
				break;
			case SerializationValueType::UInt32:
				m_core->ResetType(ScVariantType::Int);
				m_core->m_bool = value->GetSerializeValueUInt32();
				break;
			case SerializationValueType::UInt64:
				m_core->ResetType(ScVariantType::Int);
				m_core->m_bool = value->GetSerializeValueUInt64();
				break;
			case SerializationValueType::Float:
				m_core->ResetType(ScVariantType::Float);
				m_core->m_bool = value->GetSerializeValueFloat();
				break;
			case SerializationValueType::Double:
				m_core->ResetType(ScVariantType::Float);
				m_core->m_bool = value->GetSerializeValueDouble();
				break;
			case SerializationValueType::String:
				m_core->SetString(value->GetSerializeValueString());
				break;
			default:
				LN_UNREACHABLE();
				break;
			}
		}
		break;
	}
	case SerializationElementType::Array:
	{
		if (m_core == nullptr)
		{
			m_core = NewObject<ScVariantCore>();
		}
		m_core->SetList();
		int count = value->GetSerializeElementCount();
		for (int i = 0; i < count; i++)
		{
			ScVariant v;
			v.LoadInternal(value->GetSerializeElement(i));
			m_core->m_list->Add(v);
		}
		break;
	}
	case SerializationElementType::Object:
	{
		if (m_core == nullptr)
		{
			m_core = NewObject<ScVariantCore>();
		}
		m_core->SetMap();
		int count = value->GetSerializeElementCount();
		for (int i = 0; i < count; i++)
		{
			ScVariant v;
			v.LoadInternal(value->GetSerializeElement(i));
			(*m_core->m_map)[value->GetSerializeElementName(i)] = v;
		}
		break;
	}
	default:
		LN_UNREACHABLE();
		break;
	}
}



//==============================================================================
// Archive
//==============================================================================
const TCHAR* Archive::ClassNameKey = _T("_ln_class_name");
const TCHAR* Archive::ClassVersionKey = _T("_ln_class_version");
const TCHAR* Archive::ClassBaseDefaultNameKey = _T("_ln_class_base");

//------------------------------------------------------------------------------
RefPtr<ReflectionObject> Archive::CreateObject(const String& className, TypeInfo* requestedType)
{
	if (className == requestedType->GetName())
	{
		// 格納したい変数の型と同じなら型情報を総検索する必要は無い
		return requestedType->CreateInstance();
	}
	else
	{
		auto* typeInfo = TypeInfo::FindTypeInfo(className);
		return typeInfo->CreateInstance();
	}
}

////------------------------------------------------------------------------------
//Archive::Archive(const PathName& filePath, ArchiveMode mode)
//	: m_mode(mode)
//{
//	FileStreamPtr file;
//	switch (mode)
//	{
//	case ArchiveMode::Save:
//		file = FileStream::Create(filePath, FileOpenMode::Write | FileOpenMode::Truncate);
//		break;
//	case ArchiveMode::Load:
//		file = FileStream::Create(filePath, FileOpenMode::Read);
//		break;
//	default:
//		assert(0);
//		break;
//	}
//}

} // namespace tr
LN_NAMESPACE_END
