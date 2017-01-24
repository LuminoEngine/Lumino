
#pragma once

LN_NAMESPACE_BEGIN
namespace detail { class MutexImpl; }

/**
	@page	Doc_Threading
		- @ref Doc_Mutex_1

	@page	Doc_Mutex_1	単純な排他制御を行う

	@code
		Mutex	m_Mutex;
		・・・
		m_Mutex.Lock();
		// クリティカルセクション
		m_Mutex.Unlock();
	@endcode
*/

/**
	@brief	単純な排他制御を行うミューテックス
	@code
			Mutex	m_Mutex;
			・・・
			m_Mutex.Lock();
			// クリティカルセクション
			m_Mutex.Unlock();
	@endcode
 */
class LUMINO_EXPORT Mutex
{
public:
    Mutex();
    ~Mutex();
	
	/** ロック */
    void Lock();

	/** アンロック */
    void Unlock();

private:
	LN_DISALLOW_COPY_AND_ASSIGN(Mutex);
	detail::MutexImpl*	m_impl;
};

/**
	@brief	スコープ内でミューテックスをロックする
	@code
			Mutex	m_Mutex;
			・・・
			try {
				ScopedLock lock(m_Mutex);
				// クリティカルセクション内で return や throw しても

				// スコープ終端で Unlock される

				throw 0;
			}
			catch (){
			}
	@endcode
*/
class MutexScopedLock  
{
public:

    MutexScopedLock( Mutex& mutex )
        : m_Mutex( mutex )
    {
        m_Mutex.Lock();
    }

    ~MutexScopedLock()
    {
        m_Mutex.Unlock();
    }

private:

    Mutex& m_Mutex;
};

LN_NAMESPACE_END
