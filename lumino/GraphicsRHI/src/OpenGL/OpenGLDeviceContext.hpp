/*
 * SwapChain 周りの動作について
 * ----------
 * OpenGL context がデフォルトで作るバックバッファは、モジュールの外に公開しない。
 * 自前で1枚カラーバッファを作ってそれを公開している。
 * swap の際は、まずこのカラーバッファからバックバッファへデータを転送してから swap する。
 * 
 * なぜこうしているかというと、デフォルトのバックバッファをバインドするには
 * glBindFrambuffer で 0 を指定しなければならないため。
 * SwapChain を複数作る場合は共有されたコンテキストを複数作る必要があるが、
 * 各コンテキストのバックバッファはどれも 0 で指定する必要があるため区別できない。
 * 
 * DirectX のように、RenderTarget の切り替えはテクスチャでも SwapChain でも
 * 同じ方法で実現したいのだが、上記対策を行いわない場合、SwapChain の時は毎回 makeCurrent する必要がある。
 * 
 * それはそれでできなくはないが、たとえ共有コンテキスト間でも共有できない OpenGL オブジェクトがいくつかあり、
 * そういった諸々の管理を考えると実装が複雑になってしまう。
 * 
 *
 * マルチスレッドについて
 * ----------
 * OpenGLDevice はマルチスレッドレンダリングをサポートしない。
 * 大きな理由は、マルチスレッドでの動作がちゃんと規格化されていないから。
 * プラットフォーム間での差が激しく、カバーしきれない。
 *
 * 他、
 * - Emscripten で使うときはそもそもスレッドが使えない。
 * - GLFW で共有コンテキストを作るときは必ずウィンドウを作る必要がある。（ダミー作成の処理を組むと大変）
 *
 * 特に Emscripten で使うときの環境の制約が大きいかも。
 * 素直にシングルスレッドで動作するデバイスとして位置づけたい。
 *
 * マルチスレッドレンダリングやるなら Vulkan、DX12 などを使おう。
 *
 * 
 * glMapBuffer は使わない。
 * ----------
 * OpenGL ES では READ モードでロックできないため。
 * つまり 一度 OpenGL にデータを送ってしまった後、その値を得ることができなくなってしまう。
 * 通常の lock の用途なら Write だけでほぼ問題ないが、
 * デバイスロストしたときの復帰ではバックアップから GL の頂点バッファを作り直さなければならず、
 * 結局こちら側でずっと握っていた方が色々と都合が良かったりする。
 *
 * 
 * マルチターゲットの参考:http://ramemiso.hateblo.jp/entry/2013/10/20/001909
 * 
 * 
 * 
 * 
 * 
 */
#pragma once
#include "GLHelper.hpp"

namespace ln {
namespace detail {
class PlatformManager;
class OpenGLContext;
class GLUniformBufferAllocatorPageManager;
class GLDescriptorObjectPoolManager;

class OpenGLDevice : public IGraphicsDevice {
public:
    struct Settings {
        PlatformManager* platformManager = nullptr;
        PlatformWindow* mainWindow = nullptr; // nullptr の場合、OpenGL Context の生成・管理を内部で行わない
        uint32_t defaultFramebuffer;
    };

    struct Caps {
        GLint MAX_VERTEX_ATTRIBS = 0;
        GLint MAX_COLOR_ATTACHMENTS = 0;
        GLfloat MAX_TEXTURE_MAX_ANISOTROPY_EXT = 0;
        bool support_filter_anisotropic = false;
    };

    OpenGLDevice();
    virtual ~OpenGLDevice() = default;

    Result<> init(const Settings& settings);
    virtual void dispose() override;
    PlatformWindow* mainWindow() const { return m_mainWindow; }
    //OpenGLContext* mainGLContext() const { return m_mainGLContext; }
    const Caps& caps() const { return m_caps; }

    // uniform set の時、Vector4[] → vec2[] に変換するための一時バッファ
    MemoryStream* uniformTempBuffer() { return &m_uniformTempBuffer; }
    BinaryWriter* uniformTempBufferWriter() { return &m_uniformTempBufferWriter; }

    const Ref<GLUniformBufferAllocatorPageManager>& uniformBufferAllocatorPageManager() const { return m_uniformBufferAllocatorPageManager; }
    const Ref<GLDescriptorObjectPoolManager>& descriptorObjectPoolManager() const { return m_descriptorObjectPoolManager; }

protected:
    INativeGraphicsInterface* getNativeInterface() const override { return nullptr; }

    void onGetCaps(GraphicsDeviceCaps* outCaps) override;
    Ref<ISwapChain> onCreateSwapChain(PlatformWindow* window, const SizeI& backbufferSize) override;
    Ref<ICommandList> onCreateCommandList() override;
    Ref<IRenderPass> onCreateRenderPass(const DeviceFramebufferState& buffers, ClearFlags clearFlags, const Color& clearColor, float clearDepth, uint8_t clearStencil) override;
    Ref<IPipeline> onCreatePipeline(const DevicePipelineStateDesc& state) override;
    Ref<IVertexDeclaration> onCreateVertexDeclaration(const VertexElement* elements, int elementsCount) override;
    Ref<RHIResource> onCreateVertexBuffer(GraphicsResourceUsage usage, size_t bufferSize, const void* initialData) override;
    Ref<RHIResource> onCreateIndexBuffer(GraphicsResourceUsage usage, IndexBufferFormat format, int indexCount, const void* initialData) override;
    Ref<RHIResource> onCreateTexture2D(GraphicsResourceUsage usage, uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap, const void* initialData) override;
    Ref<RHIResource> onCreateTexture3D(GraphicsResourceUsage usage, uint32_t width, uint32_t height, uint32_t depth, TextureFormat requestFormat, bool mipmap, const void* initialData) override;
    Ref<RHIResource> onCreateRenderTarget(uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap, bool msaa) override;
    Ref<RHIResource> onCreateWrappedRenderTarget(intptr_t nativeObject, uint32_t hintWidth, uint32_t hintHeight) override;
    Ref<RHIResource> onCreateDepthBuffer(uint32_t width, uint32_t height) override;
    Ref<ISamplerState> onCreateSamplerState(const SamplerStateData& desc) override;
    Ref<IShaderPass> onCreateShaderPass(const ShaderPassCreateInfo& createInfo, ShaderCompilationDiag* diag) override;
    Ref<RHIResource> onCreateUniformBuffer(uint32_t size) override;
    Ref<IDescriptorPool> onCreateDescriptorPool(IShaderPass* shaderPass) override;
    void onQueueSubmit(ICommandList* context, RHIResource* affectRendreTarget) override;
    void onQueuePresent(ISwapChain* swapChain) override;


private:
    PlatformWindow* m_mainWindow;
    MemoryStream m_uniformTempBuffer;
    BinaryWriter m_uniformTempBufferWriter;
    Caps m_caps;

    Ref<GLUniformBufferAllocatorPageManager> m_uniformBufferAllocatorPageManager;
    Ref<GLDescriptorObjectPoolManager> m_descriptorObjectPoolManager;
};

class GLSwapChain
    : public ISwapChain {
public:
    GLSwapChain(OpenGLDevice* device);
    virtual ~GLSwapChain() = default;
    virtual void onDestroy() override;
    virtual uint32_t getBackbufferCount() override { return 1; }
    virtual void acquireNextImage(int* outImageIndex) override { *outImageIndex = 0; }
    virtual RHIResource* getRenderTarget(int imageIndex) const override;
    virtual Result<> resizeBackbuffer(uint32_t width, uint32_t height) override;
    
    void present();

    virtual void getBackendBufferSize(SizeI* outSize);

    void genBackbuffer(uint32_t width, uint32_t height);
    GLuint fbo() const { return m_fbo; }

    OpenGLDevice* device() const { return m_device; }
    GLuint defaultFBO() const { return m_defaultFBO; }
    void setDefaultFBO(GLuint id) { m_defaultFBO = id; }

    void setBackendBufferSize(int width, int height);

protected:
    virtual void swap() = 0;
    virtual void beginMakeContext() = 0;
    virtual void endMakeContext() = 0;

private:
    void releaseBuffers();

    OpenGLDevice* m_device;
    Ref<GLRenderTargetTexture> m_backbuffer;
    GLuint m_fbo;

    // ネイティブの UI view と関連付けられている最終的なバックバッファを示す FBO。
    // 特に、EAGL (iOS) のために用意されている。
    // WGL や NSGL はデフォルトの FBO はコンテキストによって生成され Id は 0 となっているが、EAGL では生成されない。
    // このため glBlitFramebuffer のために Id 0 をバインドすると、「FBO ではない」エラーとなってしまう。
    // GLKView::bindDrawable によって glBlitFramebuffer 可能な FBO を作ることができるが、その Id は 0 ではないので、GLKView 側からもらうことでデフォルトを認識できるようにする。
    GLuint m_defaultFBO;
    int m_backengBufferWidth;
    int m_backengBufferHeight;
};

struct GLVertexElement {
    uint32_t streamIndex;

    // 以下は GLShaderPass::getUsageAttributeIndex() に渡して attribute の location を取得する
    VertexElementUsage usage; // 要素の使用法
    int usageIndex;           // 使用法番号

    // 以下は glVertexAttribPointer() に渡す引数
    GLint size;           // データの要素数 (1, 2, 3, 4 のいずれか。Vector3 なら 3 を指定する)
    GLenum type;          // (終端は0)
    GLboolean normalized; // データを正規化するか (0.0 ～ 1.0 にするか。色を 0～255 で指定していて、GLSL では 0.0～1.0 で使いたいときは true)
    GLsizei stride;       // 頂点1つ分のバイト数 (この要素が1つのバイト数ではないので注意)
    size_t byteOffset;    // 先頭からのバイト数

    bool instance = false;
};

class GLVertexDeclaration
    : public IVertexDeclaration {
public:
    GLVertexDeclaration();
    virtual ~GLVertexDeclaration();
    void init(const VertexElement* elements, int elementsCount);
    virtual void onDestroy() override;

    const List<GLVertexElement>& vertexElements() const { return m_vertexElements; }

    const GLVertexElement* findGLVertexElement(kokage::AttributeUsage usage, int usageIndex) const;

    // 頂点宣言から GL 用の頂点宣言を生成する
    static void createGLVertexElements(const VertexElement* vertexElements, int elementsCount, List<GLVertexElement>* outList);

    // 頂点宣言の型から GLVertexElement 用のデータを作る
    static void convertDeclTypeLNToGL(VertexElementType type, GLenum* gl_type, GLint* size, GLboolean* normalized);

private:
    List<GLVertexElement> m_vertexElements;
};

class GLPipeline
    : public IPipeline {
public:
    GLPipeline();
    Result<> init(OpenGLDevice* device, const DevicePipelineStateDesc& state);
    virtual void onDestroy() override;
    void bind(const std::array<RHIResource*, MaxVertexStreams>& vertexBuffers, const RHIResource* indexBuffer, IDescriptor* descriptor);
    GLenum primitiveTopology() const { return m_primitiveTopology; }

    void rebindAttr(int vertexOffset);

private:
    OpenGLDevice* m_device;
    BlendStateDesc m_blendState;
    RasterizerStateDesc m_rasterizerState;
    DepthStencilStateDesc m_depthStencilState;
    GLenum m_primitiveTopology;
};

////==============================================================================
//// empty implementation
//
//class EmptyGLContext
//	: public GLContext
//{
//public:
//	EmptyGLContext() = default;
//	virtual ~EmptyGLContext() = default;
//
//	virtual Ref<GLSwapChain> createSwapChain(PlatformWindow* window, const SizeI& backbufferSize) override;
//	virtual void makeCurrent(GLSwapChain* swapChain) override;
//	virtual void swap(GLSwapChain* swapChain) override;
//};
//
//class EmptyGLSwapChain
//	: public GLSwapChain
//{
//public:
//	EmptyGLSwapChain(OpenGLDevice* device) : GLSwapChain(device) {}
//	virtual ~EmptyGLSwapChain() = default;
//
//private:
//};
//
} // namespace detail
} // namespace ln
