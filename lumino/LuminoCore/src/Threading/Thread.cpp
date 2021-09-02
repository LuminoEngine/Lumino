
#include "Internal.hpp"
#if defined(LN_OS_WIN32)
#include "Thread_Win32.hpp"
#else
#include "Thread_POSIX.hpp"
#endif
#include <LuminoCore/Threading/Thread.hpp>

namespace ln {

void Thread::sleep(int milliseconds)
{
	detail::ThreadImpl::sleep(milliseconds);
}

} // namespace ln
