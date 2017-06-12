
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
TaskScheduler* TaskScheduler::GetDefault()
{
	static TaskScheduler instance(4);
	return &instance;
}

//------------------------------------------------------------------------------
TaskScheduler::TaskScheduler(int threadCount)
	: m_semaphore(0, INT_MAX)
	, m_endRequested(false)
{
	m_threadList.Reserve(threadCount);
	for (int i = 0; i < threadCount; ++i)
	{
		auto thr = LN_NEW DelegateThread();
		thr->Start(CreateDelegate(this, &TaskScheduler::ExecuteThread));
		m_threadList.Add(thr);
	}
}

//------------------------------------------------------------------------------
TaskScheduler::~TaskScheduler()
{
	m_endRequested.SetTrue();		// 終了要求を出して、

	for (auto& thr : m_threadList)	// スレッドの数だけセマフォ増やして全部起こして、
	{
		m_semaphore.Unlock();
	}
	for (auto& thr : m_threadList)	// 全部終わるまで待つ
	{
		thr->Wait();
		LN_SAFE_DELETE(thr);
	}
}

//------------------------------------------------------------------------------
int TaskScheduler::GetMaxConcurrencyLevel() const
{
	return m_threadList.GetCount();
}

//------------------------------------------------------------------------------
void TaskScheduler::QueueTask(Task* task)
{
	if (LN_CHECK_ARG(task != nullptr)) return;

	MutexScopedLock lock(m_taskQueueLock);
	m_taskQueue.push_back(task);
	task->addRef();

	m_semaphore.Unlock();	// キューに入れたので取り出したい人はどうぞ。
}

//------------------------------------------------------------------------------
void TaskScheduler::ExecuteThread()
{
	while (true)
	{
		m_semaphore.Lock();	// キューに何か追加されるまで待つ。または終了要求まで。

		// 終了要求がきていたらおしまい
		if (m_endRequested.IsTrue()) {
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
		task->Execute();
		task->release();
	}
}

} // namespace tr
LN_NAMESPACE_END
