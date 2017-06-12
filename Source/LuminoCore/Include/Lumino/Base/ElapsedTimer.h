
#pragma once
#include "Common.h"

LN_NAMESPACE_BEGIN
	
/**
	@brief	経過時間を測定するためのクラスです。
*/
class ElapsedTimer
{
public:
	ElapsedTimer();
	~ElapsedTimer();

	/** 計測開始 */
	void start();

	/** 経過時間の取得 (ns) */
	uint64_t getElapsed() const LN_NOEXCEPT;

private:
	LN_DISALLOW_COPY_AND_ASSIGN(ElapsedTimer);

	uint64_t		m_start;
#if defined(LN_OS_WIN32)
	uint64_t		m_freq;
#endif
};

LN_NAMESPACE_END
