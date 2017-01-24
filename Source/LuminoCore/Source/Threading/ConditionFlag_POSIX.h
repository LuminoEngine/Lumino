
#include "../Internal.h"
#include <pthread.h>

LN_NAMESPACE_BEGIN
namespace detail {

class ConditionFlagImpl
{
public:

	ConditionFlagImpl(bool initFlag)
	{
		pthread_mutex_init(&mMutex, NULL);
		pthread_cond_init(&mWait, NULL);
		mSignal = initFlag;
	}

	~ConditionFlagImpl()
	{
		pthread_cond_destroy(&mWait);
		pthread_mutex_destroy(&mMutex);
	}

	void SetTrue()
	{
		// ミューテックスをロックする
		int r = pthread_mutex_lock(&mMutex);
		if (r != 0)
		{
			errno = r;
			return;
		}

		mSignal = true;

		// 中断しているスレッドに通知を送る
		r = pthread_cond_broadcast(&mWait);//pthread_cond_signal( &mWait );
		if (r != 0)
		{
			errno = r;
			return;
		}

		// ミューテックスをアンロックする
		r = pthread_mutex_unlock(&mMutex);
		if (r != 0)
		{
			errno = r;
			return;
		}
	}

	void SetFalse()
	{
		pthread_mutex_lock(&mMutex);
		mSignal = false;
		pthread_mutex_unlock(&mMutex);
	}

	bool IsTrue() const
	{
		pthread_mutex_lock(&mMutex);
		bool b = mSignal;
		pthread_mutex_unlock(&mMutex);
		return b;
	}

	void Wait()
	{
		// ミューテックスをロックする
		int r = pthread_mutex_lock(&mMutex);
		if (r != 0) {
			errno = r;
			return;
		}

		// true ならアンロックして中断してみる
		if (mSignal)
		{
			pthread_mutex_unlock(&mMutex);
			return;
		}

		// スレッドをスリープさせる
		r = pthread_cond_wait(&mWait, &mMutex);
		if (r != 0) {
			errno = r;
			return;
		}

		// アンロック
		pthread_mutex_unlock(&mMutex);
		if (r != 0) {
			errno = r;
			return;
		}
	}

private:
	mutable pthread_mutex_t	mMutex;
	mutable pthread_cond_t	mWait;
	bool					mSignal;
};

} // namespace detail
LN_NAMESPACE_END
