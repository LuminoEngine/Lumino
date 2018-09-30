#pragma once

#include <functional>
#include "Common.hpp"

namespace ln {

class UIEventArgs;

/**
	@brief		特定のイベントデータを持たない、UIイベントを処理するハンドラです。
	@param[in]	e		: イベントのデータ
*/
LN_DELEGATE()
using UIEventHandler = std::function<void(UIEventArgs* e)>;

} // namespace ln

