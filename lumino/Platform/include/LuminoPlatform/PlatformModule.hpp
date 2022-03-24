#pragma once
#include <LuminoEngine/Engine/Module.hpp>
#include "Common.hpp"

namespace ln {

struct PlatformModuleSettings {
    WindowSystem windowSystem;

    String windowTitle;                       // ウィンドウタイトル
    SizeI clientSize = SizeI(640, 480); // クライアント領域のピクセルサイズ
    //bool fullscreen = false;            // フルスクリーンモードで作成するかどうか
    bool resizable = true;              // 可変ウィンドウとして作成するかどうか

    intptr_t userWindow = 0;
    uint32_t win32IconResourceId = 0;
};

class PlatformModule : public Module
{
public:
	/**
	 * Initialize PlatformModule.
	 */
	static PlatformModule* initialize();

	/**
	 * Terminate PlatformModule.
	 */
	static void terminate();
};

} // namespace ln
