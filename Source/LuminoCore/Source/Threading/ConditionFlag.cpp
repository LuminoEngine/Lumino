
#include "../Internal.h"
#if defined(LN_OS_WIN32)
#include "ConditionFlag_Win32.h"
#else
#include "ConditionFlag_POSIX.h"
#endif
#include <Lumino/Threading/ConditionFlag.h>

LN_NAMESPACE_BEGIN

//==============================================================================
// ConditionFlag (Win32)
//==============================================================================
//------------------------------------------------------------------------------
ConditionFlag::ConditionFlag()
	: m_impl(LN_NEW detail::ConditionFlagImpl(false))
{
}

//------------------------------------------------------------------------------
ConditionFlag::ConditionFlag(bool initFlag)
	: m_impl(LN_NEW detail::ConditionFlagImpl(initFlag))
{
}

//------------------------------------------------------------------------------
ConditionFlag::~ConditionFlag()
{
	LN_SAFE_DELETE(m_impl);
}

//------------------------------------------------------------------------------
void ConditionFlag::SetTrue()
{
	m_impl->SetTrue();
}

//------------------------------------------------------------------------------
void ConditionFlag::SetFalse()
{
	m_impl->SetFalse();
}

//------------------------------------------------------------------------------
bool ConditionFlag::IsTrue() const
{
	return m_impl->IsTrue();
}

//------------------------------------------------------------------------------
bool ConditionFlag::IsFalse() const
{
	return !IsTrue();
}

//------------------------------------------------------------------------------
void ConditionFlag::Wait()
{
	m_impl->Wait();
}

LN_NAMESPACE_END
