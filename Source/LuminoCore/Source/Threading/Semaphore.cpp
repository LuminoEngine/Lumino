
#include "../Internal.h"
#if defined(LN_OS_WIN32)
#include "Semaphore_Win32.h"
#else
#include "Semaphore_POSIX.h"
#endif
#include <Lumino/Threading/Semaphore.h>

LN_NAMESPACE_BEGIN

//==============================================================================
// Semaphore
//==============================================================================
//------------------------------------------------------------------------------
Semaphore::Semaphore(int initialCount, int maxCount)
{
	m_impl = LN_NEW detail::SemaphoreImpl(initialCount, maxCount);
}

//------------------------------------------------------------------------------
Semaphore::~Semaphore()
{
	LN_SAFE_DELETE(m_impl);
}

//------------------------------------------------------------------------------
void Semaphore::lock()
{
	m_impl->lock();
}

//------------------------------------------------------------------------------
void Semaphore::unlock()
{
	m_impl->unlock();
}

LN_NAMESPACE_END
