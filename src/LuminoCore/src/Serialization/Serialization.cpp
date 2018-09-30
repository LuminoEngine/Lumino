
#include "Internal.hpp"
#include <LuminoCore/Serialization/Serialization.hpp>

namespace ln {

/*
	write 時の基本方針
	- 何か値を write しようとしたら、オブジェクト先頭など、何か「書き込み待ち」になっているものを書く

	文字列や配列扱いしたいオブジェクトの write 動作
	- まず普通の Object として writeValue		→ コンテナ扱い
	- serialize の中で setTag する


	read 時の基本方針
	- オブジェクト型ならまず setNextName()
	- read 使用としている型がプリミティブ型でなければ store->readContainer
	- 次に store->readValue すると、現在の Container と setNextName() した名前から値を取得する

*/

//==============================================================================
// Archive

const String Archive::ClassNameKey = _TT("lumino_class_name");
const String Archive::ClassVersionKey = _TT("lumino_class_version");
const String Archive::ClassBaseKey = _TT("lumino_base_class");

//==============================================================================
// JsonTextOutputArchive

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

String JsonTextOutputArchive::toString(JsonFormatting formatting)
{
    return m_localDoc.toString(formatting);
}

//==============================================================================
// JsonTextInputArchive

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
