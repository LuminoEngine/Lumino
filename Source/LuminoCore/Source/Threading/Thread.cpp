
#include "../Internal.h"
#if defined(LN_OS_WIN32)
#include "Thread_Win32.h"
#else
#include "Thread_POSIX.h"
#endif
#include <Lumino/Threading/Thread.h>
#include <Lumino/Threading/ThreadingExceptions.h>

LN_NAMESPACE_BEGIN

//==============================================================================
// Thread
//==============================================================================

//------------------------------------------------------------------------------
Thread::Thread()
	: m_impl(LN_NEW detail::ThreadImpl(this))
	, mFinished(true)
	, mLastException(nullptr)
{
}

//------------------------------------------------------------------------------
Thread::~Thread()
{
    if (m_impl != nullptr)
    {
        Wait();
    }
	LN_SAFE_DELETE(mLastException);
	LN_SAFE_DELETE(m_impl);
}

//------------------------------------------------------------------------------
void Thread::Start()
{
	Reset();
	m_impl->Start();
}

//------------------------------------------------------------------------------
void Thread::Wait()
{
	m_impl->Wait();

	// スレッドで例外が発生していれば throw する
	if (mLastException != nullptr)
	{
		throw *mLastException;
	}
}

//------------------------------------------------------------------------------
bool Thread::IsFinished()
{
	return mFinished.IsTrue();
}

//------------------------------------------------------------------------------
intptr_t Thread::GetThreadId() const
{ 
    return m_impl->GetThreadId();
}

//------------------------------------------------------------------------------
void Thread::Sleep(int milliseconds)
{
	detail::ThreadImpl::Sleep(milliseconds);
}

//------------------------------------------------------------------------------
intptr_t Thread::GetCurrentThreadId()
{
    return detail::ThreadImpl::GetCurrentThreadId();
}

//------------------------------------------------------------------------------
void Thread::Reset()
{
	// 前のが終了していない場合は待つ
	Wait();
	LN_SAFE_DELETE(mLastException);
	mFinished.SetFalse();
}

//------------------------------------------------------------------------------
void Thread::ExecuteInternal()
{
	try
	{
		Execute();
	}
	catch (Exception& e)
	{
		mLastException = e.Copy();
	}
	catch (...)
	{
		mLastException = LN_NEW ThreadException();
	}

	// 終了フラグを立てる
	mFinished.SetTrue();
}

//==============================================================================
// DelegateThread
//==============================================================================

//------------------------------------------------------------------------------
#ifdef LN_CPP11
void DelegateThread::Start(Delegate<void()> func)
{
	m_ThreadFunc = func;
	Thread::Start();
}
#else
void DelegateThread::Start(Delegate00 func)
{
	m_ThreadFunc = func;
	Thread::Start();
}
#endif

//------------------------------------------------------------------------------
void DelegateThread::Execute()
{
	if (!m_ThreadFunc.IsEmpty()) {
		m_ThreadFunc.Call();
	}
}

LN_NAMESPACE_END
