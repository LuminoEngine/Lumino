
#include "Internal.hpp"
#include <LuminoCore/Base/Environment.hpp>
#include <LuminoCore/IO/FileSystem.hpp>

#if defined(LN_OS_WIN32)
#include <Shlobj.h>
#include "Environment_Win32.hpp"
#elif defined(LN_OS_MAC)
#include "Environment_Mac.hpp"
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

Optional<String> Environment::getEnvironmentVariable(const StringRef& variableName)
{
	const char* value = ::getenv(variableName.toStdString().c_str());
	if (value)
		return String::fromCString(value);
	else
		return nullptr;
}

void Environment::setEnvironmentVariable(const StringRef& variableName, const StringRef& value)
{
	PlatformEnvironment::setEnvironmentVariable(variableName, value);
}

ByteOrder Environment::byteOrder()
{
	const unsigned short x = 0x0001;
	if ((*(unsigned char *)&x) != 0)
		return ByteOrder::LittleEndian;
	else
		return ByteOrder::BigEndian;
}

uint64_t Environment::getTickCount()
{
    return PlatformEnvironment::getTickCount();
}

} // namespace ln
