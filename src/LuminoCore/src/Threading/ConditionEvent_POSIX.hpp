
#include <pthread.h>

namespace ln {

class ConditionEvent::Impl
{
public:

	Impl(bool initFlag)
	{
		pthread_mutex_init(&m_mutex, NULL);
		pthread_cond_init(&m_wait, NULL);
		m_signal = initFlag;
	}

	~Impl()
	{
		pthread_cond_destroy(&m_wait);
		pthread_mutex_destroy(&m_mutex);
	}

	void lock()
	{
		// ミューテックスをロックする
		int r = pthread_mutex_lock(&m_mutex);
		if (r != 0)
		{
			errno = r;
			return;
		}

		m_signal = true;

		// 中断しているスレッドに通知を送る
		r = pthread_cond_broadcast(&m_wait);//pthread_cond_signal( &m_wait );
		if (r != 0)
		{
			errno = r;
			return;
		}

		// ミューテックスをアンロックする
		r = pthread_mutex_unlock(&m_mutex);
		if (r != 0)
		{
			errno = r;
			return;
		}
	}

	void unlock()
	{
		pthread_mutex_lock(&m_mutex);
		m_signal = false;
		pthread_mutex_unlock(&m_mutex);
	}

	bool isLocked() const
	{
		pthread_mutex_lock(&m_mutex);
		bool b = m_signal;
		pthread_mutex_unlock(&m_mutex);
		return b;
	}

	void wait()
	{
		// ミューテックスをロックする
		int r = pthread_mutex_lock(&m_mutex);
		if (r != 0) {
			errno = r;
			return;
		}

		// true ならアンロックして中断してみる
		if (m_signal)
		{
			pthread_mutex_unlock(&m_mutex);
			return;
		}

		// スレッドをスリープさせる
		r = pthread_cond_wait(&m_wait, &m_mutex);
		if (r != 0) {
			errno = r;
			return;
		}

		// アンロック
		pthread_mutex_unlock(&m_mutex);
		if (r != 0) {
			errno = r;
			return;
		}
	}

private:
	mutable pthread_mutex_t m_mutex;
	mutable pthread_cond_t m_wait;
	bool m_signal;
};

} // namespace ln
