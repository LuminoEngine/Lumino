
#pragma once

#include <Lumino/Base/Array.h>
#include "EventArgs.h"

namespace Lumino
{
namespace Platform
{

/**
	@brief	プラットフォーム固有のウィンドウシステムのウィンドウを抽象化します。
*/
class IEventListener
{
public:

	/**
		@brief	メッセージ処理コールバック
		@return true の場合、呼び出し側で他の処理を行わずに、すぐに処理を終了します。
	*/
	virtual bool OnEvent(const EventArgs& e) = 0;

protected:
	virtual ~IEventListener() {}
};

} // namespace Platform
} // namespace Lumino



