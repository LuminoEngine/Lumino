
#include "Internal.hpp"
#include <Lumino/Base/Platform.hpp>

namespace ln {
namespace detail
{

#ifdef LN_OS_WIN32
String Win32Helper::getWin32ErrorMessage(DWORD errorCode)
{
	wchar_t buf[512];
	::FormatMessageW(
		FORMAT_MESSAGE_FROM_SYSTEM, NULL, errorCode,
		0, buf, sizeof(buf) / sizeof(buf[0]), NULL);
	return String::fromCString(buf);
}
#endif

} // namespace detail
} // namespace ln
