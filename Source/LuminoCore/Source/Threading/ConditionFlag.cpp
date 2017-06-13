
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
void ConditionFlag::setTrue()
{
	m_impl->setTrue();
}

//------------------------------------------------------------------------------
void ConditionFlag::setFalse()
{
	m_impl->setFalse();
}

//------------------------------------------------------------------------------
bool ConditionFlag::isTrue() const
{
	return m_impl->isTrue();
}

//------------------------------------------------------------------------------
bool ConditionFlag::isFalse() const
{
	return !isTrue();
}

//------------------------------------------------------------------------------
void ConditionFlag::wait()
{
	m_impl->wait();
}

LN_NAMESPACE_END
