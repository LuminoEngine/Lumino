
#include "../Internal.h"
#if defined(LN_OS_WIN32)
#include "Mutex_Win32.h"
#else
#include "Mutex_POSIX.h"
#endif
#include <Lumino/Threading/Mutex.h>

LN_NAMESPACE_BEGIN

//==============================================================================
// Mutex
//==============================================================================

//------------------------------------------------------------------------------
Mutex::Mutex()
{
	m_impl = LN_NEW detail::MutexImpl();
}

//------------------------------------------------------------------------------
Mutex::~Mutex()
{
	LN_SAFE_DELETE(m_impl);
}

//------------------------------------------------------------------------------
void Mutex::Lock()
{
	m_impl->Lock();
}

//------------------------------------------------------------------------------
void Mutex::Unlock()
{
	m_impl->Unlock();
}

LN_NAMESPACE_END
