
#include "Internal.hpp"
#include <LuminoPlatform/detail/OpenGLContext.hpp>
#include <LuminoPlatform/detail/PlatformManager.hpp>
//#include <LuminoEngine/Graphics/Bitmap.hpp>
//#include <LuminoEngine/Platform/PlatformWindow.hpp>
//#include "../../Platform/PlatformManager.hpp"
//#include "../../Platform/OpenGLContext.hpp"
#include "OpenGLDeviceContext.hpp"
#include "GLBuffer.hpp"
#include "GLUniformBuffer.hpp"
#include "GLTextures.hpp"
#include "GLDepthBuffer.hpp"
#include "GLShaderPass.hpp"
#include "GLDescriptorPool.hpp"
#include "GLCommandList.hpp"
#include "GLFWSwapChain.hpp"

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
//Result IGraphicsDevice::getOpenGLCurrentFramebufferTextureId(int* id)
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

//=============================================================================
// OpenGLDevice

OpenGLDevice::OpenGLDevice()
    : m_mainWindow(nullptr)
    , m_mainGLContext(nullptr)
    , m_uniformTempBuffer()
    , m_uniformTempBufferWriter(&m_uniformTempBuffer) {
}

Result OpenGLDevice::init(const Settings& settings) {
    LN_LOG_DEBUG("OpenGLDeviceContext::init start");

    // Create main context
    {
        m_mainWindow = settings.mainWindow;
        m_mainGLContext = settings.platformManager->openGLContext();
        m_mainGLContext->makeCurrentMain();
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
    LN_LOG_INFO << "OpenGL ES enabled.";
#endif

#if defined(LN_GRAPHICS_OPENGLES)
#else
    int result = gladLoadGL();
    if (LN_ENSURE(result, "Failed gladLoadGL()")) {
        // OpenGL Context がアクティブになっていないと失敗する。
        return err();
    }

    LN_LOG_INFO("OpenGL {}.{}", GLVersion.major, GLVersion.minor);
#endif

    // Check caps.
    {
        LN_LOG_INFO("GL_VERSION : {}", glGetString(GL_VERSION));

        GL_CHECK(glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &m_caps.MAX_VERTEX_ATTRIBS));
        GL_CHECK(glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &m_caps.MAX_COLOR_ATTACHMENTS));
        LN_LOG_INFO("GL_MAX_VERTEX_ATTRIBS : {}", m_caps.MAX_VERTEX_ATTRIBS);
        LN_LOG_INFO("GL_MAX_COLOR_ATTACHMENTS : {}", m_caps.MAX_COLOR_ATTACHMENTS);

        int extensions = 0;
        glGetIntegerv(GL_EXTENSIONS, &extensions);
        LN_LOG_INFO("GL_EXTENSIONS : {}", extensions);
        for (int i = 0; i < extensions; i++) {
            LN_LOG_INFO("  {}", glGetStringi(GL_EXTENSIONS, i));
        }
        while (glGetError() != 0)
            ; // ignore error.

        const char* extensionsString = (const char*)glGetString(GL_EXTENSIONS);
        while (glGetError() != 0)
            ; // ignore error.
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

    m_graphicsQueue = makeRef<GLCommandQueue>();
    if (!m_graphicsQueue->init()) {
        return err();
    }

    const size_t PageSize = 0x200000; // 2MB
    m_uniformBufferAllocatorPageManager = makeRef<GLUniformBufferAllocatorPageManager>(this, PageSize);


    //m_graphicsContext = makeRef<GLGraphicsContext>();
    //m_graphicsContext->init(this);

    LN_LOG_DEBUG("OpenGLDeviceContext::init end");
    return ok();
}

void OpenGLDevice::dispose() {
    if (m_graphicsQueue) {
        releaseObject(m_graphicsQueue);
        m_graphicsQueue = nullptr;
    }

    IGraphicsDevice::dispose();
}

//ICommandList* OpenGLDevice::getGraphicsContext() const
//{
//	return m_graphicsContext;
//}

void OpenGLDevice::onGetCaps(GraphicsDeviceCaps* outCaps) {
    outCaps->graphicsAPI = GraphicsAPI::OpenGL;
#ifdef LN_GRAPHICS_OPENGLES
    outCaps->requestedShaderTriple.target = "glsl";
    outCaps->requestedShaderTriple.version = 300;
    outCaps->requestedShaderTriple.option = "es";
#else
    outCaps->requestedShaderTriple.target = "glsl";
    outCaps->requestedShaderTriple.version = 400;
    outCaps->requestedShaderTriple.option = "";
#endif
    outCaps->imageLayoytVFlip = true;

    GLint align = 0;
    glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &align);
    outCaps->uniformBufferOffsetAlignment = align;
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
    //if (LN_REQUIRE(!m_commandListCreated)) return nullptr;	// OpenGL では複数 CommandList の作成を禁止する

    auto ptr = makeRef<GLGraphicsContext>();
    if (!ptr->init(this)) {
        return nullptr;
    }

    //m_commandListCreated = true;
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
    ptr->init(usage, format, indexCount, initialData);
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
    ptr->init(this, createInfo, createInfo.vsCode, createInfo.vsCodeLen, createInfo.psCode, createInfo.psCodeLen, diag);
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

void OpenGLDevice::onSubmitCommandBuffer(ICommandList* context, RHIResource* affectRendreTarget) {
    LN_NOTIMPLEMENTED();
}

ICommandQueue* OpenGLDevice::getGraphicsCommandQueue() {
    return m_graphicsQueue;
}

ICommandQueue* OpenGLDevice::getComputeCommandQueue() {
    // Not supported.
    return nullptr;
}

//=============================================================================
// GLSwapChain

GLSwapChain::GLSwapChain(OpenGLDevice* device)
    : m_device(device)
    , m_backbuffer(nullptr)
    , m_fbo(0)
    , m_defaultFBO(0)
    , m_backengBufferWidth(0)
    , m_backengBufferHeight(0) {
}

void GLSwapChain::dispose() {
    releaseBuffers();
    ISwapChain::dispose();
}

void GLSwapChain::releaseBuffers() {
    if (m_fbo) {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDeleteFramebuffers(1, &m_fbo);
        m_fbo = 0;
    }

    if (m_backbuffer) {
        m_backbuffer->dispose();
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

    LN_ENSURE(GL_FRAMEBUFFER_COMPLETE == glCheckFramebufferStatus(GL_FRAMEBUFFER), "glCheckFramebufferStatus failed 0x%08x", glCheckFramebufferStatus(GL_FRAMEBUFFER));

    GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

RHIResource* GLSwapChain::getRenderTarget(int imageIndex) const {
    return m_backbuffer;
}

Result GLSwapChain::resizeBackbuffer(uint32_t width, uint32_t height) {
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

        //LN_LOG_INFO << endpointSize.width << ", " << endpointSize.height << ":" << bufferSize.width << ", " << bufferSize.height;

        // FIXME:
        // Viewport を転送元に合わせないと、転送先全体に拡大してBlitできなかった。
        // ちょっと腑に落ちないが・・・。
        GL_CHECK(glDisable(GL_SCISSOR_TEST));
        GL_CHECK(glScissor(0, 0, bufferSize.width, bufferSize.height));
        GL_CHECK(glViewport(0, 0, bufferSize.width, bufferSize.height));

        //printf("bufferSize: %d %d\n", bufferSize.width, bufferSize.height);
        //// 現在のフレームバッファにアタッチされているカラーバッファのレンダーバッファ名を取得
        //GLint colorBufferName = 0;
        //GL_CHECK(glGetFramebufferAttachmentParameteriv(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME, &colorBufferName));

        //// レンダーバッファ(カラーバッファ)をバインド
        //GL_CHECK(glBindRenderbuffer(GL_RENDERBUFFER, colorBufferName));

        // カラーバッファの幅と高さを取得
        //GLint endpointWidth;
        //GLint endpointHeight;
        //GL_CHECK(glGetRenderbufferParameteriv(GL_FRAMEBUFFER, GL_RENDERBUFFER_WIDTH, &endpointWidth));
        //GL_CHECK(glGetRenderbufferParameteriv(GL_FRAMEBUFFER, GL_RENDERBUFFER_HEIGHT, &endpointHeight));

        // Blit
        // ※EAGL(iOS) は destination が FBO ではない場合失敗する。それ以外は RenderTarget でも成功していた。
        // TODO: デュアルディスプレイで指しなおすと、次回起動時に失敗する。PC再起動で治る。
        GL_CHECK(glBlitFramebuffer(0, 0, bufferSize.width, bufferSize.height, 0, 0, endpointSize.width, endpointSize.height, GL_COLOR_BUFFER_BIT, GL_NEAREST));

        GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, defaultFBO()));
    }

    swap();

    endMakeContext();
}

////=============================================================================
//// EmptyGLContext
//
//Ref<GLSwapChain> EmptyGLContext::createSwapChain(PlatformWindow* window, const SizeI& backbufferSize)
//{
//	auto ptr = makeRef<EmptyGLSwapChain>(nullptr);
//    ptr->setBackendBufferSize(backbufferSize.width, backbufferSize.height);
//	ptr->genBackbuffer(backbufferSize.width, backbufferSize.height);
//	return ptr;
//}
//
//void EmptyGLContext::makeCurrent(GLSwapChain* swapChain)
//{
//}
//
//void EmptyGLContext::swap(GLSwapChain* swapChain)
//{
//}

//==============================================================================
// GLCommandQueue

GLCommandQueue::GLCommandQueue() {
}

Result GLCommandQueue::init() {
    return ok();
}

Result GLCommandQueue::submit(ICommandList* commandList) {
    glFlush();
    return ok();
}

//==============================================================================
// GLRenderPass

GLRenderPass::GLRenderPass() {
}

Result GLRenderPass::init(OpenGLDevice* device, const DeviceFramebufferState& buffers, ClearFlags clearFlags, const Color& clearColor, float clearDepth, uint8_t clearStencil) {
    m_device = device;

    for (auto i = 0; i < buffers.renderTargets.size(); i++) {
        m_renderTargets[i] = static_cast<GLTextureBase*>(buffers.renderTargets[i]);
    }

    m_depthBuffer = static_cast<GLDepthBuffer*>(buffers.depthBuffer);
    m_clearFlags = clearFlags;
    m_clearColor = clearColor;
    m_clearDepth = clearDepth;
    m_clearStencil = clearStencil;
    return ok();
}

void GLRenderPass::dispose() {
    IRenderPass::dispose();
}

//SizeI GLRenderPass::viewSize() const {
//    return m_renderTargets[0]->realSize();
//}

void GLRenderPass::bind(GLGraphicsContext* context) {
    auto fbo = context->fbo();
    if (fbo) {
        GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, fbo));
    }

    auto backbuffer = m_renderTargets[0].get();
    auto baseSize = backbuffer->extentSize();

    // color buffers
    std::array<GLenum, MaxMultiRenderTargets> buffers;
    int renderTargetsCount = m_renderTargets.size();
    int maxCount = std::min(renderTargetsCount, m_device->caps().MAX_COLOR_ATTACHMENTS);
    //int actualCount = 0;
    for (int i = 0; i < renderTargetsCount; ++i) {
        if (m_renderTargets[i]) {
            LN_CHECK(m_renderTargets[i]->extentSize() == baseSize);
            GLuint id = static_cast<GLTextureBase*>(m_renderTargets[i])->id();
            GL_CHECK(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, id, 0));
            buffers[i] = GL_COLOR_ATTACHMENT0 + i;
        }
        else {
            GL_CHECK(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, 0, 0));
            buffers[i] = GL_NONE;
        }
    }

    // depth buffer
    if (m_depthBuffer) {
        LN_CHECK(m_depthBuffer->extentSize() == baseSize);
        GLuint id = static_cast<GLDepthBuffer*>(m_depthBuffer)->id();
        GL_CHECK(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, id));
        GL_CHECK(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, id));
    }
    else {
        GL_CHECK(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, 0));
        GL_CHECK(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, 0));
    }

    LN_ENSURE(GL_FRAMEBUFFER_COMPLETE == glCheckFramebufferStatus(GL_FRAMEBUFFER), "glCheckFramebufferStatus failed 0x%08x", glCheckFramebufferStatus(GL_FRAMEBUFFER));

    GL_CHECK(glDrawBuffers(buffers.size(), buffers.data()));

    OpenGLHelper::clearBuffers(m_clearFlags, m_clearColor, m_clearDepth, m_clearStencil);
}

//==============================================================================
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

void GLVertexDeclaration::dispose() {
    IVertexDeclaration::dispose();
}

const GLVertexElement* GLVertexDeclaration::findGLVertexElement(AttributeUsage usage, int usageIndex) const {
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
        sizeInStream[elm.streamIndex] += GraphicsHelper::getVertexElementTypeSize(vertexElements[i].Type);

        elm.instance = (vertexElements[i].rate == VertexInputRate::Instance);

        //elm.stride = getVertexSize(vertexElements, elementsCount, elm.streamIndex);
        //elm.byteOffset = totalSize;
        outList->add(elm);

        //totalSize += getVertexElementTypeSize(vertexElements[i].Type);
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

//=============================================================================
// GLPipeline

GLPipeline::GLPipeline()
    : m_device(nullptr)
    , m_blendState()
    , m_rasterizerState()
    , m_depthStencilState()
    , m_primitiveTopology(0) {
}

Result GLPipeline::init(OpenGLDevice* device, const DevicePipelineStateDesc& state) {
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

void GLPipeline::dispose() {
    IPipeline::dispose();
}

void GLPipeline::bind(const std::array<RHIResource*, MaxVertexStreams>& vertexBuffers, const RHIResource* indexBuffer) {
    // UpdateStatus
    {
        // BlendState
        {
            GLenum blendOpTable[] = {
                GL_FUNC_ADD,
                GL_FUNC_SUBTRACT,
                GL_FUNC_REVERSE_SUBTRACT,
                GL_MIN,
                GL_MAX,
            };

    GLenum blendFactorTable[] = {
        GL_ZERO,
        GL_ONE,
        GL_SRC_COLOR,
        GL_ONE_MINUS_SRC_COLOR,
        GL_SRC_ALPHA,
        GL_ONE_MINUS_SRC_ALPHA,
        GL_DST_COLOR,
        GL_ONE_MINUS_DST_COLOR,
        GL_DST_ALPHA,
        GL_ONE_MINUS_DST_ALPHA
    };

#ifdef GL_GLES_PROTOTYPES
    // OpenGL ES is unsupported
#else
    if (m_blendState.independentBlendEnable) {
        for (int i = 0; i < 8; i++) // TODO: num RT
        {
            const RenderTargetBlendDesc& desc = m_blendState.renderTargets[i];

            // blendEnable
            if (desc.blendEnable) {
                GL_CHECK(glEnablei(GL_BLEND, i));
            }
            else {
                GL_CHECK(glEnablei(GL_BLEND, i));
            }

            // sourceBlend
            // destinationBlend
            GL_CHECK(glBlendFuncSeparatei(i, blendFactorTable[(int)desc.sourceBlend], blendFactorTable[(int)desc.destinationBlend], blendFactorTable[(int)desc.sourceBlendAlpha], blendFactorTable[(int)desc.destinationBlendAlpha]));

            // blendOp
            GL_CHECK(glBlendEquationSeparatei(i, blendOpTable[(int)desc.blendOp], blendOpTable[(int)desc.blendOpAlpha]));
        }
    }
    else
#endif
    {
        const RenderTargetBlendDesc& desc = m_blendState.renderTargets[0];

        // blendEnable
        if (desc.blendEnable) {
            GL_CHECK(glEnable(GL_BLEND));
        }
        else {
            GL_CHECK(glDisable(GL_BLEND));
        }

        // blendOp
        {
            GL_CHECK(glBlendEquationSeparate(
                blendOpTable[(int)desc.blendOp],
                blendOpTable[(int)desc.blendOpAlpha]));
        }

        // sourceBlend
        // destinationBlend
        {
            GL_CHECK(glBlendFuncSeparate(
                blendFactorTable[(int)desc.sourceBlend],
                blendFactorTable[(int)desc.destinationBlend],
                blendFactorTable[(int)desc.sourceBlendAlpha],
                blendFactorTable[(int)desc.destinationBlendAlpha]));
        }
    }
}

// RasterizerState
{
    // fillMode
    {
#ifdef GL_GLES_PROTOTYPES
// OpenGL ES is glPolygonMode unsupported
#else
        const GLenum tb[] = { GL_FILL, GL_LINE, GL_POINT };
GL_CHECK(glPolygonMode(GL_FRONT_AND_BACK, tb[(int)m_rasterizerState.fillMode]));
#endif
    }

    // cullingMode
    {
        switch (m_rasterizerState.cullMode){
            case CullMode::None :
                GL_CHECK(glDisable(GL_CULL_FACE));
break;
case CullMode::Front:
    GL_CHECK(glEnable(GL_CULL_FACE));
    GL_CHECK(glCullFace(GL_BACK));
    break;
case CullMode::Back:
    GL_CHECK(glEnable(GL_CULL_FACE));
    GL_CHECK(glCullFace(GL_FRONT));
    break;
}
}
}

// DepthStencilState
{
    GLenum cmpFuncTable[] = {
        GL_NEVER,    // Never
        GL_LESS,     // Less
        GL_LEQUAL,   // LessEqual
        GL_GREATER,  // Greater
        GL_GEQUAL,   // GreaterEqual
        GL_EQUAL,    // Equal
        GL_NOTEQUAL, // NotEqual
        GL_ALWAYS,   // Always
    };

    //// depthTestEnabled
    //if (depthStencilState.depthTestEnabled) {
    //	GL_CHECK(glEnable(GL_DEPTH_TEST));
    //}
    //else {
    //	GL_CHECK(glDisable(GL_DEPTH_TEST));
    //}

    if (m_depthStencilState.depthTestFunc == ComparisonFunc::Always) {
        GL_CHECK(glDisable(GL_DEPTH_TEST));
    }
    else {
        GL_CHECK(glEnable(GL_DEPTH_TEST));
        GL_CHECK(glDepthFunc(cmpFuncTable[(int)m_depthStencilState.depthTestFunc]));
    }

    // depthWriteEnabled
    GL_CHECK(glDepthMask(m_depthStencilState.depthWriteEnabled ? GL_TRUE : GL_FALSE));

    // stencilEnabled
    if (m_depthStencilState.stencilEnabled) {
        GL_CHECK(glEnable(GL_STENCIL_TEST));
    }
    else {
        GL_CHECK(glDisable(GL_STENCIL_TEST));
    }

    // stencilFunc
    // stencilReferenceValue
    GL_CHECK(glStencilFuncSeparate(GL_BACK, cmpFuncTable[(int)m_depthStencilState.frontFace.stencilFunc], m_depthStencilState.stencilReferenceValue, 0xFFFFFFFF));
    GL_CHECK(glStencilFuncSeparate(GL_FRONT, cmpFuncTable[(int)m_depthStencilState.backFace.stencilFunc], m_depthStencilState.stencilReferenceValue, 0xFFFFFFFF));

    // stencilFailOp
    // stencilDepthFailOp
    // stencilPassOp
    GLenum stencilOpTable[] = { GL_KEEP, GL_REPLACE };
    GL_CHECK(glStencilOpSeparate(GL_BACK, stencilOpTable[(int)m_depthStencilState.frontFace.stencilFailOp], stencilOpTable[(int)m_depthStencilState.frontFace.stencilDepthFailOp], stencilOpTable[(int)m_depthStencilState.frontFace.stencilPassOp]));
    GL_CHECK(glStencilOpSeparate(GL_FRONT, stencilOpTable[(int)m_depthStencilState.backFace.stencilFailOp], stencilOpTable[(int)m_depthStencilState.backFace.stencilDepthFailOp], stencilOpTable[(int)m_depthStencilState.backFace.stencilPassOp]));
}
}

// PrimitiveData
{
    auto* glDecl = static_cast<const GLVertexDeclaration*>(vertexLayout());
    if (glDecl) {
        size_t count = shaderPass()->attributes().size();
        for (size_t iAttr = 0; iAttr < count; iAttr++) {
            auto& attr = shaderPass()->attributes()[iAttr];

            if (const auto* element = glDecl->findGLVertexElement(attr.usage, attr.index)) {

                GL_CHECK(glEnableVertexAttribArray(attr.layoutLocation));
                GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, static_cast<const GLVertexBuffer*>(vertexBuffers[element->streamIndex])->vertexBufferId()));
                GL_CHECK(glVertexAttribPointer(attr.layoutLocation, element->size, element->type, element->normalized, element->stride, (void*)(element->byteOffset)));

                if (element->instance) {
                    glVertexAttribDivisor(attr.layoutLocation, 1);
                }
                else {
                    glVertexAttribDivisor(attr.layoutLocation, 0);
                }
            }
            else {
                GL_CHECK(glDisableVertexAttribArray(attr.layoutLocation));
                GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));
            }
        }
    }

    auto* glIndexBuffer = static_cast<const GLIndexBuffer*>(indexBuffer);
    if (glIndexBuffer) {
        GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glIndexBuffer->indexBufferId()));
    }
    else {
        GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    }
}

// shaderPass
if (auto* pass = static_cast<const GLShaderPass*>(shaderPass())) {
    pass->apply();
}
}

} // namespace detail
} // namespace ln
