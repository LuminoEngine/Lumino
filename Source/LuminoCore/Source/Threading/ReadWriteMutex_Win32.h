
#include "../Internal.h"

LN_NAMESPACE_BEGIN
namespace detail {

class ReadWriteMutexImpl
{
public:
	ReadWriteMutexImpl()
		: mNoReaders(NULL)
		, mReaderCount(0)
	{
		::InitializeCriticalSection(&mReaderCountLock);
		::InitializeCriticalSection(&mWriterLock);

		mNoReaders = ::CreateEvent(NULL, TRUE, TRUE, NULL);
	}

	~ReadWriteMutexImpl()
	{
		if (mNoReaders)
		{
			::CloseHandle(mNoReaders);
			mNoReaders = NULL;
		}
		::DeleteCriticalSection(&mWriterLock);
		::DeleteCriticalSection(&mReaderCountLock);
	}

	void lockRead()
	{
		::EnterCriticalSection(&mWriterLock);
		::EnterCriticalSection(&mReaderCountLock);
		mReaderCount++;
		if (mReaderCount == 1)
		{
			// ReadLock スレッド数が 0 → 1 になった。
			// ひとつ以上存在していることになる。
			::ResetEvent(mNoReaders);
		}
		::LeaveCriticalSection(&mReaderCountLock);
		::LeaveCriticalSection(&mWriterLock);
	}

	void unlockRead()
	{
		::EnterCriticalSection(&mReaderCountLock);
		if (mReaderCount > 0)
		{
			mReaderCount--;
			if (mReaderCount == 0)
			{
				// ReadLock スレッド数が 1 → 0 になった。
				// ReadLock しているスレッドは存在していない。
				// もし WriteLock 待ちのスレッドがいれば、この時点で動き出す。
				::SetEvent(mNoReaders);
			}
		}
		::LeaveCriticalSection(&mReaderCountLock);
	}

	void lockWrite()
	{
		::EnterCriticalSection(&mWriterLock);
		if (mReaderCount > 0)
		{
			// もしひとつ以上 ReadLock されていれば、ここで待つ。
			::WaitForSingleObject(mNoReaders, INFINITE);
		}
	}

	void unlockWrite()
	{
		::LeaveCriticalSection(&mWriterLock);
	}

private:
	CRITICAL_SECTION    mReaderCountLock;
	CRITICAL_SECTION    mWriterLock;
	HANDLE              mNoReaders;		///< ReadLock() しているスレッドがひとつも無いか
	int                 mReaderCount;
};

} // namespace detail
LN_NAMESPACE_END
