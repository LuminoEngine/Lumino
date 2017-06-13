
#include "../Internal.h"

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

	void setTrue()
	{
		if (mHandle)
		{
			::SetEvent(mHandle);
		}
	}

	void setFalse()
	{
		if (mHandle)
		{
			::ResetEvent(mHandle);
		}
	}

	bool isTrue() const
	{
		if (mHandle)
		{
			return (::WaitForSingleObject(mHandle, 0) == WAIT_OBJECT_0);
		}
		return true;
	}

	void wait()
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
