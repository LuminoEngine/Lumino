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
 */
#pragma once

#ifdef LN_EMSCRIPTEN
#define LN_GRAPHICS_OPENGLES
#endif

#if defined(LN_EMSCRIPTEN)
#include <emscripten.h>
#define GL_GLEXT_PROTOTYPES
#define EGL_EGLEXT_PROTOTYPES
#include <GL/gl.h>

//#include <GLES2/gl2.h>
//#include <GLES2/gl2ext.h>

#include <GLES3/gl3.h>
//#include <GLES3/gl2ext.h>

#elif defined(LN_GRAPHICS_OPENGLES)
#ifdef __APPLE__
#include <unistd.h>
#include <sys/resource.h>
//#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES3/gl.h>
#include <OpenGLES/ES3/glext.h>
#define GL_GLES_PROTOTYPES

#else
//#include <GL/gl.h>
//#include <GLES2/gl2.h>
//#include <GLES2/gl2ext.h>
#include <GLES3/gl3.h>
//#include <GLES3/gl2ext.h>
//#include <glad/glad.h>
#endif


#else
#include <glad/glad.h>
#endif

#include "GraphicsDeviceContext.hpp"

namespace ln {
namespace detail {
class GLGraphicsContext;
class GLContext;
class GLSwapChain;
class GLRenderPass;
class GLCommandQueue;
class GLIndexBuffer;
class GLTextureBase;
class GLRenderTargetTexture;
class GLDepthBuffer;
class GLShaderPass;
class GLShaderUniformBuffer;
class GLShaderUniform;
class GLLocalShaderSamplerBuffer;

class OpenGLDevice
	: public IGraphicsDevice
{
public:
	struct Settings
	{
		PlatformWindow* mainWindow = nullptr;   // nullptr の場合、OpenGL Context の生成・管理を内部で行わない
        uint32_t defaultFramebuffer;
	};
	
	struct Caps
	{
		GLint MAX_VERTEX_ATTRIBS = 0;
		GLint MAX_COLOR_ATTACHMENTS = 0;
		GLfloat MAX_TEXTURE_MAX_ANISOTROPY_EXT = 0;
		bool support_filter_anisotropic = false;
	};

	OpenGLDevice();
	virtual ~OpenGLDevice() = default;

	void init(const Settings& settings);
	virtual void dispose() override;
	const Ref<GLContext>& glContext() const { return m_glContext; }
	const Caps& caps() const { return m_caps; }

	// uniform set の時、Vector4[] → vec2[] に変換するための一時バッファ 
	MemoryStream* uniformTempBuffer() { return &m_uniformTempBuffer; }
	BinaryWriter* uniformTempBufferWriter() { return &m_uniformTempBufferWriter; }


protected:
	virtual INativeGraphicsInterface* getNativeInterface() const { return nullptr; }
	//virtual ICommandList* getGraphicsContext() const;
	virtual void onGetCaps(GraphicsDeviceCaps* outCaps) override;
	virtual Ref<ISwapChain> onCreateSwapChain(PlatformWindow* window, const SizeI& backbufferSize) override;
	virtual Ref<ICommandList> onCreateCommandList() override;
	virtual Ref<IRenderPass> onCreateRenderPass(const DeviceFramebufferState& buffers, ClearFlags clearFlags, const Color& clearColor, float clearDepth, uint8_t clearStencil) override;
	virtual Ref<IPipeline> onCreatePipeline(const DevicePipelineStateDesc& state) override;
	virtual Ref<IVertexDeclaration> onCreateVertexDeclaration(const VertexElement* elements, int elementsCount) override;
	virtual Ref<IVertexBuffer> onCreateVertexBuffer(GraphicsResourceUsage usage, size_t bufferSize, const void* initialData) override;
	virtual Ref<IIndexBuffer> onCreateIndexBuffer(GraphicsResourceUsage usage, IndexBufferFormat format, int indexCount, const void* initialData) override;
	virtual Ref<ITexture> onCreateTexture2D(GraphicsResourceUsage usage, uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap, const void* initialData) override;
	virtual Ref<ITexture> onCreateTexture3D(GraphicsResourceUsage usage, uint32_t width, uint32_t height, uint32_t depth, TextureFormat requestFormat, bool mipmap, const void* initialData) override;
	virtual Ref<ITexture> onCreateRenderTarget(uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap) override;
    virtual Ref<ITexture> onCreateWrappedRenderTarget(intptr_t nativeObject, uint32_t hintWidth, uint32_t hintHeight) override;
    virtual Ref<IDepthBuffer> onCreateDepthBuffer(uint32_t width, uint32_t height) override;
	virtual Ref<ISamplerState> onCreateSamplerState(const SamplerStateData& desc) override;
	virtual Ref<IShaderPass> onCreateShaderPass(const ShaderPassCreateInfo& createInfo, ShaderCompilationDiag* diag) override;
	virtual void onFlushCommandBuffer(ICommandList* context, ITexture* affectRendreTarget) override {}
	virtual ICommandQueue* getGraphicsCommandQueue() override;
	virtual ICommandQueue* getComputeCommandQueue() override;

private:
	Ref<GLContext> m_glContext;
	MemoryStream m_uniformTempBuffer;
	BinaryWriter m_uniformTempBufferWriter;
	//int m_lastUsedAttribIndex;
	Ref<GLCommandQueue> m_graphicsQueue;	// dummy
	//Ref<GLGraphicsContext> m_graphicsContext;
	Caps m_caps;
	//bool m_commandListCreated;
};

class GLGraphicsContext
	: public ICommandList
{
public:
	GLGraphicsContext();
	Result init(OpenGLDevice* owner);
	void dispose();
	void setActiveShaderPass(GLShaderPass* pass);
	GLuint fbo() const { return m_fbo; }

protected:
	virtual void onSaveExternalRenderState() override;
	virtual void onRestoreExternalRenderState() override;
	virtual void onBeginCommandRecoding() override {}
	virtual void onEndCommandRecoding() override {}
	virtual void onBeginRenderPass(IRenderPass* renderPass) override;
	virtual void onEndRenderPass(IRenderPass* renderPass) override;
	virtual void onSubmitStatus(const GraphicsContextState& state, uint32_t stateDirtyFlags, GraphicsContextSubmitSource submitSource, IPipeline* pipeline) override;
	virtual void* onMapResource(IGraphicsResource* resource, uint32_t offset, uint32_t size) override;
	virtual void onUnmapResource(IGraphicsResource* resource) override;
	virtual void onSetSubData(IGraphicsResource* resource, size_t offset, const void* data, size_t length) override;
	virtual void onSetSubData2D(ITexture* resource, int x, int y, int width, int height, const void* data, size_t dataSize) override;
	virtual void onSetSubData3D(ITexture* resource, int x, int y, int z, int width, int height, int depth, const void* data, size_t dataSize) override;
	virtual void onClearBuffers(ClearFlags flags, const Color& color, float z, uint8_t stencil) override;
	virtual void onDrawPrimitive(PrimitiveTopology primitive, int startVertex, int primitiveCount) override;
	virtual void onDrawPrimitiveIndexed(PrimitiveTopology primitive, int startIndex, int primitiveCount) override;
	virtual void onDrawExtension(INativeGraphicsExtension* extension) override { LN_NOTIMPLEMENTED(); }

private:
	static void getPrimitiveInfo(PrimitiveTopology primitive, int primitiveCount, GLenum* gl_prim, int* vertexCount);

	OpenGLDevice* m_device;
	GLuint m_vao;	// https://www.khronos.org/opengl/wiki/Vertex_Specification#Index_buffers
	GLuint m_fbo;
	GLIndexBuffer* m_currentIndexBuffer;
	GLRenderPass* m_currentRenderPass = nullptr;
	GLShaderPass* m_activeShaderPass;

	struct
	{
		GLboolean state_GL_CULL_FACE;

	} m_savedState;
};

class GLContext
	: public RefObject
{
public:
    GLContext();
	virtual ~GLContext() = default;

	virtual Ref<GLSwapChain> createSwapChain(PlatformWindow* window, const SizeI& backbufferSize) = 0;
	virtual void makeCurrent(GLSwapChain* swapChain) = 0;
	virtual void swap(GLSwapChain* swapChain) = 0;

};

class GLSwapChain
	: public ISwapChain
{
public:
	GLSwapChain(OpenGLDevice* device);
	virtual ~GLSwapChain() = default;
	virtual void dispose() override;
	virtual uint32_t getBackbufferCount() override { return 1; }
    virtual void acquireNextImage(int* outImageIndex) override { *outImageIndex = 0; }
    virtual ITexture* getRenderTarget(int imageIndex) const override;
	virtual Result resizeBackbuffer(uint32_t width, uint32_t height) override;
	virtual void present() override;

	virtual void getBackendBufferSize(SizeI* outSize);

	void genBackbuffer(uint32_t width, uint32_t height);
	GLuint fbo() const { return m_fbo; }


    GLuint defaultFBO() const { return m_defaultFBO; }
    void setDefaultFBO(GLuint id) { m_defaultFBO = id; }

    void setBackendBufferSize(int width, int height);

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

// 他の Graphics API との処理を抽象化するためのダミー。
class GLCommandQueue
	: public ICommandQueue
{
public:
	GLCommandQueue();
	Result init();
	virtual Result submit(ICommandList* commandList) override;
};

class GLRenderPass
	: public IRenderPass
{
public:
	GLRenderPass();
	Result init(OpenGLDevice* device, const DeviceFramebufferState& buffers, ClearFlags clearFlags, const Color& clearColor, float clearDepth, uint8_t clearStencil);
	virtual void dispose() override;

	SizeI viewSize() const;
	void bind(GLGraphicsContext* context);

private:
	OpenGLDevice* m_device;
	std::array<Ref<GLTextureBase>, MaxMultiRenderTargets> m_renderTargets;
	Ref<GLDepthBuffer> m_depthBuffer;
	ClearFlags m_clearFlags;
	Color m_clearColor;
	float m_clearDepth;
	uint8_t m_clearStencil;
};

struct GLVertexElement
{
	uint32_t streamIndex;

	// 以下は GLShaderPass::getUsageAttributeIndex() に渡して attribute の location を取得する
	VertexElementUsage	usage;			// 要素の使用法
	int					usageIndex;     // 使用法番号

	// 以下は glVertexAttribPointer() に渡す引数
	GLint				size;			// データの要素数 (1, 2, 3, 4 のいずれか。Vector3 なら 3 を指定する)
	GLenum				type;			// (終端は0)
	GLboolean			normalized;		// データを正規化するか (0.0 ～ 1.0 にするか。色を 0～255 で指定していて、GLSL では 0.0～1.0 で使いたいときは true)
	GLsizei				stride;			// 頂点1つ分のバイト数 (この要素が1つのバイト数ではないので注意)
	size_t				byteOffset;		// 先頭からのバイト数
};

class GLVertexDeclaration
	: public IVertexDeclaration
{
public:
	GLVertexDeclaration();
	virtual ~GLVertexDeclaration();
	void init(const VertexElement* elements, int elementsCount);
	virtual void dispose() override;

	const List<GLVertexElement>& vertexElements() const { return m_vertexElements; }

	// 頂点宣言から GL 用の頂点宣言を生成する
	static void createGLVertexElements(const VertexElement* vertexElements, int elementsCount, List<GLVertexElement>* outList);

    // 頂点宣言の型から GLVertexElement 用のデータを作る
	static void convertDeclTypeLNToGL(VertexElementType type, GLenum* gl_type, GLint* size, GLboolean* normalized);

private:
	List<GLVertexElement>	m_vertexElements;
};

class GLVertexBuffer
	: public IVertexBuffer
{
public:
	GLVertexBuffer();
	virtual ~GLVertexBuffer();
	void init(GraphicsResourceUsage usage, size_t bufferSize, const void* initialData);
	virtual void dispose() override;

	GLuint getGLVertexBuffer() const { return m_glVertexBuffer; }

	GLuint vertexBufferId() const { return m_glVertexBuffer; }

	virtual size_t getBytesSize() override { return m_size; }
	virtual GraphicsResourceUsage usage() const override { return m_usage; }

	void setSubData(size_t offset, const void* data, size_t length);
	void* map(uint32_t offset, uint32_t size);
	virtual void* map() override { return map(0, m_size); }
	virtual void unmap() override;

private:
	GLuint m_glVertexBuffer;
	GraphicsResourceUsage m_usage;
	//GraphicsResourceUsage m_format;
	size_t m_size;
};

class GLIndexBuffer
	: public IIndexBuffer
{
public:
	GLIndexBuffer();
	virtual ~GLIndexBuffer();
	void init(GraphicsResourceUsage usage, IndexBufferFormat format, int indexCount, const void* initialData);
	virtual void dispose() override;

	GLuint indexBufferId() const { return m_indexBufferId; }
	IndexBufferFormat format() const { return m_format; }

	virtual size_t getBytesSize() override { return m_size; }
	virtual GraphicsResourceUsage usage() const override { return m_usage; }

	void setSubData(size_t offset, const void* data, size_t length);
	void* map(uint32_t offset, uint32_t size);
	virtual void* map() override { return map(0, m_size); }
	virtual void unmap() override;

private:
	GLuint m_indexBufferId;
	IndexBufferFormat m_format;
	GraphicsResourceUsage m_usage;
	size_t m_size;
};

class GLTextureBase
	: public ITexture
{
public:
	virtual ~GLTextureBase() = default;

	virtual GLuint id() const = 0;
    virtual bool mipmap() const = 0;
    virtual void setSubData(int x, int y, int width, int height, const void* data, size_t dataSize) = 0;
    virtual void setSubData3D(int x, int y, int z, int width, int height, int depth, const void* data, size_t dataSize) = 0;
};


class GLTexture2D
	: public GLTextureBase
{
public:
	GLTexture2D();
	virtual ~GLTexture2D();
	void init(GraphicsResourceUsage usage, uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap, const void* initialData);
	virtual void dispose() override;

	virtual DeviceTextureType type() const override { return DeviceTextureType::Texture2D; }
	virtual void readData(void* outData) override;
	virtual SizeI realSize() override;
	virtual TextureFormat getTextureFormat() const override;
	virtual GraphicsResourceUsage usage() const override { return m_usage; }
	virtual void setSubData(int x, int y, int width, int height, const void* data, size_t dataSize) override;
	virtual void setSubData3D(int x, int y, int z, int width, int height, int depth, const void* data, size_t dataSize) override;

	virtual GLuint id() const override { return m_id; }
    virtual bool mipmap() const override  { return m_mipmap; }

private:
	GLuint m_id;
	GraphicsResourceUsage m_usage;
	SizeI m_size;
	TextureFormat m_textureFormat;
	GLenum m_pixelFormat;
	GLenum m_elementType;
    bool m_mipmap;
};

class GLTexture3D
	: public GLTextureBase
{
public:
	GLTexture3D();
	virtual ~GLTexture3D();
	void init(GraphicsResourceUsage usage, uint32_t width, uint32_t height, uint32_t depth, TextureFormat requestFormat, bool mipmap, const void* initialData);
	virtual void dispose() override;

	virtual DeviceTextureType type() const override { return DeviceTextureType::Texture3D; }
	virtual void readData(void* outData) override;
	virtual SizeI realSize() override;
	virtual TextureFormat getTextureFormat() const override;
	virtual GraphicsResourceUsage usage() const override { return m_usage; }
	virtual void setSubData(int x, int y, int width, int height, const void* data, size_t dataSize) override;
	virtual void setSubData3D(int x, int y, int z, int width, int height, int depth, const void* data, size_t dataSize) override;

	virtual GLuint id() const override { return m_id; }
    virtual bool mipmap() const override { return false; }

private:
	GLuint m_id;
	GraphicsResourceUsage m_usage;
	uint32_t m_width;
	uint32_t m_height;
	uint32_t m_depth;
	TextureFormat m_textureFormat;
	GLenum m_pixelFormat;
	GLenum m_elementType;
};

class GLRenderTargetTexture
	: public GLTextureBase
{
public:
	GLRenderTargetTexture();
	virtual ~GLRenderTargetTexture();
	void init(uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap);
    void init(intptr_t nativeObject, uint32_t hintWidth, uint32_t hintHeight);
	virtual void dispose() override;


	virtual DeviceTextureType type() const override { return DeviceTextureType::RenderTarget; }
	virtual void readData(void* outData) override;
	virtual SizeI realSize() override;
	virtual TextureFormat getTextureFormat() const override;
	virtual GraphicsResourceUsage usage() const override { return GraphicsResourceUsage::Static; }
	virtual void setSubData(int x, int y, int width, int height, const void* data, size_t dataSize) override;
	virtual void setSubData3D(int x, int y, int z, int width, int height, int depth, const void* data, size_t dataSize) override;

    virtual GLuint id() const override { return m_id; }
    virtual bool mipmap() const override { return false; }

private:
	SizeI m_size;
	TextureFormat m_textureFormat;
	GLuint m_id;
	GLenum m_pixelFormat;
	GLenum m_elementType;

//public:
//	// override IDeviceObject
//	virtual void onLostDevice();
//	virtual void onResetDevice();
//
//	// override ITexture
//	virtual TextureType getTextureType() const { return TextureType_RenderTarget; }
//	virtual TextureFormat getTextureFormat() const { return m_format; }
//	virtual const SizeI& getSize() const { return m_size; }
//	virtual const SizeI& getRealSize() const { return m_realSize; }
//	virtual void setSamplerState(const SamplerState& state) { LN_UNREACHABLE(); }
//	virtual void setSubData(const PointI& point, const void* data, size_t dataBytes, const SizeI& dataBitmapSize) { LN_UNREACHABLE(); }
//	virtual void setSubData3D(const BoxI& box, const void* data, size_t dataBytes);
//	virtual void getData(const RectI& rect, void* outData) override;
//	virtual RawBitmap* lock();
//	virtual void unlock();
//
//	// override GLTextureBase
//	virtual GLuint getGLTexture() { return m_glTexture; }
//
//private:
//	GLuint				m_glTexture;
//	TextureFormat		m_format;
//	SizeI				m_size;
//	SizeI				m_realSize;
//	int					m_mipLevels;
//	GLenum				m_pixelFormat;
//	GLenum				m_elementType;
//	RawBitmap*	m_lockingBitmap;
};

class GLDepthBuffer
	: public IDepthBuffer
{
public:
	GLDepthBuffer();
	void init(uint32_t width, uint32_t height);
	virtual void dispose() override;

	GLuint id() const { return m_id; }

private:
	GLuint m_id;
};

class GLSamplerState
	: public ISamplerState
{
public:
	GLSamplerState();
	virtual ~GLSamplerState();
	void init(OpenGLDevice* device, const SamplerStateData& desc);
	virtual void dispose() override;

    GLuint resolveId(bool mipmap) const { return (mipmap) ? m_idMip : m_id; }
	//GLuint id2() const { return m_id; }

private:
	GLuint m_id;
    GLuint m_idMip;
    SamplerStateData m_desc;
};

class GLSLShader
{
public:
	GLSLShader();
	~GLSLShader();
	bool create(const byte_t* code, int length, GLenum type, ShaderCompilationDiag* diag);
	void dispose();

	GLuint shader() const { return m_shader; }
	GLenum type() const { return m_type; }

private:
	GLuint m_shader;
	GLenum m_type;
};

class GLShaderPass
	: public IShaderPass
{
public:
	GLShaderPass();
    virtual ~GLShaderPass();
	void init(OpenGLDevice* context, const byte_t* vsCode, int vsCodeLen, const byte_t* fsCodeLen, int psCodeLen, ShaderCompilationDiag* diag);
	virtual void dispose() override;

	GLuint program() const { return m_program; }
	void apply() const;

	//virtual int getUniformCount() const override;
	//virtual IShaderUniform* getUniform(int index) const override;
	//virtual void setUniformValue(int index, const void* data, size_t size) override;

	virtual int getUniformBufferCount() const override;
	virtual IShaderUniformBuffer* getUniformBuffer(int index) const override;

	virtual IShaderSamplerBuffer* samplerBuffer() const override;

private:
	void buildUniforms();

	OpenGLDevice* m_context;
	GLuint m_program;
	List<Ref<GLShaderUniformBuffer>> m_uniformBuffers;
	List<Ref<GLShaderUniform>> m_uniforms;
	Ref<GLLocalShaderSamplerBuffer> m_samplerBuffer;

};

class GLShaderUniformBuffer
	: public IShaderUniformBuffer
{
public:
	GLShaderUniformBuffer(const GLchar* blockName, GLuint blockIndex, GLint blockSize, GLuint bindingPoint);
	virtual ~GLShaderUniformBuffer();
	virtual void dispose() override;
	void addUniform(GLShaderUniform* uniform) { m_uniforms.add(uniform); }
	void bind(GLuint program);

	virtual const std::string& name() const;
	virtual int getUniformCount() const;
	virtual IShaderUniform* getUniform(int index) const;
	virtual size_t bufferSize() const { return m_blockSize; }
	virtual void setData(const void* data, size_t size);

private:
	std::string m_name;
	GLuint m_blockIndex;
	GLint m_blockSize;
	GLuint m_bindingPoint;
	List<Ref<GLShaderUniform>> m_uniforms;
	GLuint m_ubo;
};

class GLShaderUniform
	: public IShaderUniform
{
public:
	GLShaderUniform(const ShaderUniformTypeDesc& desc, const GLchar* name, GLint location);
    virtual ~GLShaderUniform();
	virtual void dispose() override;
	virtual const ShaderUniformTypeDesc& desc() const override { return m_desc; }
	virtual const std::string& name() const override { return m_name; }

	void setUniformValue(OpenGLDevice* context, const void* data, size_t size);

	//GLint offsetOnBuffer() const {}
	//GLint sizeOnBuffer() const;

private:
	ShaderUniformTypeDesc m_desc;
	std::string m_name;
	GLint m_location;	// [obsolete]

	
};

// 変数名から独自のテーブルを構築する
class GLLocalShaderSamplerBuffer
	: public IShaderSamplerBuffer
{
public:
	GLLocalShaderSamplerBuffer();
	virtual ~GLLocalShaderSamplerBuffer() = default;
	void addGlslSamplerUniform(const std::string& name, GLint uniformLocation);
    void addIsRenderTargetUniform(const std::string& name, GLint uniformLocation);
	void bind();

	virtual int registerCount() const override;
	virtual const std::string& getTextureRegisterName(int registerIndex) const override;
	//virtual const std::string& getSamplerRegisterName(int registerIndex) const override;
	virtual void setTexture(int registerIndex, ITexture* texture) override;
	virtual void setSamplerState(int registerIndex, ISamplerState* state) override;

private:
	// 外部に公開する Uniform 情報。
	// lnCISlnTOg_texture1lnSOg_samplerState1 は、g_texture1 と g_samplerState1 の２つの uniform であるかのように公開する。
	struct ExternalUnifrom
	{
		std::string name;
		GLTextureBase* texture = nullptr;
		GLSamplerState* samplerState = nullptr;
	};

	// 内部的な Uniform 情報。
	// 実際の GLSL の Uniform と一致する。
	struct Uniform
	{
		std::string name;	// lnCISlnTOg_texture1lnSOg_samplerState1 のような FullName
		//std::string samplerRegisterName;
		GLint uniformLocation = -1;
        GLint isRenderTargetUniformLocation = -1;	// texture または sampler の場合、それが RenderTarget であるかを示す値を入力する Uniform の Loc。末尾が lnIsRT になっているもの。
		int m_textureExternalUnifromIndex = -1;
		int m_samplerExternalUnifromIndex = -1;
		//ITexture* texture = nullptr;
		//GLSamplerState* samplerState = nullptr;
	};


	std::vector<Uniform> m_table;
	std::vector<ExternalUnifrom> m_externalUniforms;    // TODO: 名前、virtual のほうがいいかも
};

class GLPipeline
	: public IPipeline
{
public:
	GLPipeline();
	Result init(OpenGLDevice* device, const DevicePipelineStateDesc& state);
	virtual void dispose() override;
	void bind(const std::array<IVertexBuffer*, MaxVertexStreams>& vertexBuffers, const IIndexBuffer* indexBuffer);
	GLenum primitiveTopology() const { return m_primitiveTopology; }

private:
	OpenGLDevice* m_device;
	BlendStateDesc m_blendState;
	RasterizerStateDesc m_rasterizerState;
	DepthStencilStateDesc m_depthStencilState;
	GLenum m_primitiveTopology;
};

//=============================================================================
// empty implementation

class EmptyGLContext
	: public GLContext
{
public:
	EmptyGLContext() = default;
	virtual ~EmptyGLContext() = default;

	virtual Ref<GLSwapChain> createSwapChain(PlatformWindow* window, const SizeI& backbufferSize) override;
	virtual void makeCurrent(GLSwapChain* swapChain) override;
	virtual void swap(GLSwapChain* swapChain) override;
};

class EmptyGLSwapChain
	: public GLSwapChain
{
public:
	EmptyGLSwapChain(OpenGLDevice* device) : GLSwapChain(device) {}
	virtual ~EmptyGLSwapChain() = default;

private:
};

} // namespace detail
} // namespace ln

