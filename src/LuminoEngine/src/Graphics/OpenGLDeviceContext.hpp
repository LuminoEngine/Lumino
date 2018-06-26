#pragma once

#ifdef LN_GRAPHICS_OPENGLES

#ifdef __APPLE__
#include <unistd.h>
#include <sys/resource.h>
#include <OpenGLES/ES2/gl.h>
#else
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
//#include <GLES3/gl3.h>
//#include <GLES3/gl2ext.h>
#endif

#else
//#include <GL/gl.h>
#endif
#include <glad/glad.h>

#include "GraphicsDeviceContext.hpp"

namespace ln {
namespace detail {
class GLContext;
class GLIndexBuffer;
class GLShaderPass;

/*
	Note:

	OpenGLDeviceContext はマルチスレッドレンダリングをサポートしない。
	大きな理由は、マルチスレッドでの動作がちゃんと規格化されていないから。
	プラットフォーム間での差が激しく、カバーしきれない。

	他、
	- Emscripten で使うときはそもそもスレッドが使えない。
	- GLFW で共有コンテキストを作るときは必ずウィンドウを作る必要がある。

	特に Emscripten で使うときの環境の制約が大きいかも。
	素直にシングルスレッドで動作するデバイスとして位置づけたい。

	マルチスレッドレンダリングやるなら Vulkan、DX12 などを使おう。
*/
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
	virtual void onEnterRenderState() override;
	virtual void onLeaveRenderState() override;
	virtual Ref<ISwapChain> onCreateSwapChain(PlatformWindow* window, const SizeI& backbufferSize) override;
	virtual Ref<IVertexDeclaration> onCreateVertexDeclaration(const VertexElement* elements, int elementsCount) override;
	virtual Ref<IVertexBuffer> onCreateVertexBuffer(GraphicsResourceUsage usage, size_t bufferSize, const void* initialData) override;
	virtual Ref<IIndexBuffer> onCreateIndexBuffer(GraphicsResourceUsage usage, IndexBufferFormat format, int indexCount, const void* initialData) override;
	virtual Ref<IShaderPass> onCreateShaderPass(const byte_t* vsCode, int vsCodeLen, const byte_t* fsCodeLen, int psCodeLen, ShaderCompilationDiag* diag) override;
	virtual void onUpdatePrimitiveData(IVertexDeclaration* decls, IVertexBuffer** vertexBuufers, int vertexBuffersCount, IIndexBuffer* indexBuffer) override;
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
};

class GLSwapChain
	: public ISwapChain
{
public:
	virtual ~GLSwapChain() = default;
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
	EmptyGLSwapChain() = default;
	virtual ~EmptyGLSwapChain() = default;
};

struct GLVertexElement
{
	uint32_t streamIndex;

	// 以下は GLShaderPass::getUsageAttributeIndex() に渡して attribute の location を取得する
	VertexElementUsage	Usage;			// 要素の使用法
	int					UsageIndex;     // 使用法番号

										// 以下は glVertexAttribPointer() に渡す引数
	GLint				Size;			// データの要素数 (1, 2, 3, 4 のいずれか。Vector3 なら 3 を指定する)
	GLenum				Type;			// (終端は0)
	GLboolean			Normalized;		// データを正規化するか (0.0 ～ 1.0 にするか。色を 0～255 で指定していて、GLSL では 0.0～1.0 で使いたいときは true)
	GLsizei				Stride;			// 頂点1つ分のバイト数 (この要素が1つのバイト数ではないので注意)
	size_t				ByteOffset;		// 先頭からのバイト数
};

class GLVertexDeclaration
	: public IVertexDeclaration
{
public:
	GLVertexDeclaration();
	virtual ~GLVertexDeclaration();
	void initialize(const VertexElement* elements, int elementsCount);

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

/*	glMapBuffer は使わない。
*	・OpenGL ES では READ モードでロックできないため。
*		つまり 一度 OpenGL にデータを送ってしまった後、その値を得ることができなくなってしまう。
*		通常の lock の用途なら Write だけでほぼ問題ないが、
*		デバイスロストしたときの復帰ではバックアップから GL の頂点バッファを作り直さなければならず、
*		結局こちら側でずっと握っていた方が色々と都合が良かったりする。
*/
class GLVertexBuffer
	: public IVertexBuffer
{
public:
	GLVertexBuffer();
	virtual ~GLVertexBuffer();
	void initialize(GraphicsResourceUsage usage, size_t bufferSize, const void* initialData);
	GLuint getGLVertexBuffer() const { return m_glVertexBuffer; }

	GLuint vertexBufferId() const { return m_glVertexBuffer; }

	virtual void setSubData(size_t offset, const void* data, size_t length) override;
	virtual void* map(size_t offset, uint32_t length) override;
	virtual void unmap() override;

private:
	GLuint					m_glVertexBuffer;
	//size_t					m_byteCount;
	//byte_t*					m_data;
	GLenum					m_usage;
	GraphicsResourceUsage			m_format;
};

class GLIndexBuffer
	: public IIndexBuffer
{
public:
	GLIndexBuffer();
	virtual ~GLIndexBuffer();

	void initialize(GraphicsResourceUsage usage, IndexBufferFormat format, int indexCount, const void* initialData);
	GLuint indexBufferId() const { return m_indexBufferId; }
	IndexBufferFormat format() const { return m_format; }

	virtual void setSubData(size_t offset, const void* data, size_t length) override;
	virtual void* map(size_t offset, uint32_t length) override;
	virtual void unmap() override;

//public:
//	virtual size_t getByteCount() const { return m_byteCount; }
//	virtual IndexBufferFormat getFormat() const { return m_format; }
//	virtual ResourceUsage getUsage() const { return m_usage; }
//	virtual void setSubData(uint32_t offsetBytes, const void* data, uint32_t dataBytes);
//	virtual void lock(void** lockedBuffer, size_t* lockedSize);
//	virtual void unlock();
//	virtual void onLostDevice();
//	virtual void onResetDevice();
//
private:
	GLuint              m_indexBufferId;
	//size_t				m_byteCount;
	IndexBufferFormat	m_format;
	GLenum		m_usage;
	//bool				m_inited;
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


class GLShaderUniform
	: public IShaderUniform
{
public:
	virtual const ShaderUniformTypeDesc& desc() const { return m_desc; }
	virtual const std::string& name() const { return m_name; }

	void setUniformValue(OpenGLDeviceContext* context, const void* data, size_t size);
	
	ShaderUniformTypeDesc m_desc;
	std::string m_name;
	GLint m_location;
};

class GLShaderPass
	: public IShaderPass
{
public:
	GLShaderPass();
	void initialize(OpenGLDeviceContext* context, const byte_t* vsCode, int vsCodeLen, const byte_t* fsCodeLen, int psCodeLen, ShaderCompilationDiag* diag);
	void dispose();

	GLuint program() const { return m_program; }

	virtual int getUniformCount() const override;
	virtual IShaderUniform* getUniform(int index) const override;
	virtual void setUniformValue(int index, const void* data, size_t size) override;

private:
	void buildUniforms();

	OpenGLDeviceContext* m_context;
	GLuint m_program;
	List<Ref<GLShaderUniform>> m_uniforms;
};

} // namespace detail
} // namespace ln

