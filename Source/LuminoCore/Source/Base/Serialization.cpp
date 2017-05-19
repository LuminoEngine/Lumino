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
// Archive
//==============================================================================
const TCHAR* Archive::ClassNameKey = _T("_ln_class_name");
const TCHAR* Archive::ClassVersionKey = _T("_ln_class_version");
const TCHAR* Archive::ClassBaseDefaultNameKey = _T("_ln_base_class");

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
