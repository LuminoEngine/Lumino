#include <LuminoBase/Logger.hpp>
#include "WebGL2Device.hpp"
#include "WebGL2Pipeline.hpp"
#include "WebGL2SwapChain.hpp"
#include <vector>

namespace ln::rhi::webgl2 {

namespace {

/** Emscripten の既定 canvas。DeviceDesc::canvasSelector が空のときに使う。 */
const char* kDefaultCanvasSelector = "#canvas";

/**
 * BufferUsage から、このバッファを常にバインドするターゲットを選ぶ。
 *
 * WebGL2 は GL と違い、一度 ELEMENT_ARRAY_BUFFER にバインドしたバッファを他のターゲットへ
 * バインドすることを禁じている。そのため生成時にターゲットを決め、writeBuffer も含めて
 * ずっと同じターゲットを使う。
 */
GLenum pickBufferTarget(BufferUsage usage) {
    if (usage & BufferUsage::Index) return GL_ELEMENT_ARRAY_BUFFER;
    if (usage & BufferUsage::Uniform) return GL_UNIFORM_BUFFER;
    return GL_ARRAY_BUFFER;
}

} // namespace

// ------ WebGL2Buffer ----------------------------------------------------------------------------------------------------------

VoidResult WebGL2Buffer::init(WebGL2Device* /*device*/, const BufferDesc& desc) {
    m_size = desc.size;
    glGenBuffers(1, &m_buffer);
    if (!m_buffer) {
        return LN_MAKE_ERROR("glGenBuffers failed.");
    }
    m_target = pickBufferTarget(desc.usage);
    glBindBuffer(m_target, m_buffer);
    // 内容は writeBuffer で毎フレーム書き換わりうるため DYNAMIC_DRAW とする。
    glBufferData(m_target, static_cast<GLsizeiptr>(desc.size), desc.initialData, GL_DYNAMIC_DRAW);
    glBindBuffer(m_target, 0);
    return LN_MAKE_SUCCESS();
}

void WebGL2Buffer::finalize() {
    if (m_buffer) {
        glDeleteBuffers(1, &m_buffer);
        m_buffer = 0;
    }
    Buffer::finalize();
}

// ------ WebGL2Texture ---------------------------------------------------------------------------------------------------------

VoidResult WebGL2Texture::init(WebGL2Device* /*device*/, const TextureDesc& desc) {
    const GLFormatInfo fmt = toGLFormat(desc.format);
    if (!fmt.supported) {
        return LN_MAKE_ERROR("Unsupported TextureFormat on WebGL2: %d", static_cast<int>(desc.format));
    }
    m_width = desc.width;
    m_height = desc.height;
    m_format = desc.format;

    glGenTextures(1, &m_texture);
    if (!m_texture) {
        return LN_MAKE_ERROR("glGenTextures failed.");
    }
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTexStorage2D(GL_TEXTURE_2D, static_cast<GLsizei>(desc.mipLevels), fmt.internalFormat,
                   static_cast<GLsizei>(desc.width), static_cast<GLsizei>(desc.height));

    // サンプラーオブジェクトが結び付かない状態でも「完全なテクスチャ」になるようにする。
    // 既定の縮小フィルタ (NEAREST_MIPMAP_LINEAR) はミップを持たないテクスチャを不完全にし、
    // サンプリング結果が黒くなるため。
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    if (desc.initialData) {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0,
                        static_cast<GLsizei>(desc.width), static_cast<GLsizei>(desc.height),
                        fmt.format, fmt.type, desc.initialData);
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    return LN_MAKE_SUCCESS();
}

void WebGL2Texture::finalize() {
    if (m_texture) {
        glDeleteTextures(1, &m_texture);
        m_texture = 0;
    }
    Texture::finalize();
}

// ------ WebGL2TextureView -----------------------------------------------------------------------------------------------------

void WebGL2TextureView::init(WebGL2Texture* texture) {
    m_texture = Ref<WebGL2Texture>::retain(texture);
}

void WebGL2TextureView::finalize() {
    m_texture = nullptr;
    TextureView::finalize();
}

// ------ WebGL2Sampler ---------------------------------------------------------------------------------------------------------

VoidResult WebGL2Sampler::init(WebGL2Device* /*device*/, const SamplerDesc& desc) {
    glGenSamplers(1, &m_sampler);
    if (!m_sampler) {
        return LN_MAKE_ERROR("glGenSamplers failed.");
    }
    // ミップマップの生成口が RHI に無いため、縮小フィルタもミップを使わない指定にする。
    glSamplerParameteri(m_sampler, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(toGLFilter(desc.minFilter)));
    glSamplerParameteri(m_sampler, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(toGLFilter(desc.magFilter)));
    glSamplerParameteri(m_sampler, GL_TEXTURE_WRAP_S, static_cast<GLint>(toGLWrap(desc.addressU)));
    glSamplerParameteri(m_sampler, GL_TEXTURE_WRAP_T, static_cast<GLint>(toGLWrap(desc.addressV)));
    glSamplerParameteri(m_sampler, GL_TEXTURE_WRAP_R, static_cast<GLint>(toGLWrap(desc.addressW)));
    return LN_MAKE_SUCCESS();
}

void WebGL2Sampler::finalize() {
    if (m_sampler) {
        glDeleteSamplers(1, &m_sampler);
        m_sampler = 0;
    }
    Sampler::finalize();
}

// ------ WebGL2ShaderModule ----------------------------------------------------------------------------------------------------

VoidResult WebGL2ShaderModule::init(WebGL2Device* /*device*/, const ShaderModuleDesc& desc) {
    if (desc.format != ShaderCodeFormat::GLSL) {
        return LN_MAKE_ERROR("WebGL2 backend requires ShaderCodeFormat::GLSL. (%s)", desc.debugName.c_str());
    }
    if (!desc.code || desc.codeSizeBytes == 0) {
        return LN_MAKE_ERROR("Empty shader code. (%s)", desc.debugName.c_str());
    }
    m_source.assign(static_cast<const char*>(desc.code), desc.codeSizeBytes);
    m_debugName = desc.debugName;
    return LN_MAKE_SUCCESS();
}

Result<GLuint> WebGL2ShaderModule::getOrCompile(GLenum stage) {
    GLuint& slot = (stage == GL_VERTEX_SHADER) ? m_vertexShader : m_fragmentShader;
    if (slot) return slot;

    GLuint shader = glCreateShader(stage);
    if (!shader) {
        return LN_MAKE_ERROR("glCreateShader failed. (%s)", m_debugName.c_str());
    }
    const char* src = m_source.c_str();
    const GLint len = static_cast<GLint>(m_source.size());
    glShaderSource(shader, 1, &src, &len);
    glCompileShader(shader);

    GLint status = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE) {
        GLint logLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
        std::string log(static_cast<size_t>(logLength > 0 ? logLength : 1), '\0');
        glGetShaderInfoLog(shader, logLength, nullptr, log.data());
        glDeleteShader(shader);
        return LN_MAKE_ERROR("Failed to compile GLSL ES shader. (%s) %s", m_debugName.c_str(), log.c_str());
    }

    slot = shader;
    return slot;
}

void WebGL2ShaderModule::finalize() {
    if (m_vertexShader) {
        glDeleteShader(m_vertexShader);
        m_vertexShader = 0;
    }
    if (m_fragmentShader) {
        glDeleteShader(m_fragmentShader);
        m_fragmentShader = 0;
    }
    ShaderModule::finalize();
}

// ------ WebGL2Device ----------------------------------------------------------------------------------------------------------

VoidResult WebGL2Device::init(const DeviceDesc& desc) {
    m_canvasSelector = desc.canvasSelector.empty() ? kDefaultCanvasSelector : desc.canvasSelector;

    EmscriptenWebGLContextAttributes attrs;
    emscripten_webgl_init_context_attributes(&attrs);
    attrs.majorVersion = 2; // WebGL 2.0 (OpenGL ES 3.0)
    attrs.minorVersion = 0;
    // 描画は常にオフスクリーン FBO へ行い、present で既定のフレームバッファへ blit する
    // (WebGL2 の FBO 0 には自前のデプスをアタッチできないため)。
    // したがって既定のフレームバッファにデプス / ステンシル / MSAA は要らない。
    attrs.alpha = EM_FALSE;
    attrs.depth = EM_FALSE;
    attrs.stencil = EM_FALSE;
    attrs.antialias = EM_FALSE;
    attrs.preserveDrawingBuffer = EM_FALSE;
    attrs.failIfMajorPerformanceCaveat = EM_FALSE;

    m_context = emscripten_webgl_create_context(m_canvasSelector.c_str(), &attrs);
    if (m_context <= 0) {
        // 最も多い原因はセレクタの指し先が無いこと。WebGL のコンテキストは canvas に
        // 結び付くため、GraphicsContext ではなく初期化設定で描画先を渡す必要がある。
        return LN_MAKE_ERROR(
            "Failed to create a WebGL2 context for canvas '%s'. "
            "Check that the element exists, and that the same selector is passed to "
            "LNInstanceInitializeSettings::canvasSelector (Runtime.initialize({ canvasSelector })). "
            "The WebGL2 backend binds the device to one canvas at initialization time.",
            m_canvasSelector.c_str());
    }
    if (emscripten_webgl_make_context_current(m_context) != EMSCRIPTEN_RESULT_SUCCESS) {
        return LN_MAKE_ERROR("emscripten_webgl_make_context_current failed.");
    }

    GLint alignment = 256;
    GLint maxBlockSize = 16384;
    glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &alignment);
    glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &maxBlockSize);
    m_limits.minUniformBufferOffsetAlignment = static_cast<uint32_t>(alignment > 0 ? alignment : 256);
    m_limits.maxUniformBufferRange = static_cast<uint32_t>(maxBlockSize > 0 ? maxBlockSize : 16384);

    LN_LOG_INFO("[WebGL2] Context created. (canvas: %s, uboAlign: %d, maxUboRange: %d)",
                m_canvasSelector.c_str(), alignment, maxBlockSize);
    return LN_MAKE_SUCCESS();
}

Result<Ref<SwapChain>> WebGL2Device::createSwapChain(const SwapChainDesc& desc) {
    auto sc = Ref<WebGL2SwapChain>::adopt(new WebGL2SwapChain());
    auto r = sc->init(this, desc);
    if (!r) return LN_FORWARD_ERROR(r);
    return Ref<SwapChain>(sc);
}

Result<Ref<Buffer>> WebGL2Device::createBuffer(const BufferDesc& desc) {
    auto buf = Ref<WebGL2Buffer>::adopt(new WebGL2Buffer());
    auto r = buf->init(this, desc);
    if (!r) return LN_FORWARD_ERROR(r);
    return Ref<Buffer>(buf);
}

Result<Ref<Texture>> WebGL2Device::createTexture(const TextureDesc& desc) {
    auto tex = Ref<WebGL2Texture>::adopt(new WebGL2Texture());
    auto r = tex->init(this, desc);
    if (!r) return LN_FORWARD_ERROR(r);
    return Ref<Texture>(tex);
}

Result<Ref<TextureView>> WebGL2Device::createTextureView(Texture* texture) {
    if (!texture) {
        return LN_MAKE_ERROR("createTextureView: texture is null.");
    }
    auto view = Ref<WebGL2TextureView>::adopt(new WebGL2TextureView());
    view->init(static_cast<WebGL2Texture*>(texture));
    return Ref<TextureView>(view);
}

Result<Ref<Sampler>> WebGL2Device::createSampler(const SamplerDesc& desc) {
    auto s = Ref<WebGL2Sampler>::adopt(new WebGL2Sampler());
    auto r = s->init(this, desc);
    if (!r) return LN_FORWARD_ERROR(r);
    return Ref<Sampler>(s);
}

Result<Ref<ShaderModule>> WebGL2Device::createShaderModule(const ShaderModuleDesc& desc) {
    auto sm = Ref<WebGL2ShaderModule>::adopt(new WebGL2ShaderModule());
    auto r = sm->init(this, desc);
    if (!r) return LN_FORWARD_ERROR(r);
    return Ref<ShaderModule>(sm);
}

Result<Ref<PipelineLayout>> WebGL2Device::createPipelineLayout(const PipelineLayoutDesc& desc) {
    auto pl = Ref<WebGL2PipelineLayout>::adopt(new WebGL2PipelineLayout());
    pl->init(desc);
    return Ref<PipelineLayout>(pl);
}

Result<Ref<RenderPipeline>> WebGL2Device::createRenderPipeline(const RenderPipelineDesc& desc) {
    auto rp = Ref<WebGL2RenderPipeline>::adopt(new WebGL2RenderPipeline());
    auto r = rp->init(desc);
    if (!r) return LN_FORWARD_ERROR(r);
    return Ref<RenderPipeline>(rp);
}

VoidResult WebGL2Device::writeBuffer(Buffer* dst, uint64_t dstOffset, const void* data, uint64_t size) {
    // 描画コマンドは CommandBuffer に記録され submit() まで発行されないため、
    // ここで即時に書き込んでも Rhi.hpp の順序契約 (同一フレームの draw は最後の内容を読む) を満たす。
    auto* buf = static_cast<WebGL2Buffer*>(dst);
    glBindBuffer(buf->target(), buf->handle());
    glBufferSubData(buf->target(), static_cast<GLintptr>(dstOffset),
                    static_cast<GLsizeiptr>(size), data);
    glBindBuffer(buf->target(), 0);
    return LN_MAKE_SUCCESS();
}

Result<std::vector<uint8_t>> WebGL2Device::readbackTexture(TextureView* view) {
    auto* glView = static_cast<WebGL2TextureView*>(view);
    if (!glView || glView->textureHandle() == 0) {
        return LN_MAKE_ERROR("Invalid TextureView for readback.");
    }
    const GLFormatInfo fmt = toGLFormat(glView->format());
    if (!fmt.supported) {
        return LN_MAKE_ERROR("Unsupported format for readback.");
    }

    const uint32_t width = glView->width();
    const uint32_t height = glView->height();

    if (!m_readbackFbo) {
        glGenFramebuffers(1, &m_readbackFbo);
    }
    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_readbackFbo);
    glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_2D, glView->textureHandle(), 0);
    const GLenum status = glCheckFramebufferStatus(GL_READ_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
        return LN_MAKE_ERROR("Readback framebuffer is incomplete. (0x%04x)", static_cast<unsigned>(status));
    }

    std::vector<uint8_t> pixels(static_cast<size_t>(width) * height * fmt.bytesPerPixel);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glReadPixels(0, 0, static_cast<GLsizei>(width), static_cast<GLsizei>(height),
                 fmt.format, fmt.type, pixels.data());
    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

    // 行の並べ替えは不要。頂点シェーダで gl_Position.y を反転しているため
    // (SPIRV-Cross の flip_vert_y)、レンダーターゲットの行 0 は画像の最上段であり、
    // glReadPixels が行 0 から返す順序がそのまま Rhi.hpp の「上から下」の契約と一致する。
    return pixels;
}

void WebGL2Device::waitIdle() {
    glFinish();
}

void WebGL2Device::finalize() {
    if (m_readbackFbo) {
        glDeleteFramebuffers(1, &m_readbackFbo);
        m_readbackFbo = 0;
    }
    if (m_context > 0) {
        emscripten_webgl_destroy_context(m_context);
        m_context = 0;
    }
    Device::finalize();
}

} // namespace ln::rhi::webgl2
