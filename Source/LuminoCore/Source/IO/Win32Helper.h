#pragma once
#include <Lumino/Base/String.h>

LN_NAMESPACE_BEGIN
namespace detail
{

class Win32Helper
{
public:
	static String getWin32ErrorMessage(DWORD errorCode);
};

} // namespace detail
LN_NAMESPACE_END
