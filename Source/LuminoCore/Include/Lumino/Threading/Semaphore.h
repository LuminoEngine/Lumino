
#pragma once

LN_NAMESPACE_BEGIN
namespace detail { class SemaphoreImpl; }

/**
	@brief	
*/
class Semaphore
{
public:

	/**
		@brief		エントリ数の初期値と最大値を指定してセマフォを作成します。
		@params[in]	initialCount	: (初期状態で解放済みの数)
		@params[in]	maxCount		: 最大エントリ数
	*/
	Semaphore(int initialCount, int maxCount);

	~Semaphore();

	void lock();

	void unlock();

private:
	LN_DISALLOW_COPY_AND_ASSIGN(Semaphore);
	detail::SemaphoreImpl*	m_impl;
};

LN_NAMESPACE_END
