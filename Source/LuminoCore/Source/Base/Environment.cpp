
#include <time.h>
#include "../Internal.h"
#ifdef LN_OS_WIN32
    #include <Shlobj.h>
#elif defined(LN_OS_MAC)
    #include <mach/mach_time.h>
#include <CoreServices/CoreServices.h>
#endif
#include <Lumino/Base/Environment.h>

// linux
#include <Lumino/Base/StringHelper.h>
#include <Lumino/IO/FileSystem.h>

LN_NAMESPACE_BEGIN

//==============================================================================
// Environment
//==============================================================================

//------------------------------------------------------------------------------
String Environment::getEnvironmentVariable(const String& variableName)
{
	return LN_AFX_FUNCNAME(getEnvironmentVariable)(variableName);
}
String Environment::LN_AFX_FUNCNAME(getEnvironmentVariable)(const String& variableName)
{
	String val;
	bool r = tryGetEnvironmentVariable(variableName, &val);
	LN_THROW(r, KeyNotFoundException);
	return val;
}

//------------------------------------------------------------------------------
bool Environment::tryGetEnvironmentVariable(const String& variableName, String* outValue)
{
	String name = variableName;
#ifdef LN_OS_WIN32
	size_t len;
	errno_t err = _tgetenv_s(&len, NULL, 0, name.c_str());
	if (err != 0 || len == 0) {	// Win32 では環境変数を空にはできない
		return false;
	}
	Char* val = new Char[len];
	_tgetenv_s(&len, val, len, name.c_str());
	if (outValue) { *outValue = val; }
	return true;
#else
#endif
}

//------------------------------------------------------------------------------
ByteOrder Environment::getByteOrder()
{
	if (isLittleEndian()) {
		return ByteOrder::Little;
	}
	return ByteOrder::Big;
}

//------------------------------------------------------------------------------
bool Environment::isLittleEndian()
{
	const unsigned short x = 1;   // 0x0001
	return (*(unsigned char *)&x) != 0;
}

//------------------------------------------------------------------------------
uint64_t Environment::getTickCount()
{
#ifdef _WIN32
	// timeGetTime() は timeBeginPeriod() によって精度が変わるため、
	// GetTickCount() の方が無難かもしれない
	return ::GetTickCount();
#elif defined(LN_OS_MAC)
    return getTickCountNS() / 1000000;
    
#else
	struct timespec ts;
	if (clock_gettime(CLOCK_MONOTONIC, &ts) != 0) {
		return 0;
	}
	return ts.tv_nsec / 1000000;
#endif
}

//------------------------------------------------------------------------------
uint64_t Environment::getTickCountNS()
{
#ifdef _WIN32
	static LARGE_INTEGER freq = {};
	static bool initGetTickCount = false;

	// 初回呼び出し時に初期化
	if (!initGetTickCount)
	{
		::QueryPerformanceFrequency(&freq);
		initGetTickCount = true;
	}

	LARGE_INTEGER current;
	::QueryPerformanceCounter(&current);

	if (freq.QuadPart) {
		return (uint64_t)((current.QuadPart * 1000 * 1000 * 1000) / freq.QuadPart);
	}
    return 0;
    
#elif defined(LN_OS_MAC)
    static mach_timebase_info_data_t info = { 0, 0 };
    
    uint64_t cpuTime = mach_absolute_time();
    if (info.denom == 0) {
        mach_timebase_info(&info);
    }
    return cpuTime * info.numer / info.denom;
    
#else
	LN_THROW(0, NotImplementedException);
	return 0;
#endif
}

//------------------------------------------------------------------------------
template<>
const char* Environment::getNewLine<char>()
{
#ifdef LN_OS_WIN32
	return "\r\n";
#else
	return "\n";
#endif
}
template<>
const wchar_t* Environment::getNewLine<wchar_t>()
{
#ifdef LN_OS_WIN32
	return L"\r\n";
#else
	return L"\n";
#endif
}

//------------------------------------------------------------------------------
#if defined(LN_OS_WIN32)
template<>
void Environment::getSpecialFolderPath(SpecialFolder specialFolder, char* outPath)
{
	if (outPath == NULL) { return; }
	switch (specialFolder)
	{
	case SpecialFolder::ApplicationData:
		::SHGetSpecialFolderPathA(NULL, outPath, CSIDL_APPDATA, FALSE);
		break;
	case SpecialFolder::Temporary:
		::GetTempPathA(LN_MAX_PATH, outPath);
		break;
	}
}
template<>
void Environment::getSpecialFolderPath(SpecialFolder specialFolder, wchar_t* outPath)
{
	if (outPath == NULL) { return; }
	switch (specialFolder)
	{
	case SpecialFolder::ApplicationData:
		::SHGetSpecialFolderPathW(NULL, outPath, CSIDL_APPDATA, FALSE);
		break;
	case SpecialFolder::Temporary:
		::GetTempPathW(LN_MAX_PATH, outPath);
		break;
	}
}
#elif defined(LN_OS_LINUX)
template<typename TChar>
void Environment::getSpecialFolderPath(SpecialFolder specialFolder, TChar* outPath)
{
	if (outPath == NULL) { return; }

	switch (specialFolder)
	{
	case SpecialFolder::ApplicationData:
	{
		const TChar* pathes[] =
		{
			LN_T(TChar, "~/.local/share"),
			LN_T(TChar, "/usr/local/share"),
			LN_T(TChar, "/usr/share"),
		};
		for (int i = 0; i < LN_ARRAY_SIZE_OF(pathes); ++i) {
			if (FileSystem::ExistsDirectory(pathes[i])) {
				StringTraits::tstrcpy(outPath, LN_MAX_PATH, pathes[i]);
				return;
			}
		}
		break;
	}
	case SpecialFolder::Temporary:
		const TChar* pathes[] =
		{
			LN_T(TChar, "/tmp"),
		};
		for (int i = 0; i < LN_ARRAY_SIZE_OF(pathes); ++i) {
			if (FileSystem::ExistsDirectory(pathes[i])) {
				StringTraits::tstrcpy(outPath, LN_MAX_PATH, pathes[i]);
				return;
			}
		}
		break;
	}
	LN_THROW(0, InvalidOperationException);
}
template void Environment::getSpecialFolderPath(SpecialFolder specialFolder, char* outPath);
template void Environment::getSpecialFolderPath(SpecialFolder specialFolder, wchar_t* outPath);

#elif defined(LN_OS_MAC)

template<typename TChar>
void Environment::getSpecialFolderPath(SpecialFolder specialFolder, TChar* outPath)
{
	short domain = kOnAppropriateDisk;

	OSType type = kDesktopFolderType;
	switch (specialFolder)
	{
		case SpecialFolder::ApplicationData:
			type = kApplicationSupportFolderType;
			break;
		case SpecialFolder::Temporary:
			type = kTemporaryFolderType;
			break;
		default:
			LN_THROW(0, ArgumentException);
			break;
	}

	FSRef ref;
	if (FSFindFolder(domain, type, false, &ref) != 0) {
		LN_THROW(0, RuntimeException);
		return;
	}

	ByteBuffer buf(2048);
	if (FSRefMakePath(&ref, reinterpret_cast<UInt8 *>(buf.getData()), buf.getSize()) != noErr) {
		LN_THROW(0, RuntimeException);
		return;
	}

	GenericString<TChar> path;
	path.assignCStr((const char*)buf.getConstData(), buf.getSize());
	StringTraits::tstrcpy(outPath, LN_MAX_PATH, path.c_str());
}
template void Environment::getSpecialFolderPath(SpecialFolder specialFolder, char* outPath);
template void Environment::getSpecialFolderPath(SpecialFolder specialFolder, wchar_t* outPath);

#else
#endif


LN_NAMESPACE_END
