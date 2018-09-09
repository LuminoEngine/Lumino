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
 * マルチスレッドスレッドについて
 * ----------
 * OpenGLDeviceContext はマルチスレッドレンダリングをサポートしない。
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

#if defined(LN_GRAPHICS_OPENGLES)
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

#elif defined(LN_EMSCRIPTEN)
#include <emscripten.h>
#define GL_GLEXT_PROTOTYPES
#define EGL_EGLEXT_PROTOTYPES
#include <GL/gl.h>

//#include <GLES2/gl2.h>
//#include <GLES2/gl2ext.h>

#include <GLES3/gl3.h>
//#include <GLES3/gl2ext.h>

#else
#include <glad/glad.h>
#endif

#include "GraphicsDeviceContext.hpp"

namespace ln {
namespace detail {
class GLContext;
class GLSwapChain;
class GLIndexBuffer;
class GLRenderTargetTexture;
class GLShaderPass;
class GLShaderUniformBuffer;
class GLShaderUniform;
class GLLocalShaderSamplerBuffer;

class OpenGLDeviceContext
	: public IGraphicsDeviceContext
{
public:
	struct Settings
	{
		PlatformWindow* mainWindow = nullptr;
	};

	OpenGLDeviceContext();
	virtual ~OpenGLDeviceContext() = default;

	void initialize(const Settings& settings);
	virtual void dispose() override;

	// uniform set の時、Vector4[] → vec2[] に変換するための一時バッファ 
	MemoryStream* uniformTempBuffer() { return &m_uniformTempBuffer; }
	BinaryWriter* uniformTempBufferWriter() { return &m_uniformTempBufferWriter; }

	void setActiveShaderPass(GLShaderPass* pass);

protected:
	virtual void onEnterMainThread() override;
	virtual void onLeaveMainThread() override;
	virtual void onSaveExternalRenderState() override;
	virtual void onRestoreExternalRenderState() override;
	virtual Ref<ISwapChain> onCreateSwapChain(PlatformWindow* window, const SizeI& backbufferSize) override;
	virtual Ref<IVertexDeclaration> onCreateVertexDeclaration(const VertexElement* elements, int elementsCount) override;
	virtual Ref<IVertexBuffer> onCreateVertexBuffer(GraphicsResourceUsage usage, size_t bufferSize, const void* initialData) override;
	virtual Ref<IIndexBuffer> onCreateIndexBuffer(GraphicsResourceUsage usage, IndexBufferFormat format, int indexCount, const void* initialData) override;
	virtual Ref<ITexture> onCreateTexture2D(uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap, const void* initialData) override;
	virtual Ref<ITexture> onCreateRenderTarget(uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap) override;
	virtual Ref<IDepthBuffer> onCreateDepthBuffer(uint32_t width, uint32_t height) override;
	virtual Ref<ISamplerState> onCreateSamplerState(const SamplerStateData& desc) override;
	virtual Ref<IShaderPass> onCreateShaderPass(const byte_t* vsCode, int vsCodeLen, const byte_t* fsCodeLen, int psCodeLen, ShaderCompilationDiag* diag) override;
	virtual void onUpdatePipelineState(const BlendStateDesc& blendState, const RasterizerStateDesc& rasterizerState, const DepthStencilStateDesc& depthStencilState) override;
	virtual void onUpdateFrameBuffers(ITexture** renderTargets, int renderTargetsCount, IDepthBuffer* depthBuffer) override;
	virtual void onUpdatePrimitiveData(IVertexDeclaration* decls, IVertexBuffer** vertexBuufers, int vertexBuffersCount, IIndexBuffer* indexBuffer) override;
	virtual void onUpdateShaderPass(IShaderPass* newPass) override;
	virtual void onClearBuffers(ClearFlags flags, const Color& color, float z, uint8_t stencil) override;
	virtual void onDrawPrimitive(PrimitiveType primitive, int startVertex, int primitiveCount) override;
	virtual void onDrawPrimitiveIndexed(PrimitiveType primitive, int startIndex, int primitiveCount) override;
	virtual void onPresent(ISwapChain* swapChain) override;

private:
	static void getPrimitiveInfo(PrimitiveType primitive, int primitiveCount, GLenum* gl_prim, int* vertexCount);

	Ref<GLContext> m_glContext;
	MemoryStream m_uniformTempBuffer;
	BinaryWriter m_uniformTempBufferWriter;
	GLShaderPass* m_activeShaderPass;
	GLIndexBuffer* m_currentIndexBuffer;
	GLuint m_vao;	// https://www.khronos.org/opengl/wiki/Vertex_Specification#Index_buffers
	GLuint m_fbo;
	//int m_lastUsedAttribIndex;

	struct
	{
		GLboolean state_GL_CULL_FACE;

	} m_savedState;

	struct
	{
		GLint MAX_VERTEX_ATTRIBS = 0;

	} m_caps;
};

class GLContext
	: public RefObject
{
public:
	GLContext() = default;
	virtual ~GLContext() = default;

	virtual Ref<GLSwapChain> createSwapChain(PlatformWindow* window, const SizeI& backbufferSize) = 0;
	virtual void makeCurrent(GLSwapChain* swapChain) = 0;
	virtual void swap(GLSwapChain* swapChain) = 0;
};

class GLSwapChain
	: public ISwapChain
{
public:
	GLSwapChain();
	virtual ~GLSwapChain() = default;
	virtual void dispose() override;
	virtual ITexture* getColorBuffer() const override;
	virtual void getBackendBufferSize(SizeI* outSize) = 0;

	void genBackbuffer(uint32_t width, uint32_t height);
	GLuint fbo() const { return m_fbo; }


private:
	Ref<GLRenderTargetTexture> m_backbuffer;
	GLuint m_fbo;
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
	void initialize(const VertexElement* elements, int elementsCount);
	virtual void dispose() override;

	const List<GLVertexElement>& vertexElements() const { return m_vertexElements; }

	// 頂点宣言から GL 用の頂点宣言を生成する
	static void createGLVertexElements(const VertexElement* vertexElements, int elementsCount, List<GLVertexElement>* outList);

	// 頂点宣言から頂点1つ分のデータサイズ (バイト数) を求める
	static int getVertexSize(const VertexElement* vertexElements, int elementsCount, int streamIndex);

	// 頂点宣言の型のサイズ (バイト数) を求める
	static int getVertexElementTypeSize(VertexElementType type);

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
	void initialize(GraphicsResourceUsage usage, size_t bufferSize, const void* initialData);
	virtual void dispose() override;

	GLuint getGLVertexBuffer() const { return m_glVertexBuffer; }

	GLuint vertexBufferId() const { return m_glVertexBuffer; }

	virtual size_t getBytesSize() override { return m_size; }
	virtual void setSubData(size_t offset, const void* data, size_t length) override;
	virtual void* map() override;
	virtual void unmap() override;

private:
	GLuint m_glVertexBuffer;
	GLenum m_usage;
	GraphicsResourceUsage m_format;
	size_t m_size;
};

class GLIndexBuffer
	: public IIndexBuffer
{
public:
	GLIndexBuffer();
	virtual ~GLIndexBuffer();
	void initialize(GraphicsResourceUsage usage, IndexBufferFormat format, int indexCount, const void* initialData);
	virtual void dispose() override;

	GLuint indexBufferId() const { return m_indexBufferId; }
	IndexBufferFormat format() const { return m_format; }

	virtual size_t getBytesSize() override { return m_size; }
	virtual void setSubData(size_t offset, const void* data, size_t length) override;
	virtual void* map() override;
	virtual void unmap() override;

private:
	GLuint m_indexBufferId;
	IndexBufferFormat m_format;
	GLenum m_usage;
	size_t m_size;
};

class GLTextureBase
	: public ITexture
{
public:
	virtual ~GLTextureBase() = default;

	virtual GLuint id() const = 0;
};


class GLTexture2D
	: public GLTextureBase
{
public:
	GLTexture2D();
	virtual ~GLTexture2D();
	void initialize(uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap, const void* initialData);
	virtual void dispose() override;

	virtual void readData(void* outData) override;
	virtual const SizeI& realSize() override;
	virtual TextureFormat getTextureFormat() const override;
	virtual void setSubData(int x, int y, int width, int height, const void* data, size_t dataSize) override;

	virtual GLuint id() const override { return m_id; }

private:
	GLuint m_id;
	SizeI m_size;
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
	void initialize(uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap);
	virtual void dispose() override;

	virtual GLuint id() const override { return m_id; }

	virtual void readData(void* outData) override;
	virtual const SizeI& realSize() override;
	virtual TextureFormat getTextureFormat() const override;
	virtual void setSubData(int x, int y, int width, int height, const void* data, size_t dataSize) override;


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
//	virtual void setSubData3D(const Box32& box, const void* data, size_t dataBytes);
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
	void initialize(uint32_t width, uint32_t height);
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
	void initialize(const SamplerStateData& desc);
	virtual void dispose() override;

	GLuint id() const { return m_id; }

private:
	GLuint m_id;
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
	virtual ~GLShaderPass() = default;
	void initialize(OpenGLDeviceContext* context, const byte_t* vsCode, int vsCodeLen, const byte_t* fsCodeLen, int psCodeLen, ShaderCompilationDiag* diag);
	virtual void dispose() override;

	GLuint program() const { return m_program; }
	void apply();

	virtual int getUniformCount() const override;
	virtual IShaderUniform* getUniform(int index) const override;
	virtual void setUniformValue(int index, const void* data, size_t size) override;

	virtual int getUniformBufferCount() const override;
	virtual IShaderUniformBuffer* getUniformBuffer(int index) const override;

	virtual IShaderSamplerBuffer* samplerBuffer() const override;

private:
	void buildUniforms();

	OpenGLDeviceContext* m_context;
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
	virtual ~GLShaderUniform() = default;
	virtual void dispose() override;
	virtual const ShaderUniformTypeDesc& desc() const override { return m_desc; }
	virtual const std::string& name() const override { return m_name; }

	void setUniformValue(OpenGLDeviceContext* context, const void* data, size_t size);

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
	void bind();

	virtual int registerCount() const override;
	virtual const std::string& getTextureRegisterName(int registerIndex) const override;
	virtual const std::string& getSamplerRegisterName(int registerIndex) const override;
	virtual void setTexture(int registerIndex, ITexture* texture) override;
	virtual void setSamplerState(int registerIndex, ISamplerState* state) override;

private:
	struct Entry
	{
		std::string textureRegisterName;
		std::string samplerRegisterName;
		GLint uniformLocation = 0;
		ITexture* texture = nullptr;
		GLSamplerState* samplerState = nullptr;
	};

	std::vector<Entry> m_table;
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
	EmptyGLSwapChain(const SizeI& size) : m_size(size) {}
	virtual ~EmptyGLSwapChain() = default;
	virtual void getBackendBufferSize(SizeI* outSize) override { *outSize = m_size; }

private:
	SizeI m_size;
};

} // namespace detail
} // namespace ln

