
#include "../Internal.h"
#include <windows.h>

LN_NAMESPACE_BEGIN
namespace detail {

class SemaphoreImpl
{
public:
	SemaphoreImpl(int initialCount, int maxCount)
	{
		LN_CHECK_ARG(initialCount >= 0);
		LN_CHECK_ARG(maxCount >= 1);
		LN_CHECK_ARG(initialCount <= maxCount);
		m_handle = ::CreateSemaphore(NULL, initialCount, maxCount, NULL);
	}

	~SemaphoreImpl()
	{
		if (m_handle)
		{
			::CloseHandle(m_handle);
		}
	}

	void Lock()
	{
		::WaitForSingleObject(m_handle, INFINITE);// == WAIT_OBJECT_0);
	}

	void Unlock()
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
