
#include "../Internal.h"
#include <pthread.h>

LN_NAMESPACE_BEGIN
namespace detail {

class ReadWriteMutexImpl
{
public:
	ReadWriteMutexImpl()
		: mReaders(0)
		, mWriters(0)
		, mReadWaiters(0)
		, mWriteWaiters(0)
	{
		pthread_mutex_init(&mLock, NULL);
		pthread_cond_init(&mRead, NULL);
		pthread_cond_init(&mWrite, NULL);
	}

	~ReadWriteMutexImpl()
	{
		pthread_mutex_destroy(&mLock);
		pthread_cond_destroy(&mRead);
		pthread_cond_destroy(&mWrite);
	}

	void LockRead()
	{
		pthread_mutex_lock(&mLock);
		if (mWriters || mWriteWaiters)
		{
			++mReadWaiters;
			do {
				pthread_cond_wait(&mRead, &mLock);
			} while (mWriters || mWriteWaiters);
			--mReadWaiters;
		}
		++mReaders;
		pthread_mutex_unlock(&mLock);
	}

	void UnlockRead()
	{
		pthread_mutex_lock(&mLock);
		--mReaders;
		if (mWriteWaiters) {
			pthread_cond_signal(&mWrite);
		}
		pthread_mutex_unlock(&mLock);
	}

	void LockWrite()
	{
		pthread_mutex_lock(&mLock);
		if (mReaders || mWriters)
		{
			++mWriteWaiters;
			do {
				pthread_cond_wait(&mWrite, &mLock);
			} while (mReaders || mWriters);
			--mWriteWaiters;
		}
		mWriters = 1;
		pthread_mutex_unlock(&mLock);
	}

	void UnlockWrite()
	{
		pthread_mutex_lock(&mLock);
		mWriters = 0;
		if (mWriteWaiters)
		{
			pthread_cond_signal(&mWrite);
		}
		else if (mReadWaiters)
		{
			pthread_cond_broadcast(&mRead);
		}
		pthread_mutex_unlock(&mLock);
	}

private:
	pthread_mutex_t     mLock;
	pthread_cond_t      mRead;
	pthread_cond_t      mWrite;
	unsigned            mReaders;
	unsigned            mWriters;
	unsigned            mReadWaiters;
	unsigned            mWriteWaiters;
};

} // namespace detail
LN_NAMESPACE_END
