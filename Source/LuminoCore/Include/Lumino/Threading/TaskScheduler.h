
#pragma once
#include <deque>
#include "../Base/List.h"
#include "ConditionFlag.h"
#include "Mutex.h"
#include "Semaphore.h"

LN_NAMESPACE_BEGIN
class DelegateThread;
namespace tr {
class Task;

/**
	@brief	
*/
class TaskScheduler
{
public:
	static TaskScheduler* GetDefault();

	/** この TaskScheduler が同時に並列実行できる Task の数を取得します。*/
	int GetMaxConcurrencyLevel() const;

private:
	friend class Task;
	
	TaskScheduler(int threadCount);
	~TaskScheduler();
	
	void QueueTask(Task* task);
	void ExecuteThread();

	List<DelegateThread*>	m_threadList;
	std::deque<Task*>		m_taskQueue;
	Semaphore				m_semaphore;
	Mutex					m_taskQueueLock;
	ConditionFlag			m_endRequested;
};

} // namespace tr
LN_NAMESPACE_END
