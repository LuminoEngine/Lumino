
#include "../Internal.h"

LN_NAMESPACE_BEGIN
namespace detail {

class MutexImpl
{
public:
	MutexImpl()
	{
		::InitializeCriticalSection(&m_criticalSection);
	}

	~MutexImpl()
	{
		::DeleteCriticalSection(&m_criticalSection);
	}

	void lock()
	{
		::EnterCriticalSection(&m_criticalSection);
	}

	void unlock()
	{
		::LeaveCriticalSection(&m_criticalSection);
	}

private:
	CRITICAL_SECTION m_criticalSection;
};

} // namespace detail
LN_NAMESPACE_END
