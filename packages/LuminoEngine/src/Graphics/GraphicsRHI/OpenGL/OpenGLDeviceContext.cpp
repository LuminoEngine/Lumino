#include <LuminoEngine/Platform/detail/OpenGLContext.hpp>
#include <LuminoEngine/Platform/detail/PlatformManager.hpp>
#include <LuminoEngine/Graphics/GraphicsRHI/RHIHelper.hpp>
#include "OpenGLDeviceContext.hpp"
#include "GLVertexBuffer.hpp"
#include "GLIndexBuffer.hpp"
#include "GLUniformBuffer.hpp"
#include "GLTextures.hpp"
#include "GLDepthBuffer.hpp"
#include "GLShaderPass.hpp"
#include "GLRenderPass.hpp"
#include "GLDescriptorPool.hpp"
#include "GLCommandList.hpp"
#include "GLFWSwapChain.hpp"
#include "GLStateCache.hpp"

#ifndef GL_TEXTURE_MAX_ANISOTROPY_EXT
#define GL_TEXTURE_MAX_ANISOTROPY_EXT 0x84FE
#endif // GL_TEXTURE_MAX_ANISOTROPY_EXT

#ifndef GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT
#define GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT 0x84FF
#endif // GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT

#ifdef LN_GRAPHICS_OPENGLES
#define glClearDepth glClearDepthf
#endif

namespace ln {
namespace detail {

// 外部の OpenGL Context に統合するときに使う
// Result<> IGraphicsDevice::getOpenGLCurrentFramebufferTextureId(int* id)
//{
//	GLint type;
//	GL_CHECK(glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE, &type));
//	if (LN_ENSURE(type == GL_TEXTURE)) return false;
//
//	GLint texture;
//	GL_CHECK(glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME, &texture));
//	*id = texture;
//	return true;
//}

//==============================================================================
// OpenGLDevice

OpenGLDevice::OpenGLDevice()
    : m_mainWindow(nullptr)
    //, m_mainGLContext(nullptr)
    , m_uniformTempBuffer()
    , m_uniformTempBufferWriter(&m_uniformTempBuffer)
    , m_es(false) {
}

Result<> OpenGLDevice::init(const Settings& settings) {
    LN_LOG_DEBUG("OpenGLDeviceContext::init start");

    // Create main context
    {
        m_mainWindow = settings.mainWindow;
        if (settings.platformManager) {
            auto mainGLContext = settings.platformManager->openGLContext();
            if (mainGLContext) {
                mainGLContext->makeCurrentMain();
            }
        }
        //m_mainGLContext->makeCurrentMain();
        //		if (settings.mainWindow)
        //		{
        //#ifdef LN_GLFW
        //			auto glfwContext = makeRef<GLFWContext>();
        //			glfwContext->init(this, settings.mainWindow);
        //			m_glContext = glfwContext;
        //#endif
        //		}
        //
        //		if (!m_glContext)
        //		{
        //			// Android(GLSurfaceView) や Web など、バックバッファの swap を Lumino の外側で行う場合のダミー
        //			auto glfwContext = makeRef<EmptyGLContext>();
        //			m_glContext = glfwContext;
        //		}
    }


#if defined(LN_GRAPHICS_OPENGLES)
    LN_LOG_INFO("OpenGL ES enabled.");
#endif

#if defined(LN_GRAPHICS_OPENGLES)
    m_es = true;
#else
    int result = gladLoadGL();
    if (LN_ENSURE(result, "Failed gladLoadGL()")) {
        // OpenGL Context がアクティブになっていないと失敗する。
        return err();
    }

    // GRAD は ES の検出を行っているが、例えば ES 3.2 をそのまま GL 3.2 とみなしてしまう。
    // glClearDepthf は ES 3.0 と GL 4.1 で使うことができるが、ランタイムは GL 4.1 とみなされ
    // GRAD は glClearDepthf をロードしなくなってしまう。
    // そのため、ここで明示的にロードする。
    m_es = OpenGLHelper::checkOpenGLES();
    if (m_es) {
        GLADloadproc load = reinterpret_cast<GLADloadproc>(&glfwGetProcAddress);
        // load_GL_VERSION_3_3
        glad_glVertexAttribDivisor = (PFNGLVERTEXATTRIBDIVISORPROC)load("glVertexAttribDivisor");
        glad_glGenSamplers = (PFNGLGENSAMPLERSPROC)load("glGenSamplers");
        glad_glDeleteSamplers = (PFNGLDELETESAMPLERSPROC)load("glDeleteSamplers");
        glad_glBindSampler = (PFNGLBINDSAMPLERPROC)load("glBindSampler");
        glad_glSamplerParameteri = (PFNGLSAMPLERPARAMETERIPROC)load("glSamplerParameteri");
        glad_glSamplerParameterf = (PFNGLSAMPLERPARAMETERFPROC)load("glSamplerParameterf");
        // load_GL_VERSION_4_1
        glad_glClearDepthf = (PFNGLCLEARDEPTHFPROC)load("glClearDepthf");
    }

    LN_LOG_INFO("OpenGL {}.{}", GLVersion.major, GLVersion.minor);
#endif

    // Check caps.
    {
        LN_LOG_INFO("GL_VERSION : {}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));

        GL_CHECK(glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &m_caps.MAX_VERTEX_ATTRIBS));
        GL_CHECK(glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &m_caps.MAX_COLOR_ATTACHMENTS));
        LN_LOG_INFO("GL_MAX_VERTEX_ATTRIBS : {}", m_caps.MAX_VERTEX_ATTRIBS);
        LN_LOG_INFO("GL_MAX_COLOR_ATTACHMENTS : {}", m_caps.MAX_COLOR_ATTACHMENTS);

        int extensions = 0;
        glGetIntegerv(GL_EXTENSIONS, &extensions);
        LN_LOG_INFO("GL_EXTENSIONS : {}", extensions);
        for (int i = 0; i < extensions; i++) {
            LN_LOG_INFO("  {}", reinterpret_cast<const char*> (glGetStringi(GL_EXTENSIONS, i)));
        }
        while (glGetError() != 0) {
            // GL_EXTENSIONS が無い場合は GL_INVALID_ENUM が発生するため、エラー情報が残らないように消費する。
        }

        const char* extensionsString = (const char*)glGetString(GL_EXTENSIONS);
        while (glGetError() != 0) {
            // GL_EXTENSIONS が無い場合は GL_INVALID_ENUM が発生するため、エラー情報が残らないように消費する。
        }
        if (extensionsString) {
            std::string str = extensionsString;

            m_caps.support_filter_anisotropic =
                (str.find("EXT_texture_filter_anisotropic") != std::string::npos) |
                (str.find("WEBKIT_EXT_texture_filter_anisotropic") != std::string::npos) |
                (str.find("MOZ_EXT_texture_filter_anisotropic") != std::string::npos);
            if (m_caps.support_filter_anisotropic) {
                GL_CHECK(glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &m_caps.MAX_TEXTURE_MAX_ANISOTROPY_EXT));
            }
        }
    }

    const size_t PageSize = 0x200000; // 2MB
    m_uniformBufferAllocatorPageManager = makeRef<GLUniformBufferAllocatorPageManager>(this, PageSize);
    m_descriptorObjectPoolManager = makeRef<GLDescriptorObjectPoolManager>(this);

    // m_graphicsContext = makeRef<GLGraphicsContext>();
    // m_graphicsContext->init(this);

    LN_LOG_DEBUG("OpenGLDeviceContext::init end");
    return ok();
}

void OpenGLDevice::dispose() {
    IGraphicsDevice::dispose();
}

// ICommandList* OpenGLDevice::getGraphicsContext() const
//{
//	return m_graphicsContext;
// }

void OpenGLDevice::onGetDeviceProperties(GraphicsDeviceProperties* outCaps) {
    outCaps->graphicsAPI = LN_GRAPHICS_BACKEND_OPENGL;
    //#ifdef LN_GRAPHICS_OPENGLES
    outCaps->requestedShaderTriple.target = "glsl";
    outCaps->requestedShaderTriple.version = 300;
    outCaps->requestedShaderTriple.option = "es";
//#else
//    outCaps->requestedShaderTriple.target = "glsl";
//    outCaps->requestedShaderTriple.version = 400;
//    outCaps->requestedShaderTriple.option = "";
//#endif

    if (m_es) {
        // canvas.getContext("webgl2") で取得したコンテキストでないと、
        // gl.getParameter(gl.UNIFORM_BUFFER_OFFSET_ALIGNMENT) は使えない。
        // ひとまず、 "OpenGL ES 2.0 (WebGL 2.0 (OpenGL ES 3.0 Chromium))" 上での値が 256 だったのでこれを使う。
        outCaps->uniformBufferOffsetAlignment = 256;

        // WebGL 2.0 で動かしたら、同じシェーダコードのはずだが上下反転していたので。
        outCaps->imageLayoytVFlip = false;
    }
    else {
        GLint align = 0;
        GL_CHECK(glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &align));
        outCaps->uniformBufferOffsetAlignment = align;
        outCaps->imageLayoytVFlip = true;
    }
}

Ref<ISwapChain> OpenGLDevice::onCreateSwapChain(PlatformWindow* window, const SizeI& backbufferSize) {
#ifdef LN_GLFW
    auto ptr = makeRef<GLFWSwapChain>(this);
    if (!ptr->init(window, backbufferSize)) {
        return nullptr;
    }
    return ptr;
#else
    return nullptr;
#endif
}

Ref<ICommandList> OpenGLDevice::onCreateCommandList() {
    // if (LN_REQUIRE(!m_commandListCreated)) return nullptr;	// OpenGL では複数 CommandList の作成を禁止する

    auto ptr = makeRef<GLGraphicsContext>();
    if (!ptr->init(this)) {
        return nullptr;
    }

    // m_commandListCreated = true;
    return ptr;
}

Ref<IRenderPass> OpenGLDevice::onCreateRenderPass(const DeviceFramebufferState& buffers, ClearFlags clearFlags, const Color& clearColor, float clearDepth, uint8_t clearStencil) {
    auto ptr = makeRef<GLRenderPass>();
    if (!ptr->init(this, buffers, clearFlags, clearColor, clearDepth, clearStencil)) {
        return nullptr;
    }
    return ptr;
}

Ref<IPipeline> OpenGLDevice::onCreatePipeline(const DevicePipelineStateDesc& state) {
    auto ptr = makeRef<GLPipeline>();
    if (!ptr->init(this, state)) {
        return nullptr;
    }
    return ptr;
}

Ref<IVertexDeclaration> OpenGLDevice::onCreateVertexDeclaration(const VertexElement* elements, int elementsCount) {
    auto ptr = makeRef<GLVertexDeclaration>();
    ptr->init(elements, elementsCount);
    return ptr;
}

Ref<RHIResource> OpenGLDevice::onCreateVertexBuffer(GraphicsResourceUsage usage, size_t bufferSize, const void* initialData) {
    auto ptr = makeRef<GLVertexBuffer>();
    ptr->init(usage, bufferSize, initialData);
    return ptr;
}

Ref<RHIResource> OpenGLDevice::onCreateIndexBuffer(GraphicsResourceUsage usage, IndexBufferFormat format, int indexCount, const void* initialData) {
    auto ptr = makeRef<GLIndexBuffer>();
    if (!ptr->init(usage, format, indexCount, initialData)) {
        return nullptr;
    }
    return ptr;
}

Ref<RHIResource> OpenGLDevice::onCreateTexture2D(GraphicsResourceUsage usage, uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap, const void* initialData) {
    auto ptr = makeRef<GLTexture2D>();
    ptr->init(usage, width, height, requestFormat, mipmap, initialData);
    return ptr;
}

Ref<RHIResource> OpenGLDevice::onCreateTexture3D(GraphicsResourceUsage usage, uint32_t width, uint32_t height, uint32_t depth, TextureFormat requestFormat, bool mipmap, const void* initialData) {
    auto ptr = makeRef<GLTexture3D>();
    ptr->init(usage, width, height, depth, requestFormat, mipmap, initialData);
    return ptr;
}

Ref<RHIResource> OpenGLDevice::onCreateRenderTarget(uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap, bool msaa) {
    if (msaa) {
        LN_NOTIMPLEMENTED();
        return nullptr;
    }
    auto ptr = makeRef<GLRenderTargetTexture>();
    ptr->init(width, height, requestFormat, mipmap);
    return ptr;
}

Ref<RHIResource> OpenGLDevice::onCreateWrappedRenderTarget(intptr_t nativeObject, uint32_t hintWidth, uint32_t hintHeight) {
    auto ptr = makeRef<GLRenderTargetTexture>();
    ptr->init(nativeObject, hintWidth, hintHeight);
    return ptr;
}

Ref<RHIResource> OpenGLDevice::onCreateDepthBuffer(uint32_t width, uint32_t height) {
    auto ptr = makeRef<GLDepthBuffer>();
    ptr->init(width, height);
    return ptr;
}

Ref<ISamplerState> OpenGLDevice::onCreateSamplerState(const SamplerStateData& desc) {
    auto ptr = makeRef<GLSamplerState>();
    ptr->init(this, desc);
    return ptr;
}

Ref<IShaderPass> OpenGLDevice::onCreateShaderPass(const ShaderPassCreateInfo& createInfo, ShaderCompilationDiag* diag) {
    auto ptr = makeRef<GLShaderPass>();
    if (!ptr->init(this, createInfo, createInfo.vsCode, createInfo.vsCodeLen, createInfo.psCode, createInfo.psCodeLen, diag)) {
        return nullptr;
    }
    return ptr;
}

Ref<RHIResource> OpenGLDevice::onCreateUniformBuffer(uint32_t size) {
    auto ptr = makeRef<GLUniformBuffer>();
    if (!ptr->init(size)) {
        return nullptr;
    }
    return ptr;
}

Ref<IDescriptorPool> OpenGLDevice::onCreateDescriptorPool(IShaderPass* shaderPass) {
    auto ptr = makeRef<GLDescriptorPool>();
    if (!ptr->init(this, static_cast<GLShaderPass*>(shaderPass))) {
        return nullptr;
    }
    return ptr;
}

void OpenGLDevice::onQueueSubmit(ICommandList* context, RHIResource* affectRendreTarget) {
    // CommandList と名前は付いているが、今は各操作がコマンド化されずに即実行されているため、「CommandList の実行」という意味でやることはない。
}

void OpenGLDevice::onQueuePresent(ISwapChain* swapChain) {
    static_cast<GLSwapChain*>(swapChain)->present();
}

//==============================================================================
// GLSwapChain

GLSwapChain::GLSwapChain(OpenGLDevice* device)
    : m_device(device)
    , m_backbuffer(nullptr)
    , m_fbo(0)
    , m_defaultFBO(0)
    , m_backengBufferWidth(0)
    , m_backengBufferHeight(0) {
}

void GLSwapChain::onDestroy() {
    releaseBuffers();
    ISwapChain::onDestroy();
}

void GLSwapChain::releaseBuffers() {
    if (m_fbo) {
        GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, 0));
        GL_CHECK(glDeleteFramebuffers(1, &m_fbo));
        m_fbo = 0;
    }

    if (m_backbuffer) {
        m_backbuffer->destroy();
        m_backbuffer = nullptr;
    }
}

void GLSwapChain::getBackendBufferSize(SizeI* outSize) {
    outSize->width = m_backengBufferWidth;
    outSize->height = m_backengBufferHeight;
}

void GLSwapChain::genBackbuffer(uint32_t width, uint32_t height) {
    releaseBuffers();

    m_backbuffer = makeRef<GLRenderTargetTexture>();
    m_backbuffer->init(width, height, TextureFormat::RGB8, false);

    GL_CHECK(glGenFramebuffers(1, &m_fbo));
    GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, m_fbo));

    GL_CHECK(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_backbuffer->id(), 0));

    GLenum result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    GL_CHECK2("glCheckFramebufferStatus");
    LN_ENSURE(GL_FRAMEBUFFER_COMPLETE == result, "glCheckFramebufferStatus failed 0x%08x", result);

    GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

RHIResource* GLSwapChain::getRenderTarget(int imageIndex) const {
    return m_backbuffer;
}

Result<> GLSwapChain::resizeBackbuffer(uint32_t width, uint32_t height) {
    genBackbuffer(width, height);
    setBackendBufferSize(width, height);
    return ok();
}

void GLSwapChain::setBackendBufferSize(int width, int height) {
    m_backengBufferWidth = width;
    m_backengBufferHeight = height;
}

void GLSwapChain::present() {
    SizeI endpointSize;
    getBackendBufferSize(&endpointSize);

    auto backbuffer = static_cast<GLRenderTargetTexture*>(getRenderTarget(0));
    auto bufferSize = backbuffer->extentSize();

    beginMakeContext();

    // SwapChain の Framebuffer をウィンドウのバックバッファへ転送
    {
        GL_CHECK(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, defaultFBO()));
        GL_CHECK(glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo()));

        // LN_LOG_INFO << endpointSize.width << ", " << endpointSize.height << ":" << bufferSize.width << ", " << bufferSize.height;

        // FIXME:
        // Viewport を転送元に合わせないと、転送先全体に拡大してBlitできなかった。
        // ちょっと腑に落ちないが・・・。
        GL_CHECK(glDisable(GL_SCISSOR_TEST));
        GL_CHECK(glScissor(0, 0, bufferSize.width, bufferSize.height));
        GL_CHECK(glViewport(0, 0, bufferSize.width, bufferSize.height));

        // printf("bufferSize: %d %d\n", bufferSize.width, bufferSize.height);
        //// 現在のフレームバッファにアタッチされているカラーバッファのレンダーバッファ名を取得
        // GLint colorBufferName = 0;
        // GL_CHECK(glGetFramebufferAttachmentParameteriv(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME, &colorBufferName));

        //// レンダーバッファ(カラーバッファ)をバインド
        // GL_CHECK(glBindRenderbuffer(GL_RENDERBUFFER, colorBufferName));

        // カラーバッファの幅と高さを取得
        // GLint endpointWidth;
        // GLint endpointHeight;
        // GL_CHECK(glGetRenderbufferParameteriv(GL_FRAMEBUFFER, GL_RENDERBUFFER_WIDTH, &endpointWidth));
        // GL_CHECK(glGetRenderbufferParameteriv(GL_FRAMEBUFFER, GL_RENDERBUFFER_HEIGHT, &endpointHeight));

        // Blit
        // ※EAGL(iOS) は destination が FBO ではない場合失敗する。それ以外は RenderTarget でも成功していた。
        // TODO: デュアルディスプレイで指しなおすと、次回起動時に失敗する。PC再起動で治る。
        GL_CHECK(glBlitFramebuffer(0, 0, bufferSize.width, bufferSize.height, 0, 0, endpointSize.width, endpointSize.height, GL_COLOR_BUFFER_BIT, GL_NEAREST));

        GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, defaultFBO()));
    }

    swap();

    endMakeContext();
}

////==============================================================================
//// EmptyGLContext
//
// Ref<GLSwapChain> EmptyGLContext::createSwapChain(PlatformWindow* window, const SizeI& backbufferSize)
//{
//	auto ptr = makeRef<EmptyGLSwapChain>(nullptr);
//    ptr->setBackendBufferSize(backbufferSize.width, backbufferSize.height);
//	ptr->genBackbuffer(backbufferSize.width, backbufferSize.height);
//	return ptr;
//}
//
// void EmptyGLContext::makeCurrent(GLSwapChain* swapChain)
//{
//}
//
// void EmptyGLContext::swap(GLSwapChain* swapChain)
//{
//}


//===============================================================================
// GLVertexDeclaration

GLVertexDeclaration::GLVertexDeclaration()
    : m_vertexElements() {
}

GLVertexDeclaration::~GLVertexDeclaration() {
}

void GLVertexDeclaration::init(const VertexElement* elements, int elementsCount) {
    if (LN_REQUIRE(elements != nullptr)) return;
    if (LN_REQUIRE(elementsCount >= 0)) return;

    // 頂点宣言作成
    createGLVertexElements(elements, elementsCount, &m_vertexElements);
}

void GLVertexDeclaration::onDestroy() {
    IVertexDeclaration::onDestroy();
}

const GLVertexElement* GLVertexDeclaration::findGLVertexElement(kokage::AttributeUsage usage, int usageIndex) const {
    // TODO: これ線形探索じゃなくて、map 作った方がいいかも。
    // usage の種類は固定だし、usageIndex も最大 16 あれば十分だし、byte 型 8x16 くらいの Matrix で足りる。
    auto u = IGraphicsHelper::AttributeUsageToElementUsage(usage);
    for (auto& e : m_vertexElements) {
        if (e.usage == u && e.usageIndex == usageIndex) {
            return &e;
        }
    }
    return nullptr;
}

void GLVertexDeclaration::createGLVertexElements(const VertexElement* vertexElements, int elementsCount, List<GLVertexElement>* outList) {
    outList->reserve(elementsCount);

    int sizeInStream[16] = { 0 };

    int totalSize = 0;
    for (int i = 0; i < elementsCount; ++i) {
        GLVertexElement elm;
        elm.streamIndex = vertexElements[i].StreamIndex;
        elm.usage = vertexElements[i].Usage;
        elm.usageIndex = vertexElements[i].UsageIndex;

        convertDeclTypeLNToGL(
            vertexElements[i].Type,
            &elm.type,
            &elm.size,
            &elm.normalized);

        elm.byteOffset = sizeInStream[elm.streamIndex];
        sizeInStream[elm.streamIndex] += RHIHelper::getVertexElementTypeSize(vertexElements[i].Type);

        elm.instance = (vertexElements[i].rate == VertexInputRate::Instance);

        // elm.stride = getVertexSize(vertexElements, elementsCount, elm.streamIndex);
        // elm.byteOffset = totalSize;
        outList->add(elm);

        // totalSize += getVertexElementTypeSize(vertexElements[i].Type);
    }

    for (auto& e : *outList) {
        e.stride = sizeInStream[e.streamIndex];
    }
}

void GLVertexDeclaration::convertDeclTypeLNToGL(VertexElementType type, GLenum* gl_type, GLint* size, GLboolean* normalized) {
    struct FormatType {
        GLenum Type;
        GLint Size;
        GLboolean normalize;
    };

    static const FormatType formatTable[] = {
        { 0, 0, GL_FALSE },                // VertexElementType::Unknown
        { GL_FLOAT, 1, GL_FALSE },         // VertexElementType::Float1
        { GL_FLOAT, 2, GL_FALSE },         // VertexElementType::Float2
        { GL_FLOAT, 3, GL_FALSE },         // VertexElementType::Float3
        { GL_FLOAT, 4, GL_FALSE },         // VertexElementType::Float4
        { GL_UNSIGNED_BYTE, 4, GL_FALSE }, // VertexElementType::Ubyte4
        { GL_UNSIGNED_BYTE, 4, GL_TRUE },  // VertexElementType::Color4
        { GL_SHORT, 2, GL_FALSE },         // VertexElementType::Short2
        { GL_SHORT, 4, GL_FALSE },         // VertexElementType::Short4
    };
    // http://www.opengl.org/sdk/docs/man/xhtml/glVertexAttribPointer.xml
    // GL_BYTE, GL_UNSIGNED_BYTE, GL_SHORT, GL_UNSIGNED_SHORT, GL_INT, GL_UNSIGNED_INT, GL_FLOAT, GL_DOUBLE

    *gl_type = formatTable[(int)type].Type;
    *size = formatTable[(int)type].Size;
    *normalized = formatTable[(int)type].normalize;
}

//==============================================================================
// GLPipeline

GLPipeline::GLPipeline()
    : m_device(nullptr)
    , m_blendState()
    , m_rasterizerState()
    , m_depthStencilState()
    , m_primitiveTopology(0) {
}

Result<> GLPipeline::init(OpenGLDevice* device, const DevicePipelineStateDesc& state) {
    m_device = device;
    m_blendState = state.blendState;
    m_rasterizerState = state.rasterizerState;
    m_depthStencilState = state.depthStencilState;

    switch (state.topology) {
        case PrimitiveTopology::TriangleList:
            m_primitiveTopology = GL_TRIANGLES;
            break;
        case PrimitiveTopology::TriangleStrip:
            m_primitiveTopology = GL_TRIANGLE_STRIP;
            break;
        case PrimitiveTopology::TriangleFan:
            m_primitiveTopology = GL_TRIANGLE_FAN;
            break;
        case PrimitiveTopology::LineList:
            m_primitiveTopology = GL_LINES;
            break;
        case PrimitiveTopology::LineStrip:
            m_primitiveTopology = GL_LINE_STRIP;
            break;
        case PrimitiveTopology::PointList:
            m_primitiveTopology = GL_POINTS;
            break;
        default:
            LN_UNREACHABLE();
            break;
    }

    return ok();
}

void GLPipeline::onDestroy() {
    IPipeline::onDestroy();
}

void GLPipeline::bind(
    GLGraphicsContext* commandList,
    const std::array<RHIResource*, MaxVertexStreams>& vertexBuffers,
    const RHIResource* indexBuffer,
    IDescriptor* descriptor) {
    
    // BlendState
    commandList->state()->setBlendState(m_blendState);

    // RasterizerState
    commandList->state()->setRasterizerState(m_rasterizerState);

    // DepthStencilState
    commandList->state()->setDepthStencilState(m_depthStencilState);

    // PrimitiveData
    commandList->state()->setPrimitiveData(
        static_cast<const GLVertexDeclaration*>(vertexLayout()),
        static_cast<const GLShaderPass*>(shaderPass()),
        vertexBuffers
    );

    // IndexBuffer
    commandList->state()->setIndexBuffer(static_cast<const GLIndexBuffer*>(indexBuffer));

    // shaderPass
    if (auto* pass = static_cast<const GLShaderPass*>(shaderPass())) {
        commandList->state()->useProgram(pass->program());
        if (auto* d = static_cast<GLDescriptor*>(descriptor)) {
            d->bind(pass);
        }
    }
}

// WebGL や OpenGLES で glDrawElementsBaseVertex() が使えないので、glVertexAttribPointer() の pointer にオフセットを加える対策 
void GLPipeline::rebindAttr(int vertexOffset) {

    auto* glDecl = static_cast<const GLVertexDeclaration*>(vertexLayout());
    if (glDecl) {
        const auto& attributes = shaderPass()->attributes();
        size_t count = attributes.size();
        for (size_t iAttr = 0; iAttr < count; iAttr++) {
            auto& attr = attributes[iAttr];

            // glslang からは、 SV_InstanceID も取得できるが、これには layoutLocation が付いていない。
            if (attr.usage == kokage::AttributeUsage_InstanceID) continue;

            if (const auto* element = glDecl->findGLVertexElement(attr.usage, attr.index)) {
                GL_CHECK_DEBUG(glVertexAttribPointer(attr.layoutLocation, element->size, element->type, element->normalized, element->stride, (void*)(element->stride * vertexOffset + element->byteOffset)));
            }
        }
    }
}

} // namespace detail
} // namespace ln
