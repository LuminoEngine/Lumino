#pragma once
// Ref<> のデストラクタを実体化するため、保持する型の定義まで取り込む。
#include "WebGL2CommandBuffer.hpp"
#include "WebGL2Device.hpp"
#include <string>

namespace ln::rhi::webgl2 {

/**
 * オフスクリーン FBO をバックバッファとするスワップチェーン。
 *
 * WebGL2 の既定のフレームバッファ (FBO 0) には自前のデプステクスチャをアタッチできないため、
 * カラーは通常のテクスチャとして確保し、present() で既定のフレームバッファへ blit する。
 * blit では上下を反転する。頂点シェーダで gl_Position.y を反転している関係で
 * レンダーターゲットの行 0 が画像の最上段になっており、そのまま出すと画面が上下逆になるため。
 *
 * デスクトップ (ANGLE) では既定のフレームバッファは EGL のウィンドウサーフェスで、
 * blit のあと eglSwapBuffers で表示する。ウィンドウを渡されなかった場合は
 * デバイスの pbuffer に描いたままにする (オフスクリーン専用)。
 */
class WebGL2SwapChain final : public SwapChain {
public:
    VoidResult init(WebGL2Device* device, const SwapChainDesc& desc);

    TextureView* acquireNextTexture() override;
    void present() override;
    uint32_t width() const override { return m_width; }
    uint32_t height() const override { return m_height; }
    TextureFormat format() const override { return TextureFormat::RGBA8Unorm; }
    VoidResult resize(uint32_t width, uint32_t height) override;
    // GL では CPU と GPU の同期はドライバに任せるため、フレームを多重化しない。
    uint32_t maxFramesInFlight() const override { return 1; }
    uint32_t currentFrame() const override { return 0; }
    CommandBuffer* getCurrentCommandBuffer() override;

protected:
    void finalize() override;

private:
    VoidResult recreateBackbuffer(uint32_t width, uint32_t height);

    // ---- プラットフォーム固有の既定のフレームバッファ ----
    /** 描画先 (canvas / EGL ウィンドウサーフェス) を用意する。 */
    VoidResult initSurface(const SwapChainDesc& desc);
    void resizeDefaultFramebuffer(uint32_t width, uint32_t height);
    /** blit 後に画面へ出す。ブラウザでは何もしない。 */
    void swapBuffers();
    void finalizeSurface();

    WebGL2Device* m_device = nullptr;
#if defined(__EMSCRIPTEN__)
    std::string m_canvasSelector;
#else
    /** ウィンドウの既定のフレームバッファ。ウィンドウが無ければ EGL_NO_SURFACE。 */
    EGLSurface m_eglSurface = EGL_NO_SURFACE;
#endif
    uint32_t m_width = 0;
    uint32_t m_height = 0;
    Ref<WebGL2Texture> m_colorTexture;
    Ref<WebGL2TextureView> m_colorView;
    Ref<WebGL2CommandBuffer> m_commandBuffer;
    /** present の blit 元となる読み込み用フレームバッファ。 */
    GLuint m_presentFbo = 0;
};

} // namespace ln::rhi::webgl2
