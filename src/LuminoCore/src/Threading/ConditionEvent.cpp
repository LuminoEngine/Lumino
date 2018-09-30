
#include "Internal.hpp"
#include <LuminoCore/Threading/ConditionEvent.hpp>
#if defined(LN_OS_WIN32)
#include "ConditionEvent_Win32.hpp"
#else
#include "ConditionEvent_POSIX.hpp"
#endif

namespace ln {

//==============================================================================
// ConditionEvent

ConditionEvent::ConditionEvent()
	: m_impl(LN_NEW Impl(false))
{
}

ConditionEvent::ConditionEvent(bool initFlag)
	: m_impl(LN_NEW Impl(initFlag))
{
}

ConditionEvent::~ConditionEvent()
{
	LN_SAFE_DELETE(m_impl);
}

void ConditionEvent::lock()
{
	m_impl->lock();
}

void ConditionEvent::unlock()
{
	m_impl->unlock();
}

bool ConditionEvent::isLocked() const
{
	return m_impl->isLocked();
}

void ConditionEvent::wait()
{
	m_impl->wait();
}

} // namespace ln
