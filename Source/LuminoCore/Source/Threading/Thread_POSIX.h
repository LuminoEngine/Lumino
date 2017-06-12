
#include "../Internal.h"
#include <Lumino/Threading/Thread.h>

LN_NAMESPACE_BEGIN
namespace detail {

class ThreadImpl
{
public:
	ThreadImpl(Thread* owner)
		: m_owner(owner)
		, mThread(0)
	{
	}

	~ThreadImpl()
	{
	}

	void start()
	{
		pthread_create(&mThread, NULL, ThreadEntry, this);
	}

	void Wait()
	{
		if (mThread)
		{
			// 待機
			pthread_join(mThread, NULL);
			mThread = 0;
		}
	}

	intptr_t GetThreadId() const
	{
		return (intptr_t)(mThread);
	}

	static void Sleep(int milliseconds)
	{
		usleep(milliseconds * 1000);
	}

	static intptr_t GetCurrentThreadId()
	{
		return (intptr_t)(pthread_self());
	}

private:
	static void* ThreadEntry(void* ptr)
	{
		ThreadImpl* obj = reinterpret_cast<ThreadImpl*>(ptr);
		obj->m_owner->ExecuteInternal();
		return 0;
	}

	Thread*			m_owner;
	pthread_t		mThread;
};

} // namespace detail
LN_NAMESPACE_END
