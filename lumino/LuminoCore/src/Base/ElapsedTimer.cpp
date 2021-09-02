
#include "Internal.hpp"
#include <LuminoCore/Base/ElapsedTimer.hpp>

#if defined(LN_OS_WIN32)
#include "ElapsedTimer_Win32.hpp"
#elif defined(LN_OS_MAC)
#include "ElapsedTimer_Mac.hpp"
#else
#include "ElapsedTimer_Linux.hpp"
#endif

namespace ln {

ElapsedTimer::ElapsedTimer(const char* tag) LN_NOEXCEPT
	: ElapsedTimer()
{
	m_tag = tag;
}

ElapsedTimer::~ElapsedTimer()
{
	if (m_tag) {
		std::cout << m_tag << ": " << elapsedMicroseconds() << " [usec]" << std::endl;
	}
}

uint64_t ElapsedTimer::elapsedMilliseconds() const LN_NOEXCEPT
{
	return elapsedNanoseconds() / 1000000;
}

uint64_t ElapsedTimer::elapsedMicroseconds() const LN_NOEXCEPT
{
	return elapsedNanoseconds() / 1000;
}

} // namespace ln
