#pragma once
#include "WebGL2Common.hpp"
#if defined(__EMSCRIPTEN__)
#include <emscripten/html5_webgl.h>
#else
// EGL/eglplatform.h は Windows で windows.h を取り込む。min/max マクロが
// std::min / std::max と衝突するため、先に NOMINMAX を立てておく。
#if defined(_WIN32) && !defined(NOMINMAX)
#define NOMINMAX
#endif
#include <EGL/egl.h>
#endif
#include <string>

namespace ln::rhi::webgl2 {

class WebGL2Device;

// ------ WebGL2Buffer ----------------------------------------------------------------------------------------------------------

class WebGL2Buffer final : public Buffer {
public:
    VoidResult init(WebGL2Device* device, const BufferDesc& desc);

    uint64_t size() const override { return m_size; }
    GLuint handle() const { return m_buffer; }
    /** このバッファをバインドするターゲット。生成時に決まり、以後変わらない。 */
    GLenum target() const { return m_target; }

protected:
    void finalize() override;

private:
    WebGL2Device* m_device = nullptr;
    GLuint m_buffer = 0;
    GLenum m_target = 0;
    uint64_t m_size = 0;
};

// ------ WebGL2Texture ---------------------------------------------------------------------------------------------------------

class WebGL2Texture final : public Texture {
public:
    VoidResult init(WebGL2Device* device, const TextureDesc& desc);

    uint32_t width() const override { return m_width; }
    uint32_t height() const override { return m_height; }
    TextureFormat format() const override { return m_format; }
    GLuint handle() const { return m_texture; }

protected:
    void finalize() override;

private:
    WebGL2Device* m_device = nullptr;
    GLuint m_texture = 0;
    TextureFormat m_format = TextureFormat::RGBA8Unorm;
    uint32_t m_width = 0;
    uint32_t m_height = 0;
};

// ------ WebGL2TextureView -----------------------------------------------------------------------------------------------------

// GL にはビューに相当するオブジェクトが無いため、テクスチャを参照するだけの薄いラッパーになる。
// テクスチャの寿命に巻き込まれないよう Ref で保持する。
class WebGL2TextureView final : public TextureView {
public:
    void init(WebGL2Texture* texture);

    uint32_t width() const override { return m_texture ? m_texture->width() : 0; }
    uint32_t height() const override { return m_texture ? m_texture->height() : 0; }
    TextureFormat format() const { return m_texture ? m_texture->format() : TextureFormat::Undefined; }
    GLuint textureHandle() const { return m_texture ? m_texture->handle() : 0; }

protected:
    void finalize() override;

private:
    Ref<WebGL2Texture> m_texture;
};

// ------ WebGL2Sampler ---------------------------------------------------------------------------------------------------------

class WebGL2Sampler final : public Sampler {
public:
    VoidResult init(WebGL2Device* device, const SamplerDesc& desc);
    GLuint handle() const { return m_sampler; }

protected:
    void finalize() override;

private:
    WebGL2Device* m_device = nullptr;
    GLuint m_sampler = 0;
};

// ------ WebGL2ShaderModule ----------------------------------------------------------------------------------------------------

/**
 * GLSL ES 300 のソースを保持する。
 *
 * ShaderModuleDesc にはステージの情報が無く、頂点なのかフラグメントなのかは
 * RenderPipeline の生成時に初めて確定する。そのためコンパイルはそこまで遅らせ、
 * 結果はステージごとにキャッシュして、同じシェーダから作られる複数のパイプライン
 * (ブレンドステート違いなど) で再コンパイルが起きないようにする。
 */
class WebGL2ShaderModule final : public ShaderModule {
public:
    VoidResult init(WebGL2Device* device, const ShaderModuleDesc& desc);

    /** stage は GL_VERTEX_SHADER か GL_FRAGMENT_SHADER。 */
    Result<GLuint> getOrCompile(GLenum stage);

    const std::string& debugName() const { return m_debugName; }
    WebGL2Device* device() const { return m_device; }

protected:
    void finalize() override;

private:
    WebGL2Device* m_device = nullptr;
    std::string m_source;
    std::string m_debugName;
    GLuint m_vertexShader = 0;
    GLuint m_fragmentShader = 0;
};

// ------ WebGL2Device ----------------------------------------------------------------------------------------------------------

class WebGL2Device final : public Device {
public:
    VoidResult init(const DeviceDesc& desc);

    DeviceLimits deviceLimits() const override { return m_limits; }
    Result<Ref<SwapChain>> createSwapChain(const SwapChainDesc& desc) override;
    Result<Ref<Buffer>> createBuffer(const BufferDesc& desc) override;
    Result<Ref<Texture>> createTexture(const TextureDesc& desc) override;
    Result<Ref<TextureView>> createTextureView(Texture* texture) override;
    Result<Ref<Sampler>> createSampler(const SamplerDesc& desc) override;
    Result<Ref<ShaderModule>> createShaderModule(const ShaderModuleDesc& desc) override;
    Result<Ref<PipelineLayout>> createPipelineLayout(const PipelineLayoutDesc& desc) override;
    Result<Ref<RenderPipeline>> createRenderPipeline(const RenderPipelineDesc& desc) override;
    VoidResult writeBuffer(Buffer* dst, uint64_t dstOffset, const void* data, uint64_t size) override;
    Result<std::vector<uint8_t>> readbackTexture(TextureView* view) override;
    void waitIdle() override;
    Backend backend() const override { return Backend::WebGL2; }

    /**
     * このデバイスの GL コンテキストが現在カレントかどうか。
     *
     * GL のオブジェクト名 (GLuint) はコンテキストごとに独立している。デバイスロストの
     * 復旧後、旧デバイスのリソースをそのまま glDelete* すると、同じ名前を持つ新しい
     * コンテキスト側のオブジェクトを消してしまう。各リソースは破棄の前にこれを確認し、
     * 自分のコンテキストがカレントでなければ何もしない (コンテキストごと破棄されるため
     * 漏れにはならない)。
     */
    bool isContextCurrent() const;

#if defined(__EMSCRIPTEN__)
    /** SwapChain がリサイズ時に canvas のサイズを合わせるために使う。 */
    const std::string& canvasSelector() const { return m_canvasSelector; }
#else
    // SwapChain がウィンドウサーフェスを作るために使う。
    EGLDisplay eglDisplay() const { return m_eglDisplay; }
    EGLConfig eglConfig() const { return m_eglConfig; }
    EGLContext eglContext() const { return m_eglContext; }
    /** ウィンドウサーフェスを破棄したあと、コンテキストを pbuffer 上へ戻す。 */
    void makeDefaultSurfaceCurrent();
#endif

protected:
    void finalize() override;

private:
    /** プラットフォーム固有の GL コンテキストを作り、カレントにする。 */
    VoidResult initContext(const DeviceDesc& desc);
    void finalizeContext();

#if defined(__EMSCRIPTEN__)
    EMSCRIPTEN_WEBGL_CONTEXT_HANDLE m_context = 0;
    std::string m_canvasSelector;
#else
    EGLDisplay m_eglDisplay = EGL_NO_DISPLAY;
    EGLConfig m_eglConfig = nullptr;
    EGLContext m_eglContext = EGL_NO_CONTEXT;
    /**
     * ウィンドウが無い状態でコンテキストをカレントにするための 1x1 サーフェス。
     * デバイスの生成直後に組み込みシェーダと既定テクスチャを作るため、
     * SwapChain (= ウィンドウサーフェス) を待たずにカレントにする必要がある。
     */
    EGLSurface m_eglPbuffer = EGL_NO_SURFACE;
#endif
    DeviceLimits m_limits;
    /** readbackTexture 用の使い回し FBO。 */
    GLuint m_readbackFbo = 0;
};

} // namespace ln::rhi::webgl2
