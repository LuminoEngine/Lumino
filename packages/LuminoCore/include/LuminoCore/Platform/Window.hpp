#pragma once

#include <LuminoBase/Types.hpp>
#include <LuminoBase/Result.hpp>
#include <LuminoBase/RefObject.hpp>
#include <LuminoCore/Object.hpp>
#include <functional>
#include <string>

namespace ln {
class GraphicsModule;
class GraphicsContext;
struct GraphicsContextDesc;
} // namespace ln

namespace ln::platform {

/** ウィンドウ作成の記述子。 */
struct WindowDesc {
    std::string title = "Lumino";
    uint32_t width = 1280;
    uint32_t height = 720;
    bool resizable = false;
    /**
     * Web 専用: 紐づける HTML canvas の CSS セレクタ (例: "#my_canvas")。
     * デスクトップビルドでは無視される。
     */
    std::string canvasSelector;
};

/** プラットフォーム固有のウィンドウハンドル。 */
struct NativeWindowHandle {
    void* glfwWindow = nullptr;
    /**
     * Web 専用: canvas の CSS セレクタ。デスクトップでは未使用。
     * Impl 側で保持している文字列へのポインタで、PlatformWindow の寿命内で有効。
     */
    const char* canvasSelector = nullptr;
};

/** プラットフォームのウィンドウの抽象化。 */
class PlatformWindow : public ln::Object {
public:
    ~PlatformWindow() override;

    PlatformWindow(const PlatformWindow&) = delete;
    PlatformWindow& operator=(const PlatformWindow&) = delete;

    /** グラフィックスコンテキストを紐づけたプラットフォームウィンドウを作成する。 */
    static Result<Ref<PlatformWindow>> create(GraphicsModule* module, const WindowDesc& desc, const GraphicsContextDesc& gfxDesc);

    /** イベントをポーリングし、ウィンドウを閉じるべきなら false を返す。 */
    bool processEvents();

    /** ネイティブウィンドウハンドルを取得する。 */
    NativeWindowHandle nativeHandle() const;

    /** フレームバッファのサイズをピクセル単位で取得する。 */
    void framebufferSize(uint32_t& width, uint32_t& height) const;

    /** このウィンドウに紐づいたグラフィックスコンテキストを取得する。nullptr の場合もある。 */
    GraphicsContext* graphicsContext() const;

private:
    PlatformWindow() = default;

    struct Impl;
    Impl* m_impl = nullptr;
};

} // namespace ln::platform
