
#include "../Internal.h"
#include <Lumino/Threading/Thread.h>
#include <Lumino/Threading/Task.h>
#include <Lumino/Threading/TaskScheduler.h>

LN_NAMESPACE_BEGIN
namespace tr
{

//==============================================================================
// TaskScheduler
//==============================================================================

//------------------------------------------------------------------------------
TaskScheduler* TaskScheduler::getDefault()
{
	static TaskScheduler instance(4);
	return &instance;
}

//------------------------------------------------------------------------------
TaskScheduler::TaskScheduler(int threadCount)
	: m_semaphore(0, INT_MAX)
	, m_endRequested(false)
{
	m_threadList.reserve(threadCount);
	for (int i = 0; i < threadCount; ++i)
	{
		auto thr = LN_NEW DelegateThread();
		thr->start(createDelegate(this, &TaskScheduler::executeThread));
		m_threadList.add(thr);
	}
}

//------------------------------------------------------------------------------
TaskScheduler::~TaskScheduler()
{
	m_endRequested.setTrue();		// 終了要求を出して、
	
	for (int i = 0; i < m_threadList.getCount(); i++)	// スレッドの数だけセマフォ増やして全部起こして、
	{
		m_semaphore.unlock();
	}
	for (auto& thr : m_threadList)	// 全部終わるまで待つ
	{
		thr->wait();
		LN_SAFE_DELETE(thr);
	}
}

//------------------------------------------------------------------------------
int TaskScheduler::getMaxConcurrencyLevel() const
{
	return m_threadList.getCount();
}

//------------------------------------------------------------------------------
void TaskScheduler::queueTask(Task* task)
{
	if (LN_REQUIRE(task != nullptr)) return;

	MutexScopedLock lock(m_taskQueueLock);
	m_taskQueue.push_back(task);
	task->addRef();

	m_semaphore.unlock();	// キューに入れたので取り出したい人はどうぞ。
}

//------------------------------------------------------------------------------
void TaskScheduler::executeThread()
{
	while (true)
	{
		m_semaphore.lock();	// キューに何か追加されるまで待つ。または終了要求まで。

		// 終了要求がきていたらおしまい
		if (m_endRequested.isTrue()) {
			break;
		}

		// キューから1つ取り出す
		Task* task = nullptr;
		{
			MutexScopedLock lock(m_taskQueueLock);
			task = m_taskQueue.front();
			m_taskQueue.pop_front();
		}

		// 実行。状態変化は内部で行う
		task->execute();
		task->release();
	}
}

} // namespace tr
LN_NAMESPACE_END
