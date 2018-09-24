
#pragma once
#include "PlatformEvent.hpp"

namespace ln {

/** プラットフォーム固有の機能にアクセスします。 */
class PlatformSupport
{
public:
	//static void showAlertMessageBox(const Char* message) LN_NOEXCEPT;

	/** WindowsAPI の機能にアクセスするためのウィンドウハンドルを取得します。取得できない場合は nullptr を返します。 */
	static void* getWin32WindowHandle(detail::PlatformWindow* window);	// TODO: remove detail
};

} // namespace ln
