#pragma once

namespace ln {
class PlatformWindow;

enum class WindowSystem {
    Native,
    GLFWWithOpenGL,
    GLFWWithoutOpenGL,
    External,
};

struct WindowCreationSettings {
    String title; // ウィンドウタイトル
    int clientWidth = 0;
    int clientHeight = 0;
    // SizeI		clientSize = SizeI(640, 480);	// クライアント領域のピクセルサイズ
    bool fullscreen = false; // フルスクリーンモードで作成するかどうか
    bool resizable = true;   // 可変ウィンドウとして作成するかどうか

    intptr_t userWindow = 0;
    uint32_t win32IconResourceId = 0;
};

namespace detail {
class OpenGLContext;
class PlatformManager;
class PlatformWindowManager;


} // namespace detail
} // namespace ln
