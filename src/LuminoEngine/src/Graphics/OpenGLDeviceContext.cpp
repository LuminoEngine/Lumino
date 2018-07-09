
#include "Internal.hpp"
#include "OpenGLDeviceContext.hpp"

#include "GLFWContext.hpp"

#define LN_MACRO_BLOCK_BEGIN for(;;) {
#define LN_MACRO_BLOCK_END break; }

#define GL_CHECK(call) \
	LN_MACRO_BLOCK_BEGIN \
		call; \
		GLenum gl_err = glGetError(); \
		LN_ENSURE(0 == gl_err, #call "; GL error 0x%x: %s", gl_err, OpenGLHelper::glEnumName(gl_err) ); \
	LN_MACRO_BLOCK_END

#ifdef LN_GRAPHICS_OPENGLES
#define glClearDepth glClearDepthf
#endif

namespace ln {
namespace detail {

class OpenGLHelper
{
public:
	static const char* glEnumName(GLenum _enum)
	{
#define GLENUM(_ty) case _ty: return #_ty

		switch (_enum)
		{
			GLENUM(GL_TEXTURE);
			//GLENUM(GL_RENDERBUFFER);

			GLENUM(GL_INVALID_ENUM);
			//GLENUM(GL_INVALID_FRAMEBUFFER_OPERATION);
			GLENUM(GL_INVALID_VALUE);
			GLENUM(GL_INVALID_OPERATION);
			GLENUM(GL_OUT_OF_MEMORY);

			//GLENUM(GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT);
			//GLENUM(GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT);
			//			GLENUM(GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER);
			//			GLENUM(GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER);
			//GLENUM(GL_FRAMEBUFFER_UNSUPPORTED);
		}

#undef GLENUM
		return "<GLenum?>";
	}

	static void convertVariableTypeGLToLN(
		const char* name, GLenum gl_type, GLsizei gl_var_size,
		ShaderRefrectionParameterType* outType, int* outRows, int* outColumns, int* outElements)
	{
		*outElements = 0;

#define SET_LNDESC( c_, t_, row_, col_ ) { *outType = (t_); *outRows = (row_); *outColumns = (col_); }
		switch (gl_type)
		{
			case GL_FLOAT:      SET_LNDESC(LN_SVC_SCALAR, ShaderRefrectionParameterType::Float, 1, 1); break;
			case GL_FLOAT_VEC2: SET_LNDESC(LN_SVC_VECTOR, ShaderRefrectionParameterType::Vector, 1, 2); break;
			case GL_FLOAT_VEC3: SET_LNDESC(LN_SVC_VECTOR, ShaderRefrectionParameterType::Vector, 1, 3); break;
			case GL_FLOAT_VEC4: SET_LNDESC(LN_SVC_VECTOR, ShaderRefrectionParameterType::Vector, 1, 4); break;

			case GL_INT:        SET_LNDESC(LN_SVC_SCALAR, ShaderRefrectionParameterType::Int, 1, 1); break;
			//case GL_INT_VEC2:   SET_LNDESC(LN_SVC_VECTOR, ShaderRefrectionParameterType::IntVector, 1, 2); break;
			//case GL_INT_VEC3:   SET_LNDESC(LN_SVC_VECTOR, ShaderRefrectionParameterType::IntVector, 1, 3); break;
			//case GL_INT_VEC4:   SET_LNDESC(LN_SVC_VECTOR, ShaderRefrectionParameterType::IntVector, 1, 4); break;

			case GL_BOOL:        SET_LNDESC(LN_SVC_SCALAR, ShaderRefrectionParameterType::Bool, 1, 1); break;
			//case GL_BOOL_VEC2:   SET_LNDESC(LN_SVC_VECTOR, ShaderRefrectionParameterType::BoolVector, 1, 2); break;
			//case GL_BOOL_VEC3:   SET_LNDESC(LN_SVC_VECTOR, ShaderRefrectionParameterType::BoolVector, 1, 3); break;
			//case GL_BOOL_VEC4:   SET_LNDESC(LN_SVC_VECTOR, ShaderRefrectionParameterType::BoolVector, 1, 4); break;

			case GL_FLOAT_MAT2:     SET_LNDESC(LN_SVC_MATRIX, ShaderRefrectionParameterType::Matrix, 2, 2); break;
			case GL_FLOAT_MAT3:     SET_LNDESC(LN_SVC_MATRIX, ShaderRefrectionParameterType::Matrix, 3, 3); break;
			case GL_FLOAT_MAT4:     SET_LNDESC(LN_SVC_MATRIX, ShaderRefrectionParameterType::Matrix, 4, 4); break;

			case GL_SAMPLER_2D:         SET_LNDESC(LN_SVC_SAMPLER, ShaderRefrectionParameterType::Texture, 1, 1); break;
			case GL_SAMPLER_CUBE:       SET_LNDESC(LN_SVC_SAMPLER, ShaderRefrectionParameterType::Texture, 1, 1); break;

			//#if !defined(LNOTE_GLES)
			case GL_FLOAT_MAT2x3:   SET_LNDESC(LN_SVC_MATRIX, ShaderRefrectionParameterType::Matrix, 2, 3); break;
			case GL_FLOAT_MAT2x4:   SET_LNDESC(LN_SVC_MATRIX, ShaderRefrectionParameterType::Matrix, 2, 4); break;
			case GL_FLOAT_MAT3x2:   SET_LNDESC(LN_SVC_MATRIX, ShaderRefrectionParameterType::Matrix, 3, 2); break;
			case GL_FLOAT_MAT3x4:   SET_LNDESC(LN_SVC_MATRIX, ShaderRefrectionParameterType::Matrix, 3, 4); break;
			case GL_FLOAT_MAT4x2:   SET_LNDESC(LN_SVC_MATRIX, ShaderRefrectionParameterType::Matrix, 4, 2); break;
			case GL_FLOAT_MAT4x3:   SET_LNDESC(LN_SVC_MATRIX, ShaderRefrectionParameterType::Matrix, 4, 3); break;

			//case GL_FLOAT:      SET_LNDESC( LN_SVC_SCALAR, LN_SVT_FLOAT, 1, 1 ); break;
			//case GL_FLOAT_VEC2: SET_LNDESC( LN_SVC_VECTOR, LN_SVT_FLOAT, 1, 2 ); break;
			//case GL_FLOAT_VEC3: SET_LNDESC( LN_SVC_VECTOR, LN_SVT_FLOAT, 1, 3 ); break;
			//case GL_FLOAT_VEC4: SET_LNDESC( LN_SVC_VECTOR, LN_SVT_FLOAT, 1, 4 ); break;

			case GL_SAMPLER_1D:         SET_LNDESC(LN_SVC_SAMPLER, ShaderRefrectionParameterType::Texture, 1, 1); break;
			case GL_SAMPLER_3D:         SET_LNDESC(LN_SVC_SAMPLER, ShaderRefrectionParameterType::Texture, 1, 1); break;

			case GL_SAMPLER_1D_SHADOW:  SET_LNDESC(LN_SVC_SAMPLER, ShaderRefrectionParameterType::Unknown, 1, 1); break;
			case GL_SAMPLER_2D_SHADOW:  SET_LNDESC(LN_SVC_SAMPLER, ShaderRefrectionParameterType::Unknown, 1, 1); break;
				//#endif
			default: SET_LNDESC(LN_SVC_SAMPLER, ShaderRefrectionParameterType::Unknown, 0, 0); break;
		}

		// check array type (e.g. "list[0]")
		if (StringHelper::indexOf(name, -1, "[", 1) >= 0)
		{
			*outElements = gl_var_size;

			//switch (*outType)
			//{
			//case ShaderVariableType::Bool:
			//	desc->Type = ShaderVariableType::BoolArray;
			//	break;
			//case ShaderVariableType::Float:
			//	desc->Type = ShaderVariableType::FloatArray;
			//	break;
			//case ShaderVariableType::Vector:
			//	desc->Type = ShaderVariableType::VectorArray;
			//	break;
			//case ShaderVariableType::Matrix:
			//	desc->Type = ShaderVariableType::MatrixArray;
			//	break;
			//default:
			//	LN_UNREACHABLE();
			//	break;
			//}
		}
	}

	static void getGLTextureFormat(TextureFormat format, GLenum* internalFormat, GLenum* pixelFormat, GLenum* elementType)
	{
		// http://angra.blog31.fc2.com/blog-entry-11.html
		static GLenum table[][3] =
		{
			// internalFormat,		pixelFormat,		elementType
			{ GL_NONE,				GL_NONE,			GL_NONE },			// TextureFormat::Unknown
			{ GL_RGBA8,				GL_RGBA,			GL_UNSIGNED_BYTE },	// TextureFormat::R8G8B8A8,            ///< 32 ビットのアルファ付きフォーマット (uint32_t アクセス時の表現。lnByte[4] にすると、ABGR)
			{ GL_RGBA8,				GL_RGBA,			GL_UNSIGNED_BYTE },	// TextureFormat::R8G8B8X8,	※元々 GL_RGB だったが、それだと glGetTexImage で強制終了
			{ GL_RGBA16F,			GL_RGBA,			GL_HALF_FLOAT },	// TextureFormat::A16B16G16R16F,       ///< 64 ビットの浮動小数点フォーマット
			{ GL_RGBA32F,			GL_RGBA,			GL_FLOAT },			// TextureFormat::A32B32G32R32F,       ///< 128 ビットの浮動小数点フォーマット
			{ GL_R16F,				GL_RED,				GL_HALF_FLOAT },	// TextureFormat::R16F,
			{ GL_R32UI,				GL_RED,				GL_FLOAT },			// TextureFormat::R32F,
																			//{ GL_R32UI,				GL_RED_INTEGER,		GL_INT },			// TextureFormat::R32_UInt,
			//{ GL_DEPTH_COMPONENT24, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE }, // TextureFormat::D24S8,               ///< 32 ビットの Z バッファフォーマット
		};
		*internalFormat = table[(int)format][0];
		*pixelFormat = table[(int)format][1];
		*elementType = table[(int)format][2];
	}
};


//=============================================================================
// OpenGLDeviceContext

OpenGLDeviceContext::OpenGLDeviceContext()
	: m_glContext(nullptr)
	, m_uniformTempBuffer()
	, m_uniformTempBufferWriter(&m_uniformTempBuffer)
	, m_activeShaderPass(nullptr)
	, m_currentIndexBuffer(nullptr)
	, m_vao(0)
	, m_fbo(0)
{
}

void OpenGLDeviceContext::initialize(const Settings& settings)
{
#ifdef LN_GLFW
	auto glfwContext = makeRef<GLFWContext>();
	glfwContext->initialize(settings.mainWindow);
	m_glContext = glfwContext;
#endif
	if (!m_glContext)
	{
		auto glfwContext = makeRef<EmptyGLContext>();
		m_glContext = glfwContext;
	}

	int result = gladLoadGL();
	if (LN_ENSURE(result, "Failed gladLoadGL()")) return;

	LN_LOG_INFO << "OpenGL " << GLVersion.major << "." << GLVersion.minor;

	GL_CHECK(glGenVertexArrays(1, &m_vao));
	GL_CHECK(glGenFramebuffers(1, &m_fbo));
}

void OpenGLDeviceContext::dispose()
{
	if (m_vao)
	{
		GL_CHECK(glBindVertexArray(0));
		GL_CHECK(glDeleteVertexArrays(1, &m_vao));
		m_vao = 0;
	}
	if (m_fbo != 0)
	{
		GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, 0));
		GL_CHECK(glDeleteFramebuffers(1, &m_fbo));
		m_fbo = 0;
	}
}

void OpenGLDeviceContext::setActiveShaderPass(GLShaderPass* pass)
{
	if (m_activeShaderPass != pass) {
		m_activeShaderPass = pass;
		::glUseProgram((m_activeShaderPass) ? m_activeShaderPass->program() : 0);
	}
}

void OpenGLDeviceContext::onEnterMainThread()
{
	//m_glContext->makeCurrent();
}

void OpenGLDeviceContext::onLeaveMainThread()
{
	//m_glContext->makeCurrent();
	setActiveShaderPass(nullptr);
}

void OpenGLDeviceContext::onSaveExternalRenderState()
{
	//glDisable(GL_CULL_FACE);


	glEnable(GL_CULL_FACE);

	GLboolean ff;
	GL_CHECK(glGetBooleanv(GL_CULL_FACE, &ff));


	//glEnable();
}

void OpenGLDeviceContext::onRestoreExternalRenderState()
{
	GL_CHECK(glBindVertexArray(0));
}

Ref<ISwapChain> OpenGLDeviceContext::onCreateSwapChain(PlatformWindow* window, const SizeI& backbufferSize)
{
	return m_glContext->createSwapChain(window, backbufferSize);
}

Ref<IVertexDeclaration> OpenGLDeviceContext::onCreateVertexDeclaration(const VertexElement* elements, int elementsCount)
{
	auto ptr = makeRef<GLVertexDeclaration>();
	ptr->initialize(elements, elementsCount);
	return ptr;
}

Ref<IVertexBuffer> OpenGLDeviceContext::onCreateVertexBuffer(GraphicsResourceUsage usage, size_t bufferSize, const void* initialData)
{
	auto ptr = makeRef<GLVertexBuffer>();
	ptr->initialize(usage, bufferSize, initialData);
	return ptr;
}

Ref<IIndexBuffer> OpenGLDeviceContext::onCreateIndexBuffer(GraphicsResourceUsage usage, IndexBufferFormat format, int indexCount, const void* initialData)
{
	auto ptr = makeRef<GLIndexBuffer>();
	ptr->initialize(usage, format, indexCount, initialData);
	return ptr;
}

Ref<ITexture> OpenGLDeviceContext::onCreateRenderTarget(uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap)
{
	auto ptr = makeRef<GLRenderTargetTexture>();
	ptr->initialize(width, height, requestFormat, mipmap);
	return ptr;
}

Ref<IShaderPass> OpenGLDeviceContext::onCreateShaderPass(const byte_t* vsCode, int vsCodeLen, const byte_t* psCode, int psCodeLen, ShaderCompilationDiag* diag)
{
	auto ptr = makeRef<GLShaderPass>();
	ptr->initialize(this, vsCode, vsCodeLen, psCode, psCodeLen, diag);
	return ptr;
}

void OpenGLDeviceContext::onUpdateFrameBuffers(ITexture** renderTargets, int renderTargetsCount, IDepthBuffer* depthBuffer)
{
	GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, m_fbo));

	// color buffers
	for (int i = 0; i < renderTargetsCount; ++i)
	{
		if (renderTargets[i])
		{
			GLuint id = static_cast<GLTextureBase*>(renderTargets[i])->id();
			GL_CHECK(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, id, 0));
		}
		else
		{
			GL_CHECK(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, 0, 0));
		}
	}

	// depth buffer
	if (depthBuffer)
	{
		GLuint id = static_cast<GLDepthBuffer*>(depthBuffer)->id();
		GL_CHECK(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, id));
		GL_CHECK(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, id));
	}
	else
	{
		GL_CHECK(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, 0));
		GL_CHECK(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, 0));
	}

	LN_ENSURE(GL_FRAMEBUFFER_COMPLETE == glCheckFramebufferStatus(GL_FRAMEBUFFER),
		"glCheckFramebufferStatus failed 0x%08x",
		glCheckFramebufferStatus(GL_FRAMEBUFFER));
}

void OpenGLDeviceContext::onUpdatePrimitiveData(IVertexDeclaration* decls, IVertexBuffer** vertexBuufers, int vertexBuffersCount, IIndexBuffer* indexBuffer)
{
	//if (LN_REQUIRE(decls)) return;
	//if (LN_REQUIRE(vertexBuufers)) return;
	//if (LN_REQUIRE(vertexBuffersCount >= 1)) return;

	// 複数の頂点バッファを使う
	// https://qiita.com/y_UM4/items/75941cb75afb0a46aa5e
	
	// IVertexDeclaration で指定された頂点レイアウトと、GLSL に書かれている attribute 変数の定義順序が一致していることを前提としている。
	// ※0.4.0 以前は変数名を固定していたが、それを廃止。リフレクションっぽいことをこのモジュールの中でやりたくない。複雑になりすぎる。

	GL_CHECK(glBindVertexArray(m_vao));

	if (decls && vertexBuufers)
	{
		GLVertexDeclaration* glDecl = static_cast<GLVertexDeclaration*>(decls);
		for (int iElement = 0; iElement < glDecl->vertexElements().size(); iElement++)
		{
			const GLVertexElement& element = glDecl->vertexElements()[iElement];

			GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, static_cast<GLVertexBuffer*>(vertexBuufers[element.streamIndex])->vertexBufferId()));
			GL_CHECK(glEnableVertexAttribArray(iElement));
			GL_CHECK(glVertexAttribPointer(iElement, element.size, element.type, element.normalized, element.stride, (void*)(element.byteOffset)));
		}
	}

	m_currentIndexBuffer = static_cast<GLIndexBuffer*>(indexBuffer);
	if (m_currentIndexBuffer)
	{
		GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_currentIndexBuffer->indexBufferId()));
	}
	else
	{
		GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	}

}

void OpenGLDeviceContext::onClearBuffers(ClearFlags flags, const Color& color, float z, uint8_t stencil)
{
	GLuint glflags = 0;

	if (testFlag(flags, ClearFlags::Color))
	{
		GL_CHECK(glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE));
		GL_CHECK(glClearColor(color.r, color.g, color.b, color.a));
		glflags |= GL_COLOR_BUFFER_BIT;
	}

	if (testFlag(flags, ClearFlags::Depth))
	{
		GL_CHECK(glDepthMask(GL_TRUE));
		GL_CHECK(glClearDepth(z););
		glflags |= GL_DEPTH_BUFFER_BIT;
	}

	if (testFlag(flags, ClearFlags::Stencil))
	{
		GL_CHECK(glClearStencil(stencil));
		glflags |= GL_STENCIL_BUFFER_BIT;
	}

	GL_CHECK(glClear(glflags));
}

void OpenGLDeviceContext::onDrawPrimitive(PrimitiveType primitive, int startVertex, int primitiveCount)
{
	GLenum gl_prim;
	int vertexCount;
	getPrimitiveInfo(primitive, primitiveCount, &gl_prim, &vertexCount);

	GL_CHECK(glDrawArrays(gl_prim, startVertex, vertexCount));
}

void OpenGLDeviceContext::onDrawPrimitiveIndexed(PrimitiveType primitive, int startIndex, int primitiveCount)
{
	GLenum gl_prim;
	int vertexCount;
	getPrimitiveInfo(primitive, primitiveCount, &gl_prim, &vertexCount);

	// 引数 start end には、本来であれば0～vertexCountまでのインデックスの中の最大、最小の値を渡す。
	// http://wiki.livedoor.jp/mikk_ni3_92/d/glDrawRangeElements%A4%CB%A4%E8%A4%EB%C9%C1%B2%E8
	// ただ、全範囲を渡しても特に問題なさそうなのでこのまま。
	if (m_currentIndexBuffer->format() == IndexBufferFormat::Index16)
	{
		GL_CHECK(glDrawElements(gl_prim, vertexCount, GL_UNSIGNED_SHORT, (GLvoid*)(sizeof(GLushort) * startIndex)));
	}
	else
	{
		GL_CHECK(glDrawElements(gl_prim, vertexCount, GL_UNSIGNED_INT, (GLvoid*)(sizeof(GLuint) * startIndex)));
	}
}

void OpenGLDeviceContext::onPresent(ISwapChain* swapChain)
{
	auto* s = static_cast<GLSwapChain*>(swapChain);


	//glBindTexture(GL_TEXTURE_2D, 0); // 描画先テクスチャのバインドを解除しておく.
	//glBindFramebuffer(GL_FRAMEBUFFER, fboMSAA);
	/* レンダーターゲットへ描画処理 */

	SizeI windowSize, bufferSize;
	s->getTargetWindowSize(&windowSize);
	s->getColorBuffer()->getSize(&bufferSize);

	// いわゆるResolve処理.
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, s->fbo());
	glBlitFramebuffer(0, 0, bufferSize.width, bufferSize.height, 0, 0, windowSize.width, windowSize.height, GL_COLOR_BUFFER_BIT, GL_LINEAR);
	GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, 0));

	/* レンダーテクスチャを使用 */
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glBindTexture(GL_TEXTURE_2D, renderTex);


	m_glContext->swap(s);
}

void OpenGLDeviceContext::getPrimitiveInfo(PrimitiveType primitive, int primitiveCount, GLenum* gl_prim, int* vertexCount)
{
	switch (primitive)
	{
	case PrimitiveType::TriangleList:
		*gl_prim = GL_TRIANGLES;
		*vertexCount = primitiveCount * 3;
		break;
	case PrimitiveType::TriangleStrip:
		*gl_prim = GL_TRIANGLE_STRIP;
		*vertexCount = 2 + primitiveCount;
		break;
	case PrimitiveType::TriangleFan:
		*gl_prim = GL_TRIANGLE_FAN;
		*vertexCount = 2 + primitiveCount;
		break;
	case PrimitiveType::LineList:
		*gl_prim = GL_LINES;
		*vertexCount = primitiveCount * 2;
		break;
	case PrimitiveType::LineStrip:
		*gl_prim = GL_LINE_STRIP;
		*vertexCount = 1 + primitiveCount;
		break;
	case PrimitiveType::PointList:
		*gl_prim = GL_POINTS;
		*vertexCount = primitiveCount;
		break;
	default:
		LN_UNREACHABLE();
		break;
	}
}

//=============================================================================
// GLSwapChain

GLSwapChain::GLSwapChain()
	: m_backbuffer(nullptr)
	, m_fbo(0)
{
}

void GLSwapChain::dispose()
{
	if (m_fbo)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDeleteFramebuffers(1, &m_fbo);
	}

	if (m_backbuffer)
	{
	}
	m_backbuffer = nullptr;

	ISwapChain::dispose();
}

void GLSwapChain::setupBackbuffer(uint32_t width, uint32_t height)
{
	m_backbuffer = makeRef<GLRenderTargetTexture>();
	m_backbuffer->initialize(width, height, TextureFormat::RGBA32, false);

	GL_CHECK(glGenFramebuffers(1, &m_fbo));
	GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, m_fbo));

	GL_CHECK(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_backbuffer->id(), 0));

	LN_ENSURE(GL_FRAMEBUFFER_COMPLETE == glCheckFramebufferStatus(GL_FRAMEBUFFER),
		"glCheckFramebufferStatus failed 0x%08x",
		glCheckFramebufferStatus(GL_FRAMEBUFFER));

	GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

ITexture* GLSwapChain::getColorBuffer() const
{
	return m_backbuffer;
}

//=============================================================================
// EmptyGLContext

Ref<GLSwapChain> EmptyGLContext::createSwapChain(PlatformWindow* window, const SizeI& backbufferSize)
{
	return makeRef<EmptyGLSwapChain>();
}

void EmptyGLContext::makeCurrent(GLSwapChain* swapChain)
{
}

void EmptyGLContext::swap(GLSwapChain* swapChain)
{
}


//==============================================================================
// GLVertexDeclaration

GLVertexDeclaration::GLVertexDeclaration()
	: m_vertexElements()
{
}

GLVertexDeclaration::~GLVertexDeclaration()
{
}

void GLVertexDeclaration::initialize(const VertexElement* elements, int elementsCount)
{
	if (LN_REQUIRE(elements != nullptr)) return;
	if (LN_REQUIRE(elementsCount >= 0)) return;

	// 頂点宣言作成
	createGLVertexElements(elements, elementsCount, &m_vertexElements);
}

void GLVertexDeclaration::dispose()
{
}

void GLVertexDeclaration::createGLVertexElements(const VertexElement* vertexElements, int elementsCount, List<GLVertexElement>* outList)
{
	outList->reserve(elementsCount);

	int vertexSize = getVertexSize(vertexElements, elementsCount, 0);
	int totalSize = 0;
	for (int i = 0; i < elementsCount; ++i)
	{
		GLVertexElement elm;
		elm.streamIndex = vertexElements[i].StreamIndex;
		elm.usage = vertexElements[i].Usage;
		elm.usageIndex = vertexElements[i].UsageIndex;

		convertDeclTypeLNToGL(
			vertexElements[i].Type,
			&elm.type,
			&elm.size,
			&elm.normalized);

		elm.stride = vertexSize;
		elm.byteOffset = totalSize;
		outList->add(elm);

		totalSize += getVertexElementTypeSize(vertexElements[i].Type);
	}
}

int GLVertexDeclaration::getVertexSize(const VertexElement* vertexElements, int elementsCount, int streamIndex)
{
	int size = 0;
	for (int i = 0; i < elementsCount; ++i)
	{
		if (vertexElements[i].StreamIndex == streamIndex) {
			size += getVertexElementTypeSize(vertexElements[i].Type);
		}
	}
	return size;
}

int GLVertexDeclaration::getVertexElementTypeSize(VertexElementType type)
{
	switch (type)
	{
	case VertexElementType::Float1:	return sizeof(float);
	case VertexElementType::Float2:	return sizeof(float) * 2;
	case VertexElementType::Float3:	return sizeof(float) * 3;
	case VertexElementType::Float4:	return sizeof(float) * 4;
	case VertexElementType::Ubyte4:	return sizeof(unsigned char) * 4;
	case VertexElementType::Color4:	return sizeof(unsigned char) * 4;
	case VertexElementType::Short2:	return sizeof(short) * 2;
	case VertexElementType::Short4:	return sizeof(short) * 4;
	}
	LN_UNREACHABLE();
	return 0;
}

void GLVertexDeclaration::convertDeclTypeLNToGL(VertexElementType type, GLenum* gl_type, GLint* size, GLboolean* normalized)
{
	struct FormatType
	{
		GLenum		Type;
		GLint		Size;
		GLboolean	normalize;
	};
	
	static const FormatType formatTable[] =
	{
		{ 0,				0,	GL_FALSE },	// VertexElementType::Unknown
		{ GL_FLOAT,			1,	GL_FALSE },	// VertexElementType::Float1
		{ GL_FLOAT,			2,	GL_FALSE },	// VertexElementType::Float2
		{ GL_FLOAT,			3,	GL_FALSE },	// VertexElementType::Float3
		{ GL_FLOAT,			4,	GL_FALSE },	// VertexElementType::Float4
		{ GL_UNSIGNED_BYTE, 4,	GL_FALSE },	// VertexElementType::Ubyte4
		{ GL_UNSIGNED_BYTE, 4,	GL_TRUE },	// VertexElementType::Color4
		{ GL_SHORT,			2,	GL_FALSE },	// VertexElementType::Short2
		{ GL_SHORT,			4,	GL_FALSE },	// VertexElementType::Short4
	};
	// http://www.opengl.org/sdk/docs/man/xhtml/glVertexAttribPointer.xml
	// GL_BYTE, GL_UNSIGNED_BYTE, GL_SHORT, GL_UNSIGNED_SHORT, GL_INT, GL_UNSIGNED_INT, GL_FLOAT, GL_DOUBLE

	*gl_type = formatTable[(int)type].Type;
	*size = formatTable[(int)type].Size;
	*normalized = formatTable[(int)type].normalize;
}

//==============================================================================
// GLVertexBuffer

GLVertexBuffer::GLVertexBuffer()
	: m_glVertexBuffer(0)
	//, m_byteCount(0)
	//, m_data(NULL)
	, m_usage(0)
	, m_format(GraphicsResourceUsage::Static)
{
}

GLVertexBuffer::~GLVertexBuffer()
{
}

void GLVertexBuffer::initialize(GraphicsResourceUsage usage, size_t bufferSize, const void* initialData)
{
	m_format = usage;
	//m_byteCount = bufferSize;
	//m_data = LN_NEW byte_t[m_byteCount];
	//if (initialData) {
	//	memcpy_s(m_data, m_byteCount, initialData, m_byteCount);
	//}
	//else {
	//	memset(m_data, 0, m_byteCount);
	//}

	if (m_format == GraphicsResourceUsage::Dynamic) {
		m_usage = GL_DYNAMIC_DRAW;
	}
	else {
		m_usage = GL_STATIC_DRAW;
	}

	GL_CHECK(glGenBuffers(1, &m_glVertexBuffer));
	GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, m_glVertexBuffer));
	GL_CHECK(glBufferData(GL_ARRAY_BUFFER, bufferSize, initialData, m_usage));
	GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void GLVertexBuffer::dispose()
{
	if (m_glVertexBuffer) {
		GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));
		GL_CHECK(glDeleteBuffers(1, &m_glVertexBuffer));
		m_glVertexBuffer = 0;
	}

	IVertexBuffer::dispose();
}

void GLVertexBuffer::setSubData(size_t offset, const void* data, size_t length)
{
	GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, m_glVertexBuffer));
	GL_CHECK(glBufferSubData(GL_ARRAY_BUFFER, offset, length, data));
	GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void* GLVertexBuffer::map(size_t offset, uint32_t length)
{
	LN_NOTIMPLEMENTED();
	return nullptr;
}

void GLVertexBuffer::unmap()
{
	LN_NOTIMPLEMENTED();
}
//------------------------------------------------------------------------------
//void* GLVertexBuffer::lock()
//{
//	return m_data;
//}
//
////------------------------------------------------------------------------------
//void GLVertexBuffer::unlock()
//{
//	//glBindBuffer(GL_ARRAY_BUFFER, m_glVertexBuffer);
//	//if (LN_ENSURE_GLERROR()) return;
//	//glBufferSubData(GL_ARRAY_BUFFER, 0, m_byteCount, m_data);
//	//if (LN_ENSURE_GLERROR()) return;
//	//glBindBuffer(GL_ARRAY_BUFFER, 0);
//	//if (LN_ENSURE_GLERROR()) return;
//}

////------------------------------------------------------------------------------
//void GLVertexBuffer::onLostDevice()
//{
//	glDeleteBuffers(1, &m_glVertexBuffer);
//	if (LN_ENSURE_GLERROR()) return;
//}
//
////------------------------------------------------------------------------------
//void GLVertexBuffer::onResetDevice()
//{
//	glGenBuffers(1, &m_glVertexBuffer);
//	if (LN_ENSURE_GLERROR()) return;
//	glBindBuffer(GL_ARRAY_BUFFER, m_glVertexBuffer);
//	if (LN_ENSURE_GLERROR()) return;
//	glBufferData(GL_ARRAY_BUFFER, m_byteCount, m_data, m_usage);
//	if (LN_ENSURE_GLERROR()) return;
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//	if (LN_ENSURE_GLERROR()) return;
//}

//==============================================================================
// GLIndexBuffer

GLIndexBuffer::GLIndexBuffer()
	: m_indexBufferId(0)
	, m_format(IndexBufferFormat::Index16)
	, m_usage(GL_STATIC_DRAW)
{
}

GLIndexBuffer::~GLIndexBuffer()
{
}

void GLIndexBuffer::initialize(GraphicsResourceUsage usage, IndexBufferFormat format, int indexCount, const void* initialData)
{
	m_format = format;
	m_usage = (usage == GraphicsResourceUsage::Static) ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW;
	int stride = (m_format == IndexBufferFormat::Index16) ? 2 : 4;

	GL_CHECK(glGenBuffers(1, &m_indexBufferId));
	GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferId));
	GL_CHECK(glBufferData(GL_ELEMENT_ARRAY_BUFFER, stride * indexCount, initialData, m_usage));
	GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

void GLIndexBuffer::dispose()
{
	if (m_indexBufferId) {
		GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
		GL_CHECK(glDeleteBuffers(1, &m_indexBufferId));
		m_indexBufferId = 0;
	}

	IIndexBuffer::dispose();
}

void GLIndexBuffer::setSubData(size_t offset, const void* data, size_t length)
{
	GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferId));
	GL_CHECK(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, length, data));
	GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

void* GLIndexBuffer::map(size_t offset, uint32_t length)
{
	LN_NOTIMPLEMENTED();
	return nullptr;
}

void GLIndexBuffer::unmap()
{
	LN_NOTIMPLEMENTED();
}

//=============================================================================
// GLSLShader

GLRenderTargetTexture::GLRenderTargetTexture()
{
}

GLRenderTargetTexture::~GLRenderTargetTexture()
{
}

void GLRenderTargetTexture::initialize(uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap)
{
	if (mipmap) {
		LN_NOTIMPLEMENTED();
		return;
	}

	m_size = SizeI(width, height);
	m_textureFormat = requestFormat;

	GL_CHECK(glGenTextures(1, &m_id));
	GL_CHECK(glBindTexture(GL_TEXTURE_2D, m_id));

	GL_CHECK(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));

	GLenum internalFormat;
	//m_pixelFormat = GL_BGRA;
	OpenGLHelper::getGLTextureFormat(requestFormat, &internalFormat, &m_pixelFormat, &m_elementType);

	GL_CHECK(glTexImage2D(
		GL_TEXTURE_2D,
		0/*m_mipLevels*/,	// TODO: 0 でないと glCheckFramebufferStatus が GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT を返した。MipMap についてはちゃんと調査する必要がありそう。
		internalFormat,
		width,
		height,
		0,
		m_pixelFormat,
		m_elementType,
		nullptr));

	// glTexParameteri() を一つも指定しないと, テクスチャが正常に使用できない場合がある
	GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

	GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0));
}

void GLRenderTargetTexture::dispose()
{
	if (m_id != 0) {
		GL_CHECK(glDeleteTextures(1, &m_id));
		m_id = 0;
	}

	GLTextureBase::dispose();
}

void GLRenderTargetTexture::readData(void* outData)
{
	GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	GL_CHECK(glBindTexture(GL_TEXTURE_2D, m_id));
	GL_CHECK(glGetTexImage(GL_TEXTURE_2D, 0, m_pixelFormat, m_elementType, outData));
	GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0));
}

void GLRenderTargetTexture::getSize(SizeI* outSize)
{
	*outSize = m_size;
}

//=============================================================================
// GLDepthBuffer

void GLDepthBuffer::dispose()
{
	IDepthBuffer::dispose();
}

//=============================================================================
// GLSLShader

GLSLShader::GLSLShader()
	: m_shader(0)
	, m_type(0)
{
}

GLSLShader::~GLSLShader()
{
	dispose();
}

bool GLSLShader::create(const byte_t* code, int length, GLenum type, ShaderCompilationDiag* diag)
{
	// GL_VERTEX_SHADER, GL_FRAGMENT_SHADER
	m_type = type;

	m_shader = glCreateShader(m_type);
	if (LN_ENSURE(m_shader != 0, "Failed to create shader.")) return false;

	GLint codeSize[1] = { length };
	GL_CHECK(glShaderSource(m_shader, 1, (const GLchar**)&code, codeSize));
	GL_CHECK(glCompileShader(m_shader));

	// result
	GLint compiled = 0;
	GLint logSize = 0;
	GL_CHECK(glGetShaderiv(m_shader, GL_COMPILE_STATUS, &compiled));
	GL_CHECK(glGetShaderiv(m_shader, GL_INFO_LOG_LENGTH, &logSize));	// excluding the null terminator

	if (logSize > 1)
	{
		std::vector<char> buf(logSize);
		int length;
		GL_CHECK(glGetShaderInfoLog(m_shader, logSize, &length, (GLchar*)buf.data()));
		diag->message += (const char*)buf.data();
		diag->level = ShaderCompilationResultLevel::Warning;
	}

	if (compiled == GL_FALSE)
	{
		diag->level = ShaderCompilationResultLevel::Error;
		return false;
	}

	return true;
}

void GLSLShader::dispose()
{
	if (m_shader)
	{
		GL_CHECK(glDeleteShader(m_shader));
		m_shader = 0;
	}
}

//=============================================================================
// GLShaderPass

GLShaderPass::GLShaderPass()
	: m_context(nullptr)
	, m_program(0)
	, m_uniforms()
{
}

void GLShaderPass::initialize(OpenGLDeviceContext* context, const byte_t* vsCode, int vsCodeLen, const byte_t* fsCode, int fsCodeLen, ShaderCompilationDiag* diag)
{
	m_context = context;

	GLSLShader vertexShader;
	GLSLShader fragmentShader;
	if (!vertexShader.create(vsCode, vsCodeLen, GL_VERTEX_SHADER, diag)) return;
	if (!fragmentShader.create(fsCode, fsCodeLen, GL_FRAGMENT_SHADER, diag)) return;

	m_program = glCreateProgram();
	LN_LOG_VERBOSE << "Program create:" << m_program << " vs:" << vertexShader.shader() << " fs:" << fragmentShader.shader();

	GL_CHECK(glAttachShader(m_program, vertexShader.shader()));
	GL_CHECK(glAttachShader(m_program, fragmentShader.shader()));

	GLint linked = 0;
	GL_CHECK(glLinkProgram(m_program));
	GL_CHECK(glGetProgramiv(m_program, GL_LINK_STATUS, &linked));

	int logSize;
	GL_CHECK(glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &logSize));

	if (logSize > 1)
	{
		std::vector<char> buf(logSize);
		int length;
		GL_CHECK(glGetProgramInfoLog(m_program, logSize, &length, (GLchar*)buf.data()));
		diag->message += (const char*)buf.data();
		diag->level = ShaderCompilationResultLevel::Warning;
	}

	if (linked == GL_FALSE)
	{
		diag->level = ShaderCompilationResultLevel::Error;
		return;
	}

	buildUniforms();
}

void GLShaderPass::dispose()
{
	if (m_program)
	{
		GL_CHECK(glUseProgram(0));
		GL_CHECK(glDeleteProgram(m_program));
		m_program = 0;
	}

	IShaderPass::dispose();
}

int GLShaderPass::getUniformCount() const
{
	return m_uniforms.size();
}

IShaderUniform* GLShaderPass::getUniform(int index) const
{
	return m_uniforms[index];
}

void GLShaderPass::setUniformValue(int index, const void* data, size_t size)
{
	m_context->setActiveShaderPass(this);
	m_uniforms[index]->setUniformValue(m_context, data, size);
}

void GLShaderPass::buildUniforms()
{
	GLint count = 0;
	GL_CHECK(glGetProgramiv(m_program, GL_ACTIVE_UNIFORMS, &count));

	for (int i = 0; i < count; i++)
	{
		GLsizei name_len = 0;
		GLsizei var_size = 0;
		GLenum  var_type = 0;
		GLchar  name[256] = { 0 };
		GL_CHECK(glGetActiveUniform(m_program, i, 256, &name_len, &var_size, &var_type, name));

		GLint loc = glGetUniformLocation(m_program, name);

		ShaderUniformTypeDesc desc;
		OpenGLHelper::convertVariableTypeGLToLN(
			name, var_type, var_size,
			&desc.type, &desc.rows, &desc.columns, &desc.elements);

		auto uni = makeRef<GLShaderUniform>();
		uni->m_desc = desc;
		uni->m_name = name;
		uni->m_location = loc;
		m_uniforms.add(uni);

		//// テクスチャ型の変数にステージ番号を振っていく。
		//if (passVar.Variable->getType() == ShaderVariableType::DeviceTexture)
		//{
		//	passVar.TextureStageIndex = textureVarCount;
		//	textureVarCount++;
		//}
		//else
		//{
		//	passVar.TextureStageIndex = -1;
		//}

	}
}

//=============================================================================
// GLShaderUniform

GLShaderUniform::GLShaderUniform()
	: m_desc{ShaderRefrectionParameterType::Unknown, 0, 0, 0}
	, m_name()
	, m_location(0)
{
}

void GLShaderUniform::dispose()
{
	IShaderUniform::dispose();
}

void GLShaderUniform::setUniformValue(OpenGLDeviceContext* context, const void* data, size_t size)
{
	LN_CHECK(context);
	LN_CHECK(data);

	MemoryStream* tempBuffer = context->uniformTempBuffer();
	BinaryWriter* tempWriter = context->uniformTempBufferWriter();
	tempWriter->seek(0, SeekOrigin::Begin);

	switch (m_desc.type)
	{
	case ShaderVariableType::Bool:
		GL_CHECK(glUniform1i(m_location, *static_cast<const uint8_t*>(data)));
		break;
	case ShaderVariableType::BoolArray:
	{
		const uint8_t* begin = static_cast<const uint8_t*>(data);
		const uint8_t* end = begin + m_desc.elements;
		std::for_each(begin, end, [&tempWriter](bool v) { GLint i = (v) ? 1 : 0; tempWriter->write(&i, sizeof(GLint)); });
		GL_CHECK(glUniform1iv(m_location, m_desc.elements, (const GLint*)tempBuffer->data()));
		break;
	}
	case ShaderVariableType::Int:
		GL_CHECK(glUniform1i(m_location, *static_cast<const int32_t*>(data)));
		break;
	case ShaderVariableType::Float:
		GL_CHECK(glUniform1f(m_location, *static_cast<const float*>(data)));
		break;
	case ShaderVariableType::Vector:
	{
		const Vector4* vec = static_cast<const Vector4*>(data);
		if (m_desc.columns == 2) {
			GL_CHECK(glUniform2f(m_location, vec->x, vec->y));
		}
		else if (m_desc.columns == 3) {
			GL_CHECK(glUniform3f(m_location, vec->x, vec->y, vec->z));
		}
		else if (m_desc.columns == 4) {
			GL_CHECK(glUniform4f(m_location, vec->x, vec->y, vec->z, vec->w));
		}
		else {
			LN_UNREACHABLE();
		}
		break;
	}
	case ShaderVariableType::VectorArray:
	{
		const Vector4* begin = static_cast<const Vector4*>(data);
		const Vector4* end = begin + m_desc.elements;

		if (m_desc.columns == 2)
		{
			std::for_each(begin, end, [&tempWriter](const Vector4& v) { tempWriter->write(&v, sizeof(float) * 2); });
			GL_CHECK(glUniform2fv(m_location, m_desc.elements, (const GLfloat*)tempBuffer->data()));
		}
		else if (m_desc.columns == 3)
		{
			std::for_each(begin, end, [&tempWriter](const Vector4& v) { tempWriter->write(&v, sizeof(float) * 3); });
			GL_CHECK(glUniform3fv(m_location, m_desc.elements, (const GLfloat*)tempBuffer->data()));
		}
		else if (m_desc.columns == 4)
		{
			GL_CHECK(glUniform4fv(m_location, m_desc.elements, (const GLfloat*)begin));
		}
		else
		{
			LN_UNREACHABLE();
		}
		break;
	}
	case ShaderVariableType::Matrix:
	{
		GL_CHECK(glUniformMatrix4fv(m_location, 1, GL_FALSE, static_cast<const GLfloat*>(data)));
		break;
	}
	case ShaderVariableType::MatrixArray:
		GL_CHECK(glUniformMatrix4fv(m_location, m_desc.elements, GL_FALSE, static_cast<const GLfloat*>(data)));
		break;
	case ShaderVariableType::Texture:
		LN_NOTIMPLEMENTED();
		//// textureStageIndex のテクスチャステージにバインド
		//glActiveTexture(GL_TEXTURE0 + textureStageIndex);
		//if (LN_ENSURE_GLERROR()) return;

		//if (m_value.getDeviceTexture() != nullptr)
		//	glBindTexture(GL_TEXTURE_2D, static_cast<GLTextureBase*>(m_value.getDeviceTexture())->getGLTexture());
		//else
		//	glBindTexture(GL_TEXTURE_2D, 0);
		//if (LN_ENSURE_GLERROR()) return;


		////LNGL::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mSamplerState->MinFilter);
		////LNGL::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mSamplerState->MagFilter);
		////LNGL::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mSamplerState->AddressU);
		////LNGL::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mSamplerState->AddressV);

		//// テクスチャステージ番号をセット
		//glUniform1i(m_location, textureStageIndex);
		//if (LN_ENSURE_GLERROR()) return;
		break;
	default:
		LN_UNREACHABLE();
		break;
	}
}


} // namespace detail
} // namespace ln

