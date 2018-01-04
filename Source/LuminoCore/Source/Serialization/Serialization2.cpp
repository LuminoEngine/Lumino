
#include "../Internal.h"
#include <Lumino/Reflection/TypeInfo.h>
#include <Lumino/Serialization/Serialization2.h>

LN_NAMESPACE_BEGIN

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
const Char* Archive2::ArchiveVersionKey = _TT("lumino_archive_version");
const Char* Archive2::ArchiveRootValueKey = _TT("lumino_archive_root");

//Archive2::Archive2(tr::ISerializationeStore* stream, ArchiveMode mode, bool refrectionSupported)
//{
//}
//
//Archive2::~Archive2()
//{
//}

LN_NAMESPACE_END
