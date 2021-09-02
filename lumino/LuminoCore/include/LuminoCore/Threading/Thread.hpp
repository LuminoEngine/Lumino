
#pragma once
#include "Common.hpp"

namespace ln {

/** */
class LN_API Thread
{
public:
	/** 
	 *	指定した時間だけ現在のスレッドをスリープ
	 *	@param	milliseconds	: 待機時間 (ミリ秒)
	 */
	static void sleep(int milliseconds);
};

} // namespace ln
