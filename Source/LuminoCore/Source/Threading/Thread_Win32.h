
#include "../Internal.h"
#include <windows.h>
#include <process.h>
#include <Lumino/Threading/Thread.h>

LN_NAMESPACE_BEGIN
namespace detail {

class ThreadImpl
{
public:
	ThreadImpl(Thread* owner)
		: m_owner(owner)
		, mThread(NULL)
		, mThreadID(0)
		//, mUseCreateThreadAPI(false)
	{
	}

	~ThreadImpl()
	{
	}

	void start()
	{
#if defined(_DLL)
		//if (mUseCreateThreadAPI)
		//{
			//mThread = (HANDLE)(::AfxBeginThread(_thr_prx::_execute_static, this));

			DWORD threadID;
			mThread = CreateThread(
				NULL, //セキュリティ属性
				0, //スタックサイズ
				ThreadEntry, //スレッド関数
				this, //スレッド関数に渡す引数
				0, //作成オプション(0またはCREATE_SUSPENDED)
				&threadID);//スレッドID
			mThreadID = threadID;

			// TODO :起動できなかった例外
			// TODO :既に起動している例外
		//}
		//else
		//{
#else
			mThread = reinterpret_cast< HANDLE >(_beginthreadex(NULL, 0, ThreadEntry, this, 0, &mThreadID));
		//}
#endif
	}

	void Wait()
	{
		if (mThread)
		{
			::WaitForSingleObject(mThread, INFINITE);
			::CloseHandle(mThread);
			mThread = NULL;
		}
		mThreadID = 0;
	}

	intptr_t GetThreadId() const
	{
		return mThreadID;
	}

	static void Sleep(int milliseconds)
	{
		::Sleep(milliseconds);
	}

	static intptr_t GetCurrentThreadId()
	{
		return ::GetCurrentThreadId();
	}

private:
#if defined(_DLL)
	static DWORD WINAPI ThreadEntry(LPVOID pThread);
#else
	static unsigned __stdcall ThreadEntry(void* pThread);
#endif

	Thread*			m_owner;
	HANDLE			mThread;
	unsigned int	mThreadID;
	//bool			mUseCreateThreadAPI;
};

//------------------------------------------------------------------------------
#if defined(_DLL)
DWORD WINAPI ThreadImpl::ThreadEntry(LPVOID pThread)
#else
unsigned __stdcall ThreadImpl::ThreadEntry(void* pThread)
#endif
{
	ThreadImpl* thr = static_cast<ThreadImpl*>(pThread);
	thr->m_owner->ExecuteInternal();
	return 0;
}

} // namespace detail
LN_NAMESPACE_END
