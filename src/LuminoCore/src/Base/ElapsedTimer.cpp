
#include "Internal.hpp"
#include <Lumino/Base/ElapsedTimer.hpp>

#if defined(LN_OS_WIN32)
#include "ElapsedTimer_Win32.hpp"
#elif defined(LN_OS_MAC)
#include "ElapsedTimer_Mac.hpp"
#else
#include "ElapsedTimer_Linux.hpp"
#endif

namespace ln {

uint64_t ElapsedTimer::elapsedMilliseconds() const LN_NOEXCEPT
{
	return elapsedNanoseconds() / 1000000;
}

uint64_t ElapsedTimer::elapsedMicroseconds() const LN_NOEXCEPT
{
	return elapsedNanoseconds() / 1000;
}

} // namespace ln
