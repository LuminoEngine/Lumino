#pragma once

namespace ln {
class PlatformWindow;

enum class WindowSystem {
    Native,
    GLFW,
    GLFWWithoutOpenGL,
    External,
};

struct WindowCreationSettings {
    String title; // ウィンドウタイトル
    int clientWidth;
    int clientHeight;
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
