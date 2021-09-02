#pragma once

#ifdef _WINDOWS_
//#ifdef LN_OS_WIN32
//#define NOMINMAX
//#include <Windows.h>
//#endif
#include "String.hpp"

namespace ln {
namespace detail
{

#ifdef LN_OS_WIN32

class Win32Helper
{
public:
	static String getWin32ErrorMessage(DWORD errorCode);
	static BOOL SYSTEMTIMEtoEpochTime(const SYSTEMTIME& sysTime, time_t* outTime);
	static BOOL FILETIMEtoEpochTime(const FILETIME& fileTime, time_t* outTime);
};

#define LN_ENSURE_HRESULT(hr)		(LN_ENSURE(SUCCEEDED(hr), ln::detail::Win32Helper::getWin32ErrorMessage(hr)))
#define LN_ENSURE_HRESULT2(exp)		(FAILED(hr = (exp)) && LN_ENSURE(SUCCEEDED(hr), ln::detail::Win32Helper::getWin32ErrorMessage(hr)))
#define LN_ENSURE_WIN32(expr, err)	(LN_ENSURE(expr, ln::detail::Win32Helper::getWin32ErrorMessage(err)))

#endif

} // namespace detail
} // namespace ln

#endif // #ifdef _WINDOWS_
