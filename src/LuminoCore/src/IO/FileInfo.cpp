
#include "Internal.hpp"
#include <LuminoCore/IO//FileSystem.hpp>
#include <LuminoCore/IO/FileInfo.hpp>

namespace ln {

//==============================================================================
// FileStream

FileInfo::FileInfo(const ln::StringRef& path)
	: m_path(path)
{
}

DateTime FileInfo::lastModifiedTime()
{
	return DateTime::fromEpochTime(detail::FileSystemInternal::getLastModifiedTime(m_path.c_str(), m_path.length()));
}

} // namespace ln
