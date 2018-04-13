
#include "Internal.hpp"
//#include <Lumino/Reflection/TypeInfo.h>
#include <Lumino/Serialization/Serialization2.h>

namespace ln {

/*
	# 基本的な処理の流れ
	process() にいろいろ渡される
		- 普通の値
		- オブジェクト
		- コレクション
		- MapItem
		
		
*/

//==============================================================================
// Archive2
//==============================================================================
const int Archive2::ArchiveVersion = 1;
const String Archive2::ArchiveVersionKey = _TT("lumino_archive_version");
const String Archive2::ArchiveRootValueKey = _TT("lumino_archive_root");
const String Archive2::ClassNameKey = _TT("lumino_class_name");
const String Archive2::ClassVersionKey = _TT("lumino_class_version");
const String Archive2::ClassBaseKey = _TT("lumino_base_class");

//Archive2::Archive2(tr::ISerializationeStore* stream, ArchiveMode mode, bool refrectionSupported)
//{
//}
//
//Archive2::~Archive2()
//{
//}


//==============================================================================
// JsonTextOutputArchive
//==============================================================================
JsonTextOutputArchive::JsonTextOutputArchive()
	: m_localDoc()
	, m_localStore(&m_localDoc)
	, m_processing(false)
{
	setup(&m_localStore, ArchiveMode::Save);
}

JsonTextOutputArchive::~JsonTextOutputArchive()
{
}

String JsonTextOutputArchive::toString(tr::JsonFormatting formatting)
{
	return m_localDoc.toString(formatting);
}

//==============================================================================
// JsonTextInputArchive
//==============================================================================
JsonTextInputArchive::JsonTextInputArchive(const String& jsonText)
	: m_localDoc()
	, m_localStore(&m_localDoc)
	, m_processing(false)
{
	m_localDoc.parse(jsonText);
	setup(&m_localStore, ArchiveMode::Load);
}

JsonTextInputArchive::~JsonTextInputArchive()
{
}

} // namespace ln
