
#include "../Internal.h"

LN_NAMESPACE_BEGIN
namespace detail {

class SemaphoreImpl
{
public:
	SemaphoreImpl(int initialCount, int maxCount)
	{
		if (LN_REQUIRE(initialCount >= 0)) return;
		if (LN_REQUIRE(maxCount >= 1)) return;
		if (LN_REQUIRE(initialCount <= maxCount)) return;
		m_handle = ::CreateSemaphore(NULL, initialCount, maxCount, NULL);
	}

	~SemaphoreImpl()
	{
		if (m_handle)
		{
			::CloseHandle(m_handle);
		}
	}

	void lock()
	{
		::WaitForSingleObject(m_handle, INFINITE);// == WAIT_OBJECT_0);
	}

	void unlock()
	{
		LONG prev;
		::ReleaseSemaphore(m_handle, 1, &prev);
	}

private:
	HANDLE			m_handle;
	int				m_maxCount;
};

} // namespace detail
LN_NAMESPACE_END
