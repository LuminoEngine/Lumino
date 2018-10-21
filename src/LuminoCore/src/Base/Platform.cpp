
#include "Internal.hpp"
#include <LuminoCore/Base/Platform.hpp>

namespace ln {
namespace detail {

#ifdef LN_OS_WIN32
String Win32Helper::getWin32ErrorMessage(DWORD errorCode)
{
	wchar_t buf[512];
	::FormatMessageW(
		FORMAT_MESSAGE_FROM_SYSTEM, NULL, errorCode,
		0, buf, sizeof(buf) / sizeof(buf[0]), NULL);
	return String::fromCString(buf);
}

BOOL Win32Helper::SYSTEMTIMEtoEpochTime(const SYSTEMTIME& sysTime, time_t* outTime)
{
	if (sysTime.wYear < 1900)
	{
		*outTime = 0;
		return TRUE;
	}
	else
	{
		struct tm atm;
		atm.tm_sec = sysTime.wSecond;
		atm.tm_min = sysTime.wMinute;
		atm.tm_hour = sysTime.wHour;
		atm.tm_mday = sysTime.wDay;
		atm.tm_mon = sysTime.wMonth - 1;        // tm_mon is 0 based
		atm.tm_year = sysTime.wYear - 1900;     // tm_year is 1900 based
		atm.tm_isdst = 0;
		*outTime = mktime(&atm);
		return TRUE;
	}
}

BOOL Win32Helper::FILETIMEtoEpochTime(const FILETIME& fileTime, time_t* outTime)
{
	SYSTEMTIME sysTime;
	SYSTEMTIME localTime;
	if (!FileTimeToSystemTime(&fileTime, &sysTime) || !SystemTimeToTzSpecificLocalTime(nullptr, &sysTime, &localTime))
	{
		*outTime = 0;
		return FALSE;
	}

	return SYSTEMTIMEtoEpochTime(localTime, outTime);
}
#endif

} // namespace detail
} // namespace ln
