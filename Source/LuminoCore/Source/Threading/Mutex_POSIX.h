
#include "../Internal.h"
#include <pthread.h>

LN_NAMESPACE_BEGIN
namespace detail {

class MutexImpl
{
public:
	MutexImpl()
	{
		pthread_mutex_init(&m_mutex, NULL);
	}

	~MutexImpl()
	{
		pthread_mutex_destroy(&m_mutex);
	}

	void Lock()
	{
		pthread_mutex_lock(&m_mutex);
	}

	void Unlock()
	{
		pthread_mutex_unlock(&m_mutex);
	}

private:
	pthread_mutex_t m_mutex;
};

} // namespace detail
LN_NAMESPACE_END
