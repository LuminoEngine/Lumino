
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


namespace detail {
void Cocoa_getScreenMousePosition(int* x, int* y);
void Cocoa_pointFromScreen(void* windowId, int screenPointX, int screenPointY, int* clientPointX, int* clientPointY);
void Cocoa_pointToScreen(void* windowId, int clientPointX, int clientPointY, int* screenPointX, int* screenPointY);
bool checkGraphicsSupport();
} // namespace detail
} // namespace ln
