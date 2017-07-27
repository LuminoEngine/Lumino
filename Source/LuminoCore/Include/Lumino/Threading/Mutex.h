
#pragma once

LN_NAMESPACE_BEGIN
namespace detail { class MutexImpl; }

/**
	@brief	単純な排他制御を行うミューテックス
 */
class LUMINO_EXPORT Mutex
{
public:
    Mutex();
    ~Mutex();
	
	/** ロック */
    void lock();

	/** アンロック */
    void unlock();

private:
	LN_DISALLOW_COPY_AND_ASSIGN(Mutex);
	detail::MutexImpl*	m_impl;
};

/**
	@brief	スコープ内でミューテックスをロックする
*/
class MutexScopedLock 
{
public:

    MutexScopedLock( Mutex& mutex )
        : m_Mutex( mutex )
    {
        m_Mutex.lock();
    }

    ~MutexScopedLock()
    {
        m_Mutex.unlock();
    }

private:

    Mutex& m_Mutex;
};

LN_NAMESPACE_END
