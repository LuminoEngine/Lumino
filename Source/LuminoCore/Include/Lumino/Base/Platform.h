#pragma once
#ifdef LN_OS_WIN32
#include <Windows.h>
#endif
#include <Lumino/Base/String.h>

LN_NAMESPACE_BEGIN
namespace detail
{

#ifdef LN_OS_WIN32

class Win32Helper
{
public:
	static String getWin32ErrorMessage(DWORD errorCode);
};

#define LN_ENSURE_HRESULT(hr)		(LN_ENSURE(SUCCEEDED(hr), ln::detail::Win32Helper::getWin32ErrorMessage(hr)))
#define LN_ENSURE_WIN32(expr, err)	(LN_ENSURE(expr, ln::detail::Win32Helper::getWin32ErrorMessage(err)))

#endif

} // namespace detail
LN_NAMESPACE_END
