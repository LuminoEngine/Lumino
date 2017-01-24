
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
void ReadWriteMutex::LockRead()
{
	m_impl->LockRead();
}

//------------------------------------------------------------------------------
void ReadWriteMutex::UnlockRead()
{
	m_impl->UnlockRead();
}

//------------------------------------------------------------------------------
void ReadWriteMutex::LockWrite()
{
	m_impl->LockWrite();
}

//------------------------------------------------------------------------------
void ReadWriteMutex::UnlockWrite()
{
	m_impl->UnlockWrite();
}

LN_NAMESPACE_END
