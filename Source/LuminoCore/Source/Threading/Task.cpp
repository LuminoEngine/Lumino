
#include <atomic>
#include "../Internal.h"
#include <Lumino/Threading/Task.h>
#include <Lumino/Threading/TaskScheduler.h>
#include <Lumino/Threading/ThreadingExceptions.h>

LN_NAMESPACE_BEGIN
namespace tr
{

//==============================================================================
// Task
//==============================================================================

//------------------------------------------------------------------------------
TaskPtr Task::create(const Delegate<void()>& action)
{
	TaskPtr task(LN_NEW Task(action), false);
	return task;
}

//------------------------------------------------------------------------------
TaskPtr Task::Run(const Delegate<void()>& action)
{
	TaskPtr task(LN_NEW Task(action), false);
	task->start();
	return task;
}

//------------------------------------------------------------------------------
Task::Task(const Delegate<void()>& action)
	: m_action(action)
	, m_status(TaskStatus::Created)
	, m_exception(nullptr)
	, m_waiting(false)
{
}

//------------------------------------------------------------------------------
Task::~Task()
{
	LN_SAFE_DELETE(m_exception);
}

//------------------------------------------------------------------------------
void Task::start()
{
	LN_SAFE_DELETE(m_exception);
	m_waiting.SetFalse();
	TaskScheduler::getDefault()->QueueTask(this);
}

//------------------------------------------------------------------------------
void Task::Wait()
{
	m_waiting.Wait();
}

//------------------------------------------------------------------------------
TaskStatus Task::GetStatus() const
{
	return m_status;
}

//------------------------------------------------------------------------------
bool Task::IsCompleted() const
{
	// TODO: メモリフェンス張ったほうがいい？
	return m_status == TaskStatus::Completed;
}

//------------------------------------------------------------------------------
bool Task::IsFaulted() const
{
	return m_status == TaskStatus::Faulted;
}

//------------------------------------------------------------------------------
Exception* Task::GetException() const
{
	return m_exception;
}

//------------------------------------------------------------------------------
void Task::Execute()
{
	m_status = TaskStatus::Running;
	try
	{
		m_action.call();
		m_status = TaskStatus::Completed;
	}
	catch (Exception& e)
	{
		m_exception = e.copy();
		m_status = TaskStatus::Faulted;
	}
	catch (...)
	{
		m_exception = LN_NEW ThreadException();
		m_status = TaskStatus::Faulted;
	}
	m_waiting.SetTrue();
}

} // namespace tr
LN_NAMESPACE_END
