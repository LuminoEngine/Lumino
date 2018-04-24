
#include "Internal.hpp"
#include <Lumino/Base/Environment.hpp>

#if defined(LN_OS_WIN32)
#include <Shlobj.h>
#include "Environment_Win32.hpp"
#elif defined(LN_OS_MAC)
#include <mach/mach_time.h>
#include <CoreServices/CoreServices.h>
#else
#include "Environment_Linux.hpp"
#endif

namespace ln {

String Environment::currentDirectory()
{
	PlatformEnvironment::StringType path;
	PlatformEnvironment::getCurrentDirectory(&path);
	return String::fromStdString(path);
}

String Environment::executablePath()
{
	return String::fromStdString(PlatformEnvironment::getExecutablePath());
}

CaseSensitivity Environment::pathCaseSensitivity()
{
#ifdef LN_OS_WIN32
	return CaseSensitivity::CaseInsensitive;
#else
	return CaseSensitivity::CaseSensitive;
#endif
}

String Environment::specialFolderPath(SpecialFolder specialFolder)
{
	PlatformEnvironment::StringType path;
	PlatformEnvironment::getSpecialFolderPath(specialFolder, &path);
	return String::fromStdString(path);
}

} // namespace ln
