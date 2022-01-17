
#pragma once
#include "Common.hpp"
#include "../Base/DateTime.hpp"
#include "Path.hpp"

namespace ln {

class FileInfo	// TODO: FileSystemInfo のほうがいい？
{
public:
	FileInfo(const ln::StringView& path);

	/** ファイルが最後に変更された日時を返します。 */
	DateTime lastModifiedTime();

private:
	ln::Path m_path;
};

} // namespace ln
