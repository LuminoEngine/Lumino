
#pragma once
#include <Lumino/Base/Array.h>
#include "EventArgs.h"

LN_NAMESPACE_BEGIN

/**
	@brief	
*/
class IEventListener
{
public:

	/**
		@brief	メッセージ処理コールバック
		@return true の場合、呼び出し側で他の処理を行わずに、すぐに処理を終了します。
	*/
	virtual bool OnEvent(const PlatformEventArgs& e) = 0;

protected:
	virtual ~IEventListener() {}
};

LN_NAMESPACE_END



