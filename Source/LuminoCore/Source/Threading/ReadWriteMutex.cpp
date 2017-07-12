
#include "../Internal.h"
#if defined(LN_OS_WIN32)
#include "ReadWriteMutex_Win32.h"
#else
#include "ReadWriteMutex_POSIX.h"
#endif
#include <Lumino/Threading/ReadWriteMutex.h>

LN_NAMESPACE_BEGIN

//==============================================================================
// ReadWriteMutex
//==============================================================================

//------------------------------------------------------------------------------
ReadWriteMutex::ReadWriteMutex()
{
	m_impl = LN_NEW detail::ReadWriteMutexImpl();
}

//------------------------------------------------------------------------------
ReadWriteMutex::~ReadWriteMutex()
{
	LN_SAFE_DELETE(m_impl);
}

//------------------------------------------------------------------------------
void ReadWriteMutex::lockRead()
{
	m_impl->lockRead();
}

//------------------------------------------------------------------------------
void ReadWriteMutex::unlockRead()
{
	m_impl->unlockRead();
}

//------------------------------------------------------------------------------
void ReadWriteMutex::lockWrite()
{
	m_impl->lockWrite();
}

//------------------------------------------------------------------------------
void ReadWriteMutex::unlockWrite()
{
	m_impl->unlockWrite();
}

LN_NAMESPACE_END
