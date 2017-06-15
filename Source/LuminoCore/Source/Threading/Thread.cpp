
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
        wait();
    }
	LN_SAFE_DELETE(mLastException);
	LN_SAFE_DELETE(m_impl);
}

//------------------------------------------------------------------------------
void Thread::start()
{
	reset();
	m_impl->start();
}

//------------------------------------------------------------------------------
void Thread::wait()
{
	m_impl->wait();

	// スレッドで例外が発生していれば throw する
	if (mLastException != nullptr)
	{
		throw *mLastException;
	}
}

//------------------------------------------------------------------------------
bool Thread::isFinished()
{
	return mFinished.isTrue();
}

//------------------------------------------------------------------------------
intptr_t Thread::getThreadId() const
{ 
    return m_impl->getThreadId();
}

//------------------------------------------------------------------------------
void Thread::sleep(int milliseconds)
{
	detail::ThreadImpl::sleep(milliseconds);
}

//------------------------------------------------------------------------------
intptr_t Thread::getCurrentThreadId()
{
    return detail::ThreadImpl::getCurrentThreadId();
}

//------------------------------------------------------------------------------
void Thread::reset()
{
	// 前のが終了していない場合は待つ
	wait();
	LN_SAFE_DELETE(mLastException);
	mFinished.setFalse();
}

//------------------------------------------------------------------------------
void Thread::ExecuteInternal()
{
	try
	{
		execute();
	}
	catch (Exception& e)
	{
		mLastException = e.copy();
	}
	catch (...)
	{
		mLastException = LN_NEW ThreadException();
	}

	// 終了フラグを立てる
	mFinished.setTrue();
}

//==============================================================================
// DelegateThread
//==============================================================================

//------------------------------------------------------------------------------
#ifdef LN_CPP11
void DelegateThread::start(Delegate<void()> func)
{
	m_ThreadFunc = func;
	Thread::start();
}
#else
void DelegateThread::start(Delegate00 func)
{
	m_ThreadFunc = func;
	Thread::start();
}
#endif

//------------------------------------------------------------------------------
void DelegateThread::execute()
{
	if (!m_ThreadFunc.isEmpty()) {
		m_ThreadFunc.call();
	}
}

LN_NAMESPACE_END
