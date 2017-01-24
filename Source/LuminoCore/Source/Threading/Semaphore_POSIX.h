
#include "../Internal.h"
#include <pthread.h>

LN_NAMESPACE_BEGIN
namespace detail {

class SemaphoreImpl
{
public:
	SemaphoreImpl(int initialCount, int maxCount)
		: m_value(initialCount)
		, m_maxCount(maxCount)
	{
		pthread_mutex_init(&m_mutex, 0);
		pthread_cond_init(&m_condition, 0);
	}

	~SemaphoreImpl()
	{
		pthread_cond_destroy(&m_condition);
		pthread_mutex_destroy(&m_mutex);
	}

	void Lock()
	{
		pthread_mutex_lock(&m_mutex);
		while (m_value < 1)
		{
			pthread_cond_wait(&m_condition, &m_mutex);
		}
		m_value -= 1;
		if (m_value < 0)
		{
			m_value = 0;
		}
		pthread_cond_broadcast(&m_condition);
		pthread_mutex_unlock(&m_mutex);
	}

	void Unlock()
	{
		pthread_mutex_lock(&m_mutex);
		m_value += 1;
		if (m_value > m_maxCount)
		{
			m_value = m_maxCount;
		}
		pthread_cond_broadcast(&m_condition);
		pthread_mutex_unlock(&m_mutex);
	}

private:
	int             m_value;
	int             m_maxCount;
	pthread_mutex_t m_mutex;
	pthread_cond_t  m_condition;
};

} // namespace detail
LN_NAMESPACE_END
