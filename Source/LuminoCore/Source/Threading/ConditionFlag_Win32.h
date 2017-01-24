
#include "../Internal.h"
#include <windows.h>

LN_NAMESPACE_BEGIN
namespace detail {

class ConditionFlagImpl
{
public:
	ConditionFlagImpl(bool initFlag)
		: mHandle(NULL)
	{
		BOOL t = (initFlag) ? TRUE : FALSE;
		mHandle = CreateEvent(NULL, TRUE, t, NULL);
	}

	~ConditionFlagImpl()
	{
		if (mHandle)
		{
			::CloseHandle(mHandle);
			mHandle = NULL;
		}
	}

	void SetTrue()
	{
		if (mHandle)
		{
			::SetEvent(mHandle);
		}
	}

	void SetFalse()
	{
		if (mHandle)
		{
			::ResetEvent(mHandle);
		}
	}

	bool IsTrue() const
	{
		if (mHandle)
		{
			return (::WaitForSingleObject(mHandle, 0) == WAIT_OBJECT_0);
		}
		return true;
	}

	void Wait()
	{
		if (mHandle)
		{
			::WaitForSingleObject(mHandle, INFINITE);
		}
	}

private:
	HANDLE			mHandle;
};

} // namespace detail
LN_NAMESPACE_END
