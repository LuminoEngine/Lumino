#pragma once
#include "WebGL2Common.hpp"
#include <emscripten/html5_webgl.h>
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

protected:
    void finalize() override;

private:
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

    /** SwapChain がリサイズ時に canvas のサイズを合わせるために使う。 */
    const std::string& canvasSelector() const { return m_canvasSelector; }

protected:
    void finalize() override;

private:
    EMSCRIPTEN_WEBGL_CONTEXT_HANDLE m_context = 0;
    std::string m_canvasSelector;
    DeviceLimits m_limits;
    /** readbackTexture 用の使い回し FBO。 */
    GLuint m_readbackFbo = 0;
};

} // namespace ln::rhi::webgl2
