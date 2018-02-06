
#include "../Internal.h"
#include <Lumino/Base/Platform.h>

LN_NAMESPACE_BEGIN
namespace detail
{

String Win32Helper::getWin32ErrorMessage(DWORD errorCode)
{
	wchar_t buf[512];
	::FormatMessageW(
		FORMAT_MESSAGE_FROM_SYSTEM, NULL, errorCode,
		0, buf, sizeof(buf) / sizeof(buf[0]), NULL);
	return String::fromCString(buf);
}

} // namespace detail
LN_NAMESPACE_END
