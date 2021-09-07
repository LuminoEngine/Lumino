
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
	detail::GenericStaticallyLocalPath<CChar> localPath(m_path.c_str(), m_path.length());
	return DateTime::fromEpochTime(detail::FileSystemInternal::getLastModifiedTime(localPath.c_str(), localPath.length()));
}

} // namespace ln
