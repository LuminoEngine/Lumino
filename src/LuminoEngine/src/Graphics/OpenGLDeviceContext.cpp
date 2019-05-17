
#include "Internal.hpp"
#include <LuminoEngine/Platform/PlatformWindow.hpp>
#include "OpenGLDeviceContext.hpp"

#include "GLFWContext.hpp"


#ifndef GL_TEXTURE_MAX_ANISOTROPY_EXT
#	define GL_TEXTURE_MAX_ANISOTROPY_EXT 0x84FE
#endif // GL_TEXTURE_MAX_ANISOTROPY_EXT

#ifndef GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT
#	define GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT 0x84FF
#endif // GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT


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
			GLENUM(GL_INVALID_ENUM);
			GLENUM(GL_INVALID_VALUE);
			GLENUM(GL_INVALID_OPERATION);
			GLENUM(GL_OUT_OF_MEMORY);
			GLENUM(GL_INVALID_FRAMEBUFFER_OPERATION);
		}
#undef GLENUM
		return "<GLenum?>";
	}

	static void convertVariableTypeGLToLN(
		const char* name, GLenum gl_type, GLsizei gl_var_size,
        ShaderUniformType* outType, int* outRows, int* outColumns, int* outElements)
	{
		*outElements = 0;

#define SET_LNDESC( c_, t_, row_, col_ ) { *outType = (t_); *outRows = (row_); *outColumns = (col_); }
		switch (gl_type)
		{
			case GL_FLOAT:      SET_LNDESC(LN_SVC_SCALAR, ShaderUniformType_Float, 1, 1); break;
			case GL_FLOAT_VEC2: SET_LNDESC(LN_SVC_VECTOR, ShaderUniformType_Vector, 1, 2); break;
			case GL_FLOAT_VEC3: SET_LNDESC(LN_SVC_VECTOR, ShaderUniformType_Vector, 1, 3); break;
			case GL_FLOAT_VEC4: SET_LNDESC(LN_SVC_VECTOR, ShaderUniformType_Vector, 1, 4); break;

			case GL_INT:        SET_LNDESC(LN_SVC_SCALAR, ShaderUniformType_Int, 1, 1); break;
			//case GL_INT_VEC2:   SET_LNDESC(LN_SVC_VECTOR, ShaderUniformType_IntVector, 1, 2); break;
			//case GL_INT_VEC3:   SET_LNDESC(LN_SVC_VECTOR, ShaderUniformType_IntVector, 1, 3); break;
			//case GL_INT_VEC4:   SET_LNDESC(LN_SVC_VECTOR, ShaderUniformType_IntVector, 1, 4); break;

			case GL_UNSIGNED_INT:	SET_LNDESC(LN_SVC_SCALAR, ShaderUniformType_Int, 1, 1); break;

			case GL_BOOL:        SET_LNDESC(LN_SVC_SCALAR, ShaderUniformType_Bool, 1, 1); break;
			//case GL_BOOL_VEC2:   SET_LNDESC(LN_SVC_VECTOR, ShaderUniformType_BoolVector, 1, 2); break;
			//case GL_BOOL_VEC3:   SET_LNDESC(LN_SVC_VECTOR, ShaderUniformType_BoolVector, 1, 3); break;
			//case GL_BOOL_VEC4:   SET_LNDESC(LN_SVC_VECTOR, ShaderUniformType_BoolVector, 1, 4); break;

			case GL_FLOAT_MAT2:     SET_LNDESC(LN_SVC_MATRIX, ShaderUniformType_Matrix, 2, 2); break;
			case GL_FLOAT_MAT3:     SET_LNDESC(LN_SVC_MATRIX, ShaderUniformType_Matrix, 3, 3); break;
			case GL_FLOAT_MAT4:     SET_LNDESC(LN_SVC_MATRIX, ShaderUniformType_Matrix, 4, 4); break;

			// es で使えない
			//case GL_SAMPLER_1D:         SET_LNDESC(LN_SVC_SAMPLER, ShaderUniformType_Texture, 1, 1); break;

			case GL_SAMPLER_2D:         SET_LNDESC(LN_SVC_SAMPLER, ShaderUniformType_Texture, 1, 1); break;
			case GL_SAMPLER_CUBE:       SET_LNDESC(LN_SVC_SAMPLER, ShaderUniformType_Texture, 1, 1); break;
			case GL_SAMPLER_3D:         SET_LNDESC(LN_SVC_SAMPLER, ShaderUniformType_Texture, 1, 1); break;

			//#if !defined(LNOTE_GLES)
            // https://www.khronos.org/opengl/wiki/Data_Type_(GLSL)#Matrices
            // GL_FLOAT_MAT3x4 -> mat3x4 -> matNxM (N columns and M rows)
			case GL_FLOAT_MAT2x3:   SET_LNDESC(LN_SVC_MATRIX, ShaderUniformType_Matrix, 3, 2); break;
			case GL_FLOAT_MAT2x4:   SET_LNDESC(LN_SVC_MATRIX, ShaderUniformType_Matrix, 4, 2); break;
			case GL_FLOAT_MAT3x2:   SET_LNDESC(LN_SVC_MATRIX, ShaderUniformType_Matrix, 2, 3); break;
			case GL_FLOAT_MAT3x4:   SET_LNDESC(LN_SVC_MATRIX, ShaderUniformType_Matrix, 4, 3); break;
			case GL_FLOAT_MAT4x2:   SET_LNDESC(LN_SVC_MATRIX, ShaderUniformType_Matrix, 2, 4); break;
			case GL_FLOAT_MAT4x3:   SET_LNDESC(LN_SVC_MATRIX, ShaderUniformType_Matrix, 3, 4); break;

			//case GL_FLOAT:      SET_LNDESC( LN_SVC_SCALAR, LN_SVT_FLOAT, 1, 1 ); break;
			//case GL_FLOAT_VEC2: SET_LNDESC( LN_SVC_VECTOR, LN_SVT_FLOAT, 1, 2 ); break;
			//case GL_FLOAT_VEC3: SET_LNDESC( LN_SVC_VECTOR, LN_SVT_FLOAT, 1, 3 ); break;
			//case GL_FLOAT_VEC4: SET_LNDESC( LN_SVC_VECTOR, LN_SVT_FLOAT, 1, 4 ); break;


			// es で使えない
			//case GL_SAMPLER_1D_SHADOW:  SET_LNDESC(LN_SVC_SAMPLER, ShaderUniformType_Unknown, 1, 1); break;
			case GL_SAMPLER_2D_SHADOW:  SET_LNDESC(LN_SVC_SAMPLER, ShaderUniformType_Unknown, 1, 1); break;
				//#endif
			default:
				SET_LNDESC(LN_SVC_SAMPLER, ShaderUniformType_Unknown, 0, 0);
				break;
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
#if defined(LN_GRAPHICS_OPENGLES)
		static GLenum table[][3] =
		{
			// internalFormat,		pixelFormat,		elementType
			{ GL_NONE,				GL_NONE,			GL_NONE },			// TextureFormat::Unknown
			{ GL_RGBA,				GL_RGBA,			GL_UNSIGNED_BYTE },	// TextureFormat::R8G8B8A8,            ///< 32 ビットのアルファ付きフォーマット (uint32_t アクセス時の表現。lnByte[4] にすると、ABGR)
			{ GL_RGBA,				GL_RGBA,			GL_UNSIGNED_BYTE },	// TextureFormat::R8G8B8X8,	※元々 GL_RGB だったが、それだと glGetTexImage で強制終了
			{ GL_RGBA16F,			GL_RGBA,			GL_HALF_FLOAT },	// TextureFormat::A16B16G16R16F,       ///< 64 ビットの浮動小数点フォーマット
			{ GL_RGBA32F,			GL_RGBA,			GL_FLOAT },			// TextureFormat::A32B32G32R32F,       ///< 128 ビットの浮動小数点フォーマット
			{ GL_R16F,				GL_RED,				GL_HALF_FLOAT },	// TextureFormat::R16F,
			{ GL_R32UI,				GL_RED,				GL_FLOAT },			// TextureFormat::R32F,
																			//{ GL_R32UI,				GL_RED_INTEGER,		GL_INT },			// TextureFormat::R32_UInt,
			//{ GL_DEPTH_COMPONENT24, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE }, // TextureFormat::D24S8,               ///< 32 ビットの Z バッファフォーマット
		};

#else
		// http://angra.blog31.fc2.com/blog-entry-11.html
		static GLenum table[][3] =
		{
			// internalFormat,		pixelFormat,		elementType
			{ GL_NONE,				GL_NONE,			GL_NONE },			// TextureFormat::Unknown
			{ GL_RGBA8,				GL_RGBA,			GL_UNSIGNED_BYTE },	// TextureFormat::R8G8B8A8,            ///< 32 ビットのアルファ付きフォーマット (uint32_t アクセス時の表現。lnByte[4] にすると、ABGR)
			{ GL_RGB8,				GL_RGB,			GL_UNSIGNED_BYTE },	// TextureFormat::R8G8B8X8,	※元々 GL_RGB だったが、それだと glGetTexImage で強制終了
			{ GL_RGBA16F,			GL_RGBA,			GL_HALF_FLOAT },	// TextureFormat::A16B16G16R16F,       ///< 64 ビットの浮動小数点フォーマット
			{ GL_RGBA32F,			GL_RGBA,			GL_FLOAT },			// TextureFormat::A32B32G32R32F,       ///< 128 ビットの浮動小数点フォーマット
			{ GL_R16F,				GL_RED,				GL_HALF_FLOAT },	// TextureFormat::R16F,
			{ GL_R32UI,				GL_RED,				GL_FLOAT },			// TextureFormat::R32F,
																			//{ GL_R32UI,				GL_RED_INTEGER,		GL_INT },			// TextureFormat::R32_UInt,
			//{ GL_DEPTH_COMPONENT24, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE }, // TextureFormat::D24S8,               ///< 32 ビットの Z バッファフォーマット
		};
#endif
		*internalFormat = table[(int)format][0];
		*pixelFormat = table[(int)format][1];
		*elementType = table[(int)format][2];
	}
};


//=============================================================================
// OpenGLDevice

OpenGLDevice::OpenGLDevice()
	: m_glContext(nullptr)
	, m_uniformTempBuffer()
	, m_uniformTempBufferWriter(&m_uniformTempBuffer)
{
}

void OpenGLDevice::init(const Settings& settings)
{
	LN_LOG_DEBUG << "OpenGLDeviceContext::init start";

    if (settings.mainWindow)
    {
#ifdef LN_GLFW
	    auto glfwContext = makeRef<GLFWContext>();
	    glfwContext->init(settings.mainWindow);
	    m_glContext = glfwContext;
#endif
    }

    if (!m_glContext)
    {
        // Android(GLSurfaceView) や Web など、バックバッファの swap を Lumino の外側で行う場合のダミー
        auto glfwContext = makeRef<EmptyGLContext>();
        m_glContext = glfwContext;
    }

#if defined(LN_GRAPHICS_OPENGLES)
	LN_LOG_INFO << "OpenGL ES enabled.";
#endif

#if defined(LN_GRAPHICS_OPENGLES)
#else
	int result = gladLoadGL();
	if (LN_ENSURE(result, "Failed gladLoadGL()")) return;
	LN_LOG_INFO << "OpenGL " << GLVersion.major << "." << GLVersion.minor;
#endif

	//GLint value;
	//glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &value);

	LN_LOG_INFO << "GL_VERSION : " << glGetString(GL_VERSION);

	GL_CHECK(glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &m_caps.MAX_VERTEX_ATTRIBS));
	GL_CHECK(glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &m_caps.MAX_COLOR_ATTACHMENTS));
	LN_LOG_INFO << "GL_MAX_VERTEX_ATTRIBS : " << m_caps.MAX_VERTEX_ATTRIBS;
	LN_LOG_INFO << "GL_MAX_COLOR_ATTACHMENTS : " << m_caps.MAX_COLOR_ATTACHMENTS;
	
	
	int extensions = 0;
	glGetIntegerv(GL_EXTENSIONS, &extensions);
	LN_LOG_INFO << "GL_EXTENSIONS : " << extensions;
	for(int i = 0; i < extensions; i++) {
		LN_LOG_INFO << "  " << glGetStringi(GL_EXTENSIONS, i);
	}
    while (glGetError() != 0);	// ignore error.


    const char* extensionsString = (const char*)glGetString(GL_EXTENSIONS);
    while (glGetError() != 0);	// ignore error.
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


	m_graphicsContext = makeRef<GLGraphicsContext>();
	m_graphicsContext->init(this);

	LN_LOG_DEBUG << "OpenGLDeviceContext::init end";
}

void OpenGLDevice::dispose()
{
	if (m_graphicsContext) {
		m_graphicsContext->dispose();
		m_graphicsContext = nullptr;
	}

    IGraphicsDevice::dispose();
}

IGraphicsContext* OpenGLDevice::getGraphicsContext() const
{
	return m_graphicsContext;
}

void OpenGLDevice::onGetCaps(GraphicsDeviceCaps* outCaps)
{
#ifdef LN_GRAPHICS_OPENGLES
	outCaps->requestedShaderTriple.target = "glsl";
	outCaps->requestedShaderTriple.version = 300;
	outCaps->requestedShaderTriple.option = "es";
#else
	outCaps->requestedShaderTriple.target = "glsl";
	outCaps->requestedShaderTriple.version = 400;
	outCaps->requestedShaderTriple.option = "";
#endif
}

Ref<ISwapChain> OpenGLDevice::onCreateSwapChain(PlatformWindow* window, const SizeI& backbufferSize)
{
	return m_glContext->createSwapChain(window, backbufferSize);
}

Ref<IVertexDeclaration> OpenGLDevice::onCreateVertexDeclaration(const VertexElement* elements, int elementsCount)
{
	auto ptr = makeRef<GLVertexDeclaration>();
	ptr->init(elements, elementsCount);
	return ptr;
}

Ref<IVertexBuffer> OpenGLDevice::onCreateVertexBuffer(GraphicsResourceUsage usage, size_t bufferSize, const void* initialData)
{
	auto ptr = makeRef<GLVertexBuffer>();
	ptr->init(usage, bufferSize, initialData);
	return ptr;
}

Ref<IIndexBuffer> OpenGLDevice::onCreateIndexBuffer(GraphicsResourceUsage usage, IndexBufferFormat format, int indexCount, const void* initialData)
{
	auto ptr = makeRef<GLIndexBuffer>();
	ptr->init(usage, format, indexCount, initialData);
	return ptr;
}

Ref<ITexture> OpenGLDevice::onCreateTexture2D(GraphicsResourceUsage usage, uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap, const void* initialData)
{
	auto ptr = makeRef<GLTexture2D>();
	ptr->init(usage, width, height, requestFormat, mipmap, initialData);
	return ptr;
}

Ref<ITexture> OpenGLDevice::onCreateTexture3D(GraphicsResourceUsage usage, uint32_t width, uint32_t height, uint32_t depth, TextureFormat requestFormat, bool mipmap, const void* initialData)
{
	auto ptr = makeRef<GLTexture3D>();
	ptr->init(usage, width, height, depth, requestFormat, mipmap, initialData);
	return ptr;
}

Ref<ITexture> OpenGLDevice::onCreateRenderTarget(uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap)
{
	auto ptr = makeRef<GLRenderTargetTexture>();
	ptr->init(width, height, requestFormat, mipmap);
	return ptr;
}

Ref<ITexture> OpenGLDevice::onCreateWrappedRenderTarget(intptr_t nativeObject, uint32_t hintWidth, uint32_t hintHeight)
{
    auto ptr = makeRef<GLRenderTargetTexture>();
    ptr->init(nativeObject, hintWidth, hintHeight);
    return ptr;
}

Ref<IDepthBuffer> OpenGLDevice::onCreateDepthBuffer(uint32_t width, uint32_t height)
{
	auto ptr = makeRef<GLDepthBuffer>();
	ptr->init(width, height);
	return ptr;
}

Ref<ISamplerState> OpenGLDevice::onCreateSamplerState(const SamplerStateData& desc)
{
	auto ptr = makeRef<GLSamplerState>();
	ptr->init(this, desc);
	return ptr;
}

Ref<IShaderPass> OpenGLDevice::onCreateShaderPass(const ShaderPassCreateInfo& createInfo, ShaderCompilationDiag* diag)
{
	auto ptr = makeRef<GLShaderPass>();
	ptr->init(this, createInfo.vsCode, createInfo.vsCodeLen, createInfo.psCode, createInfo.psCodeLen, diag);
	return ptr;
}

Ref<IGraphicsContext> OpenGLDevice::onCreateGraphicsContext()
{
	auto ptr = makeRef<GLGraphicsContext>();
	if (!ptr->init(this)) {
		return nullptr;
	}
	return ptr;
}


//=============================================================================
// GLGraphicsContext

GLGraphicsContext::GLGraphicsContext()
	: m_device(nullptr)
	, m_vao(0)
	, m_fbo(0)
	, m_currentIndexBuffer(nullptr)
	, m_activeShaderPass(nullptr)
{
}

Result GLGraphicsContext::init(OpenGLDevice* owner)
{
	LN_CHECK(owner);
	IGraphicsContext::init(owner);
	m_device = owner;

	GL_CHECK(glGenVertexArrays(1, &m_vao));
	GL_CHECK(glGenFramebuffers(1, &m_fbo));

	return true;
}

void GLGraphicsContext::dispose()
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

void GLGraphicsContext::setActiveShaderPass(GLShaderPass* pass)
{
	if (m_activeShaderPass != pass) {
		m_activeShaderPass = pass;
		::glUseProgram((m_activeShaderPass) ? m_activeShaderPass->program() : 0);
	}
}

void GLGraphicsContext::onSaveExternalRenderState()
{
	GL_CHECK(glGetBooleanv(GL_CULL_FACE, &m_savedState.state_GL_CULL_FACE));
}

void GLGraphicsContext::onRestoreExternalRenderState()
{
	if (m_savedState.state_GL_CULL_FACE) {
		GL_CHECK(glEnable(GL_CULL_FACE));
	}
	else {
		GL_CHECK(glDisable(GL_CULL_FACE));
	}

	GL_CHECK(glBindVertexArray(0));

	setActiveShaderPass(nullptr);
}

void GLGraphicsContext::onUpdatePipelineState(const BlendStateDesc& blendState, const RasterizerStateDesc& rasterizerState, const DepthStencilStateDesc& depthStencilState)
{
	// BlendState
	{
		GLenum  blendOpTable[] =
		{
			GL_FUNC_ADD,
			GL_FUNC_SUBTRACT,
			GL_FUNC_REVERSE_SUBTRACT,
			GL_MIN,
			GL_MAX,
		};

		GLenum blendFactorTable[] =
		{
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
		if (blendState.independentBlendEnable)
		{
			for (int i = 0; i < 8; i++)	// TODO: num RT
			{
				const RenderTargetBlendDesc& desc = blendState.renderTargets[i];

				// blendEnable
				if (desc.blendEnable) {
					GL_CHECK(glEnablei(GL_BLEND, i));
				}
				else {
					GL_CHECK(glEnablei(GL_BLEND, i));
				}

				// sourceBlend
				// destinationBlend
				GL_CHECK(glBlendFuncSeparatei(i,
					blendFactorTable[(int)desc.sourceBlend],
					blendFactorTable[(int)desc.destinationBlend],
					blendFactorTable[(int)desc.sourceBlendAlpha],
					blendFactorTable[(int)desc.destinationBlendAlpha]));

				// blendOp
				GL_CHECK(glBlendEquationSeparatei(i,
					blendOpTable[(int)desc.blendOp],
					blendOpTable[(int)desc.blendOpAlpha]));
			}
		}
		else
#endif
		{
			const RenderTargetBlendDesc& desc = blendState.renderTargets[0];

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
			GL_CHECK(glPolygonMode(GL_FRONT_AND_BACK, tb[(int)rasterizerState.fillMode]));
#endif
		}

		// cullingMode
		{
			switch (rasterizerState.cullMode)
			{
			case CullMode::None:
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
		GLenum cmpFuncTable[] =
		{
			GL_NEVER,		// Never
			GL_LESS,		// Less
			GL_LEQUAL,		// LessEqual
			GL_GREATER,		// Greater
			GL_GEQUAL,		// GreaterEqual
			GL_EQUAL,		// Equal
			GL_NOTEQUAL,	// NotEqual
			GL_ALWAYS,		// Always
		};

		//// depthTestEnabled
		//if (depthStencilState.depthTestEnabled) {
		//	GL_CHECK(glEnable(GL_DEPTH_TEST));
		//}
		//else {
		//	GL_CHECK(glDisable(GL_DEPTH_TEST));
		//}

		if (depthStencilState.depthTestFunc == ComparisonFunc::Always) {
			GL_CHECK(glDisable(GL_DEPTH_TEST));
		}
		else {
			GL_CHECK(glEnable(GL_DEPTH_TEST));
			GL_CHECK(glDepthFunc(cmpFuncTable[(int)depthStencilState.depthTestFunc]));
		}

		// depthWriteEnabled
		GL_CHECK(glDepthMask(depthStencilState.depthWriteEnabled ? GL_TRUE : GL_FALSE));

		// stencilEnabled
		if (depthStencilState.stencilEnabled) {
			GL_CHECK(glEnable(GL_STENCIL_TEST));
		}
		else {
			GL_CHECK(glDisable(GL_STENCIL_TEST));
		}

		// stencilFunc
		// stencilReferenceValue
		GL_CHECK(glStencilFuncSeparate(GL_BACK, cmpFuncTable[(int)depthStencilState.frontFace.stencilFunc], depthStencilState.stencilReferenceValue, 0xFFFFFFFF));
		GL_CHECK(glStencilFuncSeparate(GL_FRONT, cmpFuncTable[(int)depthStencilState.backFace.stencilFunc], depthStencilState.stencilReferenceValue, 0xFFFFFFFF));

		// stencilFailOp
		// stencilDepthFailOp
		// stencilPassOp
		GLenum stencilOpTable[] = { GL_KEEP, GL_REPLACE };
		GL_CHECK(glStencilOpSeparate(GL_BACK, stencilOpTable[(int)depthStencilState.frontFace.stencilFailOp], stencilOpTable[(int)depthStencilState.frontFace.stencilDepthFailOp], stencilOpTable[(int)depthStencilState.frontFace.stencilPassOp]));
		GL_CHECK(glStencilOpSeparate(GL_FRONT, stencilOpTable[(int)depthStencilState.backFace.stencilFailOp], stencilOpTable[(int)depthStencilState.backFace.stencilDepthFailOp], stencilOpTable[(int)depthStencilState.backFace.stencilPassOp]));
	}
}

void GLGraphicsContext::onUpdateFrameBuffers(ITexture** renderTargets, int renderTargetsCount, IDepthBuffer* depthBuffer)
{
	if (m_fbo) {
		GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, m_fbo));
	}

	// color buffers
	int maxCount = std::min(renderTargetsCount, m_device->caps().MAX_COLOR_ATTACHMENTS);
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

void GLGraphicsContext::onUpdateRegionRects(const RectI& viewportRect, const RectI& scissorRect, const SizeI& targetSize)
{
	GL_CHECK(glViewport(viewportRect.x, targetSize.height - (viewportRect.y + viewportRect.height), viewportRect.width, viewportRect.height));
	//GL_CHECK(glViewport(0, 0, 160, 60));
	//GL_CHECK(glViewport(0, 0, 80, 60));

	GL_CHECK(glEnable(GL_SCISSOR_TEST));
	GL_CHECK(glScissor(scissorRect.x, targetSize.height - (scissorRect.y + scissorRect.height), scissorRect.width, scissorRect.height));
}

void GLGraphicsContext::onUpdatePrimitiveData(IVertexDeclaration* decls, IVertexBuffer** vertexBuufers, int vertexBuffersCount, IIndexBuffer* indexBuffer)
{
	//if (LN_REQUIRE(decls)) return;
	//if (LN_REQUIRE(vertexBuufers)) return;
	//if (LN_REQUIRE(vertexBuffersCount >= 1)) return;

	// 複数の頂点バッファを使う
	// https://qiita.com/y_UM4/items/75941cb75afb0a46aa5e

	// IVertexDeclaration で指定された頂点レイアウトと、GLSL に書かれている attribute 変数の定義順序が一致していることを前提としている。
	// ※0.4.0 以前は変数名を固定していたが、それを廃止。リフレクションっぽいことをこのモジュールの中でやりたくない。複雑になりすぎる。

	if (m_vao) {
		GL_CHECK(glBindVertexArray(m_vao));
	}

	if (decls && vertexBuufers)
	{
		GLVertexDeclaration* glDecl = static_cast<GLVertexDeclaration*>(decls);

		for (int iElement = 0; iElement < m_device->caps().MAX_VERTEX_ATTRIBS; iElement++)
		{
			GLuint attrLoc = iElement;	// 本来は attribute 変数の location

			if (iElement < glDecl->vertexElements().size())
			{
				const GLVertexElement& element = glDecl->vertexElements()[iElement];

				if (vertexBuufers[element.streamIndex])
				{
					GL_CHECK(glEnableVertexAttribArray(attrLoc));
					GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, static_cast<GLVertexBuffer*>(vertexBuufers[element.streamIndex])->vertexBufferId()));
					GL_CHECK(glVertexAttribPointer(attrLoc, element.size, element.type, element.normalized, element.stride, (void*)(element.byteOffset)));
				}
				else
				{
					GL_CHECK(glDisableVertexAttribArray(attrLoc));
					GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));
				}
			}
			else
			{
				GL_CHECK(glDisableVertexAttribArray(attrLoc));
				GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));
			}
		}

		//int disableCount = m_lastUsedAttribIndex - iElement;
		//if (disableCount > 0)
		//{
		//	for (int i = 0; i < disableCount; i++)
		//	{
		//		GL_CHECK(glDisableVertexAttribArray(attrLoc));
		//		GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));
		//	}
		//}

		//m_lastUsedAttribIndex = iElement;
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

void GLGraphicsContext::onUpdateShaderPass(IShaderPass* newPass)
{
	if (newPass)
	{
		GLShaderPass* pass = static_cast<GLShaderPass*>(newPass);
		pass->apply();
	}
}

void* GLGraphicsContext::onMapResource(IGraphicsResource* resource)
{
	switch (resource->resourceType())
	{
	case DeviceResourceType::VertexBuffer:
		return static_cast<GLVertexBuffer*>(resource)->map();
	case DeviceResourceType::IndexBuffer:
		return static_cast<GLIndexBuffer*>(resource)->map();
	default:
		LN_NOTIMPLEMENTED();
		return nullptr;
	}
}

void GLGraphicsContext::onUnmapResource(IGraphicsResource* resource)
{
	switch (resource->resourceType())
	{
	case DeviceResourceType::VertexBuffer:
		static_cast<GLVertexBuffer*>(resource)->unmap();
		break;
	case DeviceResourceType::IndexBuffer:
		static_cast<GLIndexBuffer*>(resource)->unmap();
		break;
	default:
		LN_NOTIMPLEMENTED();
		break;
	}
}

void GLGraphicsContext::onSetSubData(IGraphicsResource* resource, size_t offset, const void* data, size_t length)
{
	switch (resource->resourceType())
	{
	case DeviceResourceType::VertexBuffer:
		static_cast<GLVertexBuffer*>(resource)->setSubData(offset, data, length);
		break;
	case DeviceResourceType::IndexBuffer:
		static_cast<GLIndexBuffer*>(resource)->setSubData(offset, data, length);
		break;
	default:
		LN_NOTIMPLEMENTED();
		break;
	}
}

void GLGraphicsContext::onSetSubData2D(ITexture* resource, int x, int y, int width, int height, const void* data, size_t dataSize)
{
	static_cast<GLTextureBase*>(resource)->setSubData(x, y, width, height, data, dataSize);
}

void GLGraphicsContext::onSetSubData3D(ITexture* resource, int x, int y, int z, int width, int height, int depth, const void* data, size_t dataSize)
{
	static_cast<GLTextureBase*>(resource)->setSubData3D(x, y, z, width, height, depth, data, dataSize);
}

void GLGraphicsContext::onClearBuffers(ClearFlags flags, const Color& color, float z, uint8_t stencil)
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
		GL_CHECK(glClearDepth(z));
		glflags |= GL_DEPTH_BUFFER_BIT;
	}

	if (testFlag(flags, ClearFlags::Stencil))
	{
		GL_CHECK(glClearStencil(stencil));
		glflags |= GL_STENCIL_BUFFER_BIT;
	}

	// Lumino の仕様としては、Viewport や Scissor の影響を受けないようにクリアしたい。
	// しかし glClear は Scissor の影響を受けるので GL_SCISSOR_TEST を切っておく。
	GL_CHECK(glDisable(GL_SCISSOR_TEST));

	GL_CHECK(glClear(glflags));

    //GLint c[] = { 255, 0, 0, 255 };
    //GL_CHECK(glClearBufferiv(GL_COLOR, GL_DRAW_BUFFER0, c));
    //GLuint c[] = { 255, 0, 0, 255 };
    //GL_CHECK(glClearBufferuiv(GL_COLOR, GL_DRAW_BUFFER0, c));
}

void GLGraphicsContext::onDrawPrimitive(PrimitiveTopology primitive, int startVertex, int primitiveCount)
{
	GLenum gl_prim;
	int vertexCount;
	getPrimitiveInfo(primitive, primitiveCount, &gl_prim, &vertexCount);

	GL_CHECK(glDrawArrays(gl_prim, startVertex, vertexCount));
}

void GLGraphicsContext::onDrawPrimitiveIndexed(PrimitiveTopology primitive, int startIndex, int primitiveCount)
{
	GLenum gl_prim;
	int vertexCount;
	getPrimitiveInfo(primitive, primitiveCount, &gl_prim, &vertexCount);

	// 引数 start end には、本来であれば0～vertexCountまでのインデックスの中の最大、最小の値を渡す。
	// http://wiki.livedoor.jp/mikk_ni3_92/d/glDrawRangeElements%A4%CB%A4%E8%A4%EB%C9%C1%B2%E8
	// ただ、全範囲を渡しても特に問題なさそうなのでこのまま。
	if (m_currentIndexBuffer->format() == IndexBufferFormat::UInt16)
	{
		GL_CHECK(glDrawElements(gl_prim, vertexCount, GL_UNSIGNED_SHORT, (GLvoid*)(sizeof(GLushort) * startIndex)));
	}
	else
	{
		GL_CHECK(glDrawElements(gl_prim, vertexCount, GL_UNSIGNED_INT, (GLvoid*)(sizeof(GLuint) * startIndex)));
	}
}

void GLGraphicsContext::onPresent(ISwapChain* swapChain)
{
	auto* s = static_cast<GLSwapChain*>(swapChain);

	SizeI endpointSize;
	s->getBackendBufferSize(&endpointSize);

	SizeI bufferSize = s->getRenderTarget(0)->realSize();



	// SwapChain の Framebuffer をウィンドウのバックバッファへ転送
	{
		GL_CHECK(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, s->defaultFBO()));
		GL_CHECK(glBindFramebuffer(GL_READ_FRAMEBUFFER, s->fbo()));

		//LN_LOG_INFO << endpointSize.width << ", " << endpointSize.height << ":" << bufferSize.width << ", " << bufferSize.height;

		// FIXME:
		// Viewport を転送元に合わせないと、転送先全体に拡大してBlitできなかった。
		// ちょっと腑に落ちないが・・・。
		GL_CHECK(glDisable(GL_SCISSOR_TEST));
		GL_CHECK(glScissor(0, 0, bufferSize.width, bufferSize.height));
		GL_CHECK(glViewport(0, 0, bufferSize.width, bufferSize.height));

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

		GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, s->defaultFBO()));
	}

	m_device->glContext()->swap(s);
}

void GLGraphicsContext::getPrimitiveInfo(PrimitiveTopology primitive, int primitiveCount, GLenum* gl_prim, int* vertexCount)
{
	switch (primitive)
	{
	case PrimitiveTopology::TriangleList:
		*gl_prim = GL_TRIANGLES;
		*vertexCount = primitiveCount * 3;
		break;
	case PrimitiveTopology::TriangleStrip:
		*gl_prim = GL_TRIANGLE_STRIP;
		*vertexCount = 2 + primitiveCount;
		break;
	case PrimitiveTopology::TriangleFan:
		*gl_prim = GL_TRIANGLE_FAN;
		*vertexCount = 2 + primitiveCount;
		break;
	case PrimitiveTopology::LineList:
		*gl_prim = GL_LINES;
		*vertexCount = primitiveCount * 2;
		break;
	case PrimitiveTopology::LineStrip:
		*gl_prim = GL_LINE_STRIP;
		*vertexCount = 1 + primitiveCount;
		break;
	case PrimitiveTopology::PointList:
		*gl_prim = GL_POINTS;
		*vertexCount = primitiveCount;
		break;
	default:
		LN_UNREACHABLE();
		break;
	}
}

//=============================================================================
// GLContext

GLContext::GLContext()
{
}

//=============================================================================
// GLSwapChain

GLSwapChain::GLSwapChain()
	: m_backbuffer(nullptr)
	, m_fbo(0)
    , m_defaultFBO(0)
    , m_backengBufferWidth(0)
    , m_backengBufferHeight(0)
{
}

void GLSwapChain::dispose()
{
	releaseBuffers();
	ISwapChain::dispose();
}

void GLSwapChain::releaseBuffers()
{
	if (m_fbo)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDeleteFramebuffers(1, &m_fbo);
		m_fbo = 0;
	}

	if (m_backbuffer)
	{
		m_backbuffer->dispose();
		m_backbuffer = nullptr;
	}
}

void GLSwapChain::getBackendBufferSize(SizeI* outSize)
{
    outSize->width = m_backengBufferWidth;
    outSize->height = m_backengBufferHeight;
}

void GLSwapChain::genBackbuffer(uint32_t width, uint32_t height)
{
	releaseBuffers();

	m_backbuffer = makeRef<GLRenderTargetTexture>();
	m_backbuffer->init(width, height, TextureFormat::RGB8, false);

	GL_CHECK(glGenFramebuffers(1, &m_fbo));
	GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, m_fbo));

	GL_CHECK(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_backbuffer->id(), 0));

	LN_ENSURE(GL_FRAMEBUFFER_COMPLETE == glCheckFramebufferStatus(GL_FRAMEBUFFER),
		"glCheckFramebufferStatus failed 0x%08x",
		glCheckFramebufferStatus(GL_FRAMEBUFFER));

	GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

ITexture* GLSwapChain::getRenderTarget(int imageIndex) const
{
	return m_backbuffer;
}

Result GLSwapChain::resizeBackbuffer(uint32_t width, uint32_t height)
{
	genBackbuffer(width, height);
	setBackendBufferSize(width, height);
	return true;
}

void GLSwapChain::setBackendBufferSize(int width, int height)
{
    m_backengBufferWidth = width;
    m_backengBufferHeight = height;
}

//=============================================================================
// EmptyGLContext

Ref<GLSwapChain> EmptyGLContext::createSwapChain(PlatformWindow* window, const SizeI& backbufferSize)
{
	auto ptr = makeRef<EmptyGLSwapChain>();
    ptr->setBackendBufferSize(backbufferSize.width, backbufferSize.height);
	ptr->genBackbuffer(backbufferSize.width, backbufferSize.height);
	return ptr;
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

void GLVertexDeclaration::init(const VertexElement* elements, int elementsCount)
{
	if (LN_REQUIRE(elements != nullptr)) return;
	if (LN_REQUIRE(elementsCount >= 0)) return;

	// 頂点宣言作成
	createGLVertexElements(elements, elementsCount, &m_vertexElements);
}

void GLVertexDeclaration::dispose()
{
	IVertexDeclaration::dispose();
}

void GLVertexDeclaration::createGLVertexElements(const VertexElement* vertexElements, int elementsCount, List<GLVertexElement>* outList)
{
	outList->reserve(elementsCount);

	int sizeInStream[16] = {0};

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

		elm.byteOffset = sizeInStream[elm.streamIndex];
		sizeInStream[elm.streamIndex] += GraphicsHelper::getVertexElementTypeSize(vertexElements[i].Type);

		//elm.stride = getVertexSize(vertexElements, elementsCount, elm.streamIndex);
		//elm.byteOffset = totalSize;
		outList->add(elm);

		//totalSize += getVertexElementTypeSize(vertexElements[i].Type);
	}

	for (auto& e : *outList)
	{
		e.stride = sizeInStream[e.streamIndex];
	}
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
	, m_usage(GraphicsResourceUsage::Static)
	//, m_format(GraphicsResourceUsage::Static)
	, m_size(0)
{
}

GLVertexBuffer::~GLVertexBuffer()
{
}

void GLVertexBuffer::init(GraphicsResourceUsage usage, size_t bufferSize, const void* initialData)
{
	//m_format = usage;
	m_usage = usage;
	m_size = bufferSize;
	//m_byteCount = bufferSize;
	//m_data = LN_NEW byte_t[m_byteCount];
	//if (initialData) {
	//	memcpy_s(m_data, m_byteCount, initialData, m_byteCount);
	//}
	//else {
	//	memset(m_data, 0, m_byteCount);
	//}

	//if (m_format == GraphicsResourceUsage::Dynamic) {
	//	m_usage = GL_DYNAMIC_DRAW;
	//}
	//else {
	//	m_usage = GL_STATIC_DRAW;
	//}

	GL_CHECK(glGenBuffers(1, &m_glVertexBuffer));
	GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, m_glVertexBuffer));
	GL_CHECK(glBufferData(GL_ARRAY_BUFFER, bufferSize, initialData, (m_usage == GraphicsResourceUsage::Static) ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW));
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

void* GLVertexBuffer::map()
{
	GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, m_glVertexBuffer));
	void* buffer;
	//GL_CHECK(buffer = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
	//GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));

	// https://developer.apple.com/jp/documentation/3DDrawing/Conceptual/OpenGLES_ProgrammingGuide/TechniquesforWorkingwithVertexData/TechniquesforWorkingwithVertexData.html


	GL_CHECK(buffer = glMapBufferRange(GL_ARRAY_BUFFER, 0, m_size, GL_MAP_WRITE_BIT));


	//GLbitfield flags = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;
	//glBufferStorage(GL_ARRAY_BUFFER, DYN_VB_SIZE, NULL, flags);
	//m_dyn_vb_ptr = glMapBufferRange(GL_ARRAY_BUFFER, 0, DYN_VB_SIZE, flags);

	return buffer;
}

void GLVertexBuffer::unmap()
{
	GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, m_glVertexBuffer));
	GL_CHECK(glUnmapBuffer(GL_ARRAY_BUFFER));
	//GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));
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
	, m_format(IndexBufferFormat::UInt16)
	, m_usage(GraphicsResourceUsage::Static)
	, m_size(0)
{
}

GLIndexBuffer::~GLIndexBuffer()
{
}

void GLIndexBuffer::init(GraphicsResourceUsage usage, IndexBufferFormat format, int indexCount, const void* initialData)
{
	m_format = format;
	m_usage = usage;
	int stride = (m_format == IndexBufferFormat::UInt16) ? 2 : 4;
	m_size = stride * indexCount;

	GL_CHECK(glGenBuffers(1, &m_indexBufferId));
	GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferId));
	GL_CHECK(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_size, initialData, (m_usage == GraphicsResourceUsage::Static) ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW));
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

void* GLIndexBuffer::map()
{
	GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferId));
	void* buffer;
	//GL_CHECK(buffer = glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY));
	GL_CHECK(buffer = glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, 0, m_size, GL_MAP_WRITE_BIT));
	return buffer;
}

void GLIndexBuffer::unmap()
{
	GL_CHECK(glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER));
	//GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

//=============================================================================
// GLTexture2D

GLTexture2D::GLTexture2D()
	: m_id(0)
	, m_usage(GraphicsResourceUsage::Static)
	, m_size(0, 0)
	, m_textureFormat(TextureFormat::Unknown)
	, m_pixelFormat(0)
	, m_elementType(0)
    , m_mipmap(false)
{
}

GLTexture2D::~GLTexture2D()
{
}

void GLTexture2D::init(GraphicsResourceUsage usage, uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap, const void* initialData)
{
	m_usage = usage;
	m_size = SizeI(width, height);
	m_textureFormat = requestFormat;
    m_mipmap = mipmap;

	// http://marina.sys.wakayama-u.ac.jp/~tokoi/?date=20040914

    GLint levels = 0;// (m_mipmap) ? 4 : 0;	// TODO:DirectX だと 0 の場合は全レベル作成するけど、今ちょっと計算めんどうなので 

	GLenum internalFormat;
	OpenGLHelper::getGLTextureFormat(requestFormat, &internalFormat, &m_pixelFormat, &m_elementType);

	// テクスチャ作成
	GL_CHECK(glGenTextures(1, &m_id));
	GL_CHECK(glBindTexture(GL_TEXTURE_2D, m_id));
	GL_CHECK(glTexImage2D(GL_TEXTURE_2D, levels, internalFormat, m_size.width, m_size.height, 0, m_pixelFormat, m_elementType, initialData));

    //if (mipmap) {
    //    glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
    //}

	// デフォルトのサンプラステート (セットしておかないとサンプリングできない)
	//setGLSamplerState(m_samplerState);
	//{

		GLint filter[] =
		{
			GL_NEAREST,			// TextureFilterMode_Point,
			GL_LINEAR,			// TextureFilterMode_Linear,
		};
	//	GLint wrap[] =
	//	{
	//		GL_REPEAT,			// TextureWrapMode_Repeat
	//		GL_CLAMP_TO_EDGE,	// TextureWrapMode_Clamp
	//	};

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter[1]);
		//if (LN_ENSURE_GLERROR()) return;
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter[1]);
		//if (LN_ENSURE_GLERROR()) return;

	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap[1]);
	//	//if (LN_ENSURE_GLERROR()) return;
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap[1]);
	//	//if (LN_ENSURE_GLERROR()) return;
	//}

    if (m_mipmap) {
        GL_CHECK(glGenerateMipmap(GL_TEXTURE_2D));
    }


	GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0));
}

void GLTexture2D::dispose()
{
	if (m_id != 0)
	{
		GL_CHECK(glDeleteTextures(1, &m_id));
		m_id = 0;
	}

	GLTextureBase::dispose();
}

void GLTexture2D::readData(void* outData)
{
	LN_UNREACHABLE();
}

SizeI GLTexture2D::realSize()
{
	return m_size;
}

TextureFormat GLTexture2D::getTextureFormat() const
{
	return m_textureFormat;
}

void GLTexture2D::setSubData(int x, int y, int width, int height, const void* data, size_t dataSize)
{
	GL_CHECK(glBindTexture(GL_TEXTURE_2D, m_id));

    GLint levels = (m_mipmap) ? 4 : 0;	// TODO:DirectX だと 0 の場合は全レベル作成するけど、今ちょっと計算めんどうなので 

	/* テクスチャ画像はバイト単位に詰め込まれている */
	//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	GL_CHECK(glTexSubImage2D(
		GL_TEXTURE_2D,
        levels,
		x,
		/*m_realSize.Height - */y,
		width,
		height,
		m_pixelFormat,
		m_elementType,
		data));

    if (m_mipmap) {
        GL_CHECK(glGenerateMipmap(GL_TEXTURE_2D));
    }
    

	GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0));
}

void GLTexture2D::setSubData3D(int x, int y, int z, int width, int height, int depth, const void* data, size_t dataSize)
{
	LN_UNREACHABLE();
}

//=============================================================================
// GLTexture3D

GLTexture3D::GLTexture3D()
	: m_id(0)
	, m_usage(GraphicsResourceUsage::Static)
	, m_width(0)
	, m_height(0)
	, m_depth(0)
	, m_textureFormat(TextureFormat::Unknown)
	, m_pixelFormat(0)
	, m_elementType(0)
{
}

GLTexture3D::~GLTexture3D()
{
}

void GLTexture3D::init(GraphicsResourceUsage usage, uint32_t width, uint32_t height, uint32_t depth, TextureFormat requestFormat, bool mipmap, const void* initialData)
{
	m_usage = usage;
	m_width = width;
	m_height = height;
	m_depth = depth;
	m_textureFormat = requestFormat;
	GLint levels = (mipmap) ? 4 : 0;	// TODO:DirectX だと 0 の場合は全レベル作成するけど、今ちょっと計算めんどうなので 

	GLenum internalFormat;
	OpenGLHelper::getGLTextureFormat(requestFormat, &internalFormat, &m_pixelFormat, &m_elementType);

	GLuint tex;
	GL_CHECK(glGenTextures(1, &m_id));
	GL_CHECK(glBindTexture(GL_TEXTURE_3D, m_id));
	GL_CHECK(glTexImage3D(GL_TEXTURE_3D, levels, internalFormat, m_width, m_height, m_depth, 0, m_pixelFormat, m_elementType, initialData));

	//// テクスチャの拡大・縮小に線形補間を用いる
	//glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	//// テクスチャからはみ出た部分には境界色を用いる
	//glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	//glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	//glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);

	// テクスチャの境界色を設定する (ボリュームの外には何もない)
	//static const GLfloat black[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	//glTexParameterfv(GL_TEXTURE_3D, GL_TEXTURE_BORDER_COLOR, black);

	GL_CHECK(glBindTexture(GL_TEXTURE_3D, 0));
}

void GLTexture3D::dispose()
{
	if (m_id != 0)
	{
		GL_CHECK(glDeleteTextures(1, &m_id));
		m_id = 0;
	}

	GLTextureBase::dispose();
}

void GLTexture3D::readData(void* outData)
{
	LN_UNREACHABLE();
}

SizeI GLTexture3D::realSize()
{
	LN_NOTIMPLEMENTED();
	return SizeI::Zero;
}

TextureFormat GLTexture3D::getTextureFormat() const
{
	return m_textureFormat;
}

void GLTexture3D::setSubData(int x, int y, int width, int height, const void* data, size_t dataSize)
{
	LN_UNREACHABLE();
}

void GLTexture3D::setSubData3D(int x, int y, int z, int width, int height, int depth, const void* data, size_t dataSize)
{
	GL_CHECK(glBindTexture(GL_TEXTURE_3D, m_id));
	GL_CHECK(glTexSubImage3D(GL_TEXTURE_3D,
		0,	// TODO: Mipmap
		x, y, z, width, height, depth, m_pixelFormat, m_elementType, data));
	GL_CHECK(glBindTexture(GL_TEXTURE_3D, 0));
}

//=============================================================================
// GLRenderTargetTexture

GLRenderTargetTexture::GLRenderTargetTexture()
{
}

GLRenderTargetTexture::~GLRenderTargetTexture()
{
}

void GLRenderTargetTexture::init(uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap)
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

//	internalFormat = GL_RGB;
//	m_pixelFormat = GL_RGB;
//	m_elementType = GL_UNSIGNED_BYTE;

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
	//GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	//GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	//GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	//GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

	GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0));
}

void GLRenderTargetTexture::init(intptr_t nativeObject, uint32_t hintWidth, uint32_t hintHeight)
{
    m_id = nativeObject;
    m_size = SizeI(hintWidth, hintHeight);
    m_textureFormat = TextureFormat::Unknown;
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
#ifdef GL_GLES_PROTOTYPES
	// OpenGL ES is glGetTexImage unsupported
	// http://oppyen.hatenablog.com/entry/2016/10/21/071612
#else
	GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	GL_CHECK(glBindTexture(GL_TEXTURE_2D, m_id));
	GL_CHECK(glGetTexImage(GL_TEXTURE_2D, 0, m_pixelFormat, m_elementType, outData));
	GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0));
#endif
}

SizeI GLRenderTargetTexture::realSize()
{
	return m_size;
}

TextureFormat GLRenderTargetTexture::getTextureFormat() const
{
	return m_textureFormat;
}

void GLRenderTargetTexture::setSubData(int x, int y, int width, int height, const void* data, size_t dataSize)
{
	LN_UNREACHABLE();
}

void GLRenderTargetTexture::setSubData3D(int x, int y, int z, int width, int height, int depth, const void* data, size_t dataSize)
{
	LN_UNREACHABLE();
}

//=============================================================================
// GLDepthBuffer

GLDepthBuffer::GLDepthBuffer()
	: m_id(0)
{
}

void GLDepthBuffer::init(uint32_t width, uint32_t height)
{
	if (LN_REQUIRE(!m_id)) return;
	GL_CHECK(glGenRenderbuffers(1, &m_id));
	GL_CHECK(glBindRenderbuffer(GL_RENDERBUFFER, m_id));
	GL_CHECK(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height));
	GL_CHECK(glBindRenderbuffer(GL_RENDERBUFFER, 0));
}

void GLDepthBuffer::dispose()
{
	if (m_id != 0) {
		GL_CHECK(glDeleteRenderbuffers(1, &m_id));
		m_id = 0;
	}
	IDepthBuffer::dispose();
}

//=============================================================================
// GLSamplerState

GLSamplerState::GLSamplerState()
	: m_id(0)

{
}

GLSamplerState::~GLSamplerState()
{
}

void GLSamplerState::init(OpenGLDevice* device, const SamplerStateData& desc)
{
    m_desc = desc;

    static const GLenum magFilterTable[] =
    {
        GL_NEAREST, // TextureFilterMode::Point,
        GL_LINEAR,  // TextureFilterMode::Linear,
    };

    static const GLenum minFilterTable[][2] =
    {
        { GL_NEAREST, GL_NEAREST_MIPMAP_NEAREST },  // TextureFilterMode::Point,
        { GL_LINEAR,  GL_LINEAR_MIPMAP_LINEAR  },   // TextureFilterMode::Linear,
    };
    const GLint addressTable[] =
    {
        GL_REPEAT,			// TextureAddressMode::Repeat
        GL_CLAMP_TO_EDGE,	// TextureAddressMode::Clamp
    };

	GL_CHECK(glGenSamplers(1, &m_id));
    GL_CHECK(glSamplerParameteri(m_id, GL_TEXTURE_MAG_FILTER, magFilterTable[static_cast<int>(desc.filter)]));
	GL_CHECK(glSamplerParameteri(m_id, GL_TEXTURE_MIN_FILTER, minFilterTable[static_cast<int>(desc.filter)][0]));
	GL_CHECK(glSamplerParameteri(m_id, GL_TEXTURE_WRAP_S, addressTable[static_cast<int>(desc.address)]));
	GL_CHECK(glSamplerParameteri(m_id, GL_TEXTURE_WRAP_T, addressTable[static_cast<int>(desc.address)]));
	if (m_desc.anisotropy && device->caps().support_filter_anisotropic) {
		GL_CHECK(glSamplerParameterf(m_id, GL_TEXTURE_MAX_ANISOTROPY_EXT, device->caps().MAX_TEXTURE_MAX_ANISOTROPY_EXT));
	}

    GL_CHECK(glGenSamplers(1, &m_idMip));
    GL_CHECK(glSamplerParameteri(m_idMip, GL_TEXTURE_MAG_FILTER, magFilterTable[static_cast<int>(desc.filter)]));
    GL_CHECK(glSamplerParameteri(m_idMip, GL_TEXTURE_MIN_FILTER, minFilterTable[static_cast<int>(desc.filter)][1]));
    GL_CHECK(glSamplerParameteri(m_idMip, GL_TEXTURE_WRAP_S, addressTable[static_cast<int>(desc.address)]));
    GL_CHECK(glSamplerParameteri(m_idMip, GL_TEXTURE_WRAP_T, addressTable[static_cast<int>(desc.address)]));
	if (m_desc.anisotropy && device->caps().support_filter_anisotropic) {
		GL_CHECK(glSamplerParameterf(m_idMip, GL_TEXTURE_MAX_ANISOTROPY_EXT, device->caps().MAX_TEXTURE_MAX_ANISOTROPY_EXT));
	}

}

void GLSamplerState::dispose()
{
	if (m_id) {
		GL_CHECK(glDeleteSamplers(1, &m_id));
		m_id = 0;
	}
    if (m_idMip) {
        GL_CHECK(glDeleteSamplers(1, &m_idMip));
        m_idMip = 0;
    }
	ISamplerState::dispose();
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

    //const char* extcode =
    //    "vec4 xxTexture(int isRT, sampler2D s, vec2 uv) { if (isRT != 0) { return texture(s, vec2(uv.x, (uv.y * -1.0) + 1.0)); } else { return texture(s, uv); } }\n"
    //    "#define texture(s, uv) xxTexture(s#_IsRT, s, uv)\n";

    const GLchar* codes[] = {
        //extcode,
        (const GLchar*)code,
    };

	GLint codeSize[] = {
        //strlen(extcode),
        length,
    };
	GL_CHECK(glShaderSource(m_shader, LN_ARRAY_SIZE_OF(codeSize), codes, codeSize));
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

GLShaderPass::~GLShaderPass()
{
}

void GLShaderPass::init(OpenGLDevice* context, const byte_t* vsCode, int vsCodeLen, const byte_t* fsCode, int fsCodeLen, ShaderCompilationDiag* diag)
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
	for (auto& p : m_uniformBuffers) {
		p->dispose();
	}
	m_uniformBuffers.clear();

	for (auto& p : m_uniforms) {
		p->dispose();
	}
	m_uniforms.clear();

	if (m_samplerBuffer) {
		m_samplerBuffer->dispose();
		m_samplerBuffer = nullptr;
	}

	if (m_program)
	{
		GL_CHECK(glUseProgram(0));
		GL_CHECK(glDeleteProgram(m_program));
		m_program = 0;
	}

	IShaderPass::dispose();
}

void GLShaderPass::apply()
{
	GL_CHECK(glUseProgram(m_program));
	for (auto& buf : m_uniformBuffers) {
		buf->bind(m_program);
	}

	if (m_samplerBuffer) {
		m_samplerBuffer->bind();
	}
}

//int GLShaderPass::getUniformCount() const
//{
//	return m_uniforms.size();
//}
//
//IShaderUniform* GLShaderPass::getUniform(int index) const
//{
//	return m_uniforms[index];
//}
//
//void GLShaderPass::setUniformValue(int index, const void* data, size_t size)
//{
//	m_context->setActiveShaderPass(this);
//	m_uniforms[index]->setUniformValue(m_context, data, size);
//}

int GLShaderPass::getUniformBufferCount() const
{
	return m_uniformBuffers.size();
}

IShaderUniformBuffer* GLShaderPass::getUniformBuffer(int index) const
{
	return m_uniformBuffers[index];
}

IShaderSamplerBuffer* GLShaderPass::samplerBuffer() const
{
	return m_samplerBuffer;
}

void GLShaderPass::buildUniforms()
{
	m_samplerBuffer = makeRef<GLLocalShaderSamplerBuffer>();

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
			&desc.type2, &desc.rows, &desc.columns, &desc.elements);

		auto uniform = makeRef<GLShaderUniform>(desc, name, loc);
		m_uniforms.add(uniform);

		if (desc.type2 == ShaderUniformType_Texture)
		{
			m_samplerBuffer->addGlslSamplerUniform(name, loc);
		}


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

	// lnIsRT 用にもう一度回す
	for (int i = 0; i < count; i++)
	{
		GLsizei name_len = 0;
		GLsizei var_size = 0;
		GLenum  var_type = 0;
		GLchar  name[256] = { 0 };
		GL_CHECK(glGetActiveUniform(m_program, i, 256, &name_len, &var_size, &var_type, name));
		GLint loc = glGetUniformLocation(m_program, name);

		//if (strncmp(name, LN_CIS_PREFIX, std::strlen(LN_CIS_PREFIX)) == 0 && strncmp(name + name_len - std::strlen(LN_IS_RT_POSTFIX), LN_IS_RT_POSTFIX, std::strlen(LN_IS_RT_POSTFIX)) == 0)
        if (strncmp(name + name_len - std::strlen(LN_IS_RT_POSTFIX), LN_IS_RT_POSTFIX, std::strlen(LN_IS_RT_POSTFIX)) == 0)
		{
			m_samplerBuffer->addIsRenderTargetUniform(name, loc);
		}
	}

	GL_CHECK(glGetProgramiv(m_program, GL_ACTIVE_UNIFORM_BLOCKS, &count));
	for (int i = 0; i < count; i++)
	{
		GLchar blockName[128];
		GLsizei blockNameLen;
		GL_CHECK(glGetActiveUniformBlockName(m_program, i, 128, &blockNameLen, blockName));

		GLuint blockIndex = glGetUniformBlockIndex(m_program, blockName);

		GLint blockSize;
		GL_CHECK(glGetActiveUniformBlockiv(m_program, blockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize));
		
		auto uniformBlock = makeRef<GLShaderUniformBuffer>(blockName, blockIndex, blockSize, i);
		m_uniformBuffers.add(uniformBlock);

		LN_LOG_VERBOSE << "uniform block " << i;
		LN_LOG_VERBOSE << "  blockName  : " << blockName;
		LN_LOG_VERBOSE << "  blockIndex : " << blockIndex;
		LN_LOG_VERBOSE << "  blockSize  : " << blockSize;

		GLint blockMemberCount;
		GL_CHECK(glGetActiveUniformBlockiv(m_program, blockIndex, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &blockMemberCount));

		GLint indices[32];
		GL_CHECK(glGetActiveUniformBlockiv(m_program, blockIndex, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, indices));

		GLint offsets[32];
		GL_CHECK(glGetActiveUniformsiv(m_program, blockMemberCount, (const GLuint*)indices, GL_UNIFORM_OFFSET, offsets));

		//GLint elements[32];
		//glGetActiveUniformsiv(m_program, blockMemberCount, (const GLuint*)indices, GL_UNIFORM_SIZE, elements);

		GLint arrayStrides[32];
		GL_CHECK(glGetActiveUniformsiv(m_program, blockMemberCount, (const GLuint*)indices, GL_UNIFORM_ARRAY_STRIDE, arrayStrides));

        // 列間、または行間の stride (バイト単位)
		GLint matrixStrides[32];
		GL_CHECK(glGetActiveUniformsiv(m_program, blockMemberCount, (const GLuint*)indices, GL_UNIFORM_MATRIX_STRIDE, matrixStrides));
		
		GLint isRowMajors[32];
		GL_CHECK(glGetActiveUniformsiv(m_program, blockMemberCount, (const GLuint*)indices, GL_UNIFORM_IS_ROW_MAJOR, isRowMajors));

		for (int iMember = 0; iMember < blockMemberCount; iMember++)
		{
			GLsizei nameLen = 0;
			GLsizei size = 0;
			GLenum  type = 0;
			GLchar  name[256] = { 0 };
			GL_CHECK(glGetActiveUniform(m_program, indices[iMember], 256, &nameLen, &size, &type, name));

			ShaderUniformTypeDesc desc;
			OpenGLHelper::convertVariableTypeGLToLN(
				name, type, size,
				&desc.type2, &desc.rows, &desc.columns, &desc.elements);

			size_t dataSize;
			if (iMember < blockMemberCount - 1) {
				dataSize = offsets[iMember + 1] - offsets[iMember];
			}
			else {
				dataSize = blockSize - offsets[iMember];
			}

			desc.offset = offsets[iMember];
			//desc.size = dataSize;
			desc.arrayStride = arrayStrides[iMember];
			desc.matrixStride = matrixStrides[iMember];

			// 検証。縦の大きさはデータサイズから求めたものと一致するはず
			if (desc.matrixStride > 0 && desc.elements > 0) {

                // https://www.opengl.org/archives/resources/faq/technical/transformations.htm
                // https://stackoverflow.com/questions/17717600/confusion-between-c-and-opengl-matrix-order-row-major-vs-column-major
                // mat3x4 の場合、col=3, row=4
                // 実際のレイアウトは DirectX と同じ。GLSL で演算するときの意味が column_major であるか、という違い。
				// float[] = {
                //   { 0,  1,  2,  3, },
                //   { 4,  5,  6,  7, },
                //   { 8,  9, 10, 11, },
                //   { x,  x,  x,  x, },	// ここは使われないので、全体サイズは 48 byte となる
				// };
                // mat3x4[3] の場合、dataSize は 144。elements は 3。matrixStride は 16。
                // [2019/3/4] GeForce GTX 1060 で確認。

                if (isRowMajors[iMember]) {
					// Radeon HD 8490 で確認。
					// GLSL でレイアウトを指定しない場合のデフォルトのメモリレイアウトは row_major であった。
					// float[] = {
					//   { 0,  4,  8, x, },
					//   { 1,  5,  9, x, },
					//   { 2,  6, 10, x, },
					//   { 3,  7, 11, x, },
					//             // ^ この列は使われないが、領域としては確保されている。全体サイズは 64byte となる。
					// };
					// dataSize=192, elements=3, matrixStride=16
					// mat3x4 の場合、col=3, row=4 ← これは GeForce と同じ。
                    assert(desc.rows == dataSize / desc.elements / desc.matrixStride);
                }
                else {
                    // OpenGL default
                    assert(desc.columns == dataSize / desc.elements / desc.matrixStride);
                    
                     //0 3 6 9
                     //1 4 7 10
                     //2 5 8 11
                     //x x x x
                }
			}

			char* localName = name;
			if (strstr(localName, blockName) && localName[blockNameLen] == '.') {
				// "Buffer.g_value" といったように、ブロック名が先頭についていることがあるので取り除いておく
				localName += blockNameLen + 1;
			}
			char* bracket = strstr(localName, "[");
			if (bracket) {
				// 配列変数は "g_ary[0]" というような名前で出てくるので、後ろを削る。
				*bracket = '\0';
			}

			auto uniform = makeRef<GLShaderUniform>(desc, localName, offsets[iMember]);
			uniformBlock->addUniform(uniform);

			LN_LOG_VERBOSE << "uniform " << iMember;
			LN_LOG_VERBOSE << "  name          : " << name;
			LN_LOG_VERBOSE << "  index         : " << indices[iMember];	// uniform location (unique in program)
			LN_LOG_VERBOSE << "  offset        : " << offsets[iMember];
			LN_LOG_VERBOSE << "  array stride  : " << arrayStrides[iMember];
			LN_LOG_VERBOSE << "  matrix stride : " << matrixStrides[iMember];
			LN_LOG_VERBOSE << "  type          : " << type;
			LN_LOG_VERBOSE << "  elements      : " << size;
			LN_LOG_VERBOSE << "  rows          : " << desc.rows;
			LN_LOG_VERBOSE << "  columns       : " << desc.columns;
			LN_LOG_VERBOSE << "  row majors    : " << isRowMajors[iMember];
			LN_LOG_VERBOSE << "  data size     : " << dataSize;
		}
	}
}

//=============================================================================
// GLShaderUniformBuffer

GLShaderUniformBuffer::GLShaderUniformBuffer(const GLchar* blockName, GLuint blockIndex, GLint blockSize, GLuint bindingPoint)
	: m_name(blockName)
	, m_blockIndex(blockIndex)
	, m_blockSize(blockSize)
	, m_bindingPoint(bindingPoint)
	, m_ubo(0)
{
	GL_CHECK(glGenBuffers(1, &m_ubo));
	GL_CHECK(glBindBuffer(GL_UNIFORM_BUFFER, m_ubo));
	GL_CHECK(glBufferData(GL_UNIFORM_BUFFER, m_blockSize, nullptr, GL_DYNAMIC_DRAW));
	GL_CHECK(glBindBuffer(GL_UNIFORM_BUFFER, 0));

	
}

GLShaderUniformBuffer::~GLShaderUniformBuffer()
{
	if (m_ubo)
	{
		GL_CHECK(glDeleteBuffers(1, &m_ubo));
		m_ubo = 0;
	}
}

void GLShaderUniformBuffer::dispose()
{
	for (auto& p : m_uniforms) {
		p->dispose();
	}

	IShaderUniformBuffer::dispose();
}

void GLShaderUniformBuffer::bind(GLuint program)
{
	// m_ubo を context 内の Global なテーブルの m_bindingPoint 番目にセット
	GL_CHECK(glBindBufferBase(GL_UNIFORM_BUFFER, m_bindingPoint, m_ubo));

	// m_bindingPoint 番目にセットされている m_ubo を、m_blockIndex 番目の Uniform Buffer として使う
	GL_CHECK(glUniformBlockBinding(program, m_blockIndex, m_bindingPoint));
}

const std::string& GLShaderUniformBuffer::name() const
{
	return m_name;
}

int GLShaderUniformBuffer::getUniformCount() const
{
	return m_uniforms.size();
}

IShaderUniform* GLShaderUniformBuffer::getUniform(int index) const
{
	return m_uniforms[index];
}

void GLShaderUniformBuffer::setData(const void* data, size_t size)
{
	if (LN_REQUIRE(data)) return;
	if (LN_REQUIRE(size == m_blockSize)) return;
	GL_CHECK(glBindBuffer(GL_UNIFORM_BUFFER, m_ubo));
	GL_CHECK(glBufferSubData(GL_UNIFORM_BUFFER, 0, size, data));
	GL_CHECK(glBindBuffer(GL_UNIFORM_BUFFER, 0));
}


//=============================================================================
// GLShaderUniform

GLShaderUniform::GLShaderUniform(const ShaderUniformTypeDesc& desc, const GLchar* name, GLint location)
	: m_desc(desc)
	, m_name(name)
	, m_location(location)
{
}

GLShaderUniform::~GLShaderUniform()
{
}

void GLShaderUniform::dispose()
{
	IShaderUniform::dispose();
}

void GLShaderUniform::setUniformValue(OpenGLDevice* context, const void* data, size_t size)
{
	LN_CHECK(context);
	LN_CHECK(data);

	MemoryStream* tempBuffer = context->uniformTempBuffer();
	BinaryWriter* tempWriter = context->uniformTempBufferWriter();
	tempWriter->seek(0, SeekOrigin::Begin);


    switch (m_desc.type2)
    {
    case ShaderUniformType_Bool:
        if (!m_desc.isArray()) {
            GL_CHECK(glUniform1i(m_location, *static_cast<const uint8_t*>(data)));
        }
        else {
            const uint8_t* begin = static_cast<const uint8_t*>(data);
            const uint8_t* end = begin + m_desc.elements;
            std::for_each(begin, end, [&tempWriter](bool v) { GLint i = (v) ? 1 : 0; tempWriter->write(&i, sizeof(GLint)); });
            GL_CHECK(glUniform1iv(m_location, m_desc.elements, (const GLint*)tempBuffer->data()));
        }
        break;
    case ShaderUniformType_Int:
        if (!m_desc.isArray()) {
            GL_CHECK(glUniform1i(m_location, *static_cast<const int32_t*>(data)));
        }
        else {
            LN_NOTIMPLEMENTED();
        }
        break;
    case ShaderUniformType_Float:
        if (!m_desc.isArray()) {
            GL_CHECK(glUniform1f(m_location, *static_cast<const float*>(data)));
        }
        else {
            LN_NOTIMPLEMENTED();
        }
        break;
    case ShaderUniformType_Vector:
        if (!m_desc.isArray()) {
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
        }
        else {
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
        }
        break;
    case ShaderUniformType_Matrix:
        if (!m_desc.isArray()) {
            GL_CHECK(glUniformMatrix4fv(m_location, 1, GL_FALSE, static_cast<const GLfloat*>(data)));
        }
        else {
            GL_CHECK(glUniformMatrix4fv(m_location, m_desc.elements, GL_FALSE, static_cast<const GLfloat*>(data)));
        }
    case ShaderUniformType_Texture:
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


//=============================================================================
// GLLocalShaderSamplerBuffer

GLLocalShaderSamplerBuffer::GLLocalShaderSamplerBuffer()
	: m_table()
{
}

void GLLocalShaderSamplerBuffer::addGlslSamplerUniform(const std::string& name, GLint uniformLocation)
{
	// 重複チェック
	auto itr = std::find_if(m_table.begin(), m_table.end(), [&](const Uniform& x) { return x.name == name; });
	if (itr != m_table.end()) {
		LN_ERROR();
		return;
	}

	Uniform uniform;
	uniform.name = name;
	uniform.uniformLocation = uniformLocation;

	auto keyword = name.find(LN_CIS_PREFIX);
	auto textureSep = name.find(LN_TO_PREFIX);
	auto samplerSep = name.find(LN_SO_PREFIX);

	if (keyword != std::string::npos && textureSep != std::string::npos && samplerSep && std::string::npos) {
		// 所定のキーワードを持っていた場合は texture と samplerState に分割して登録

		ExternalUnifrom texture;
		texture.name = name.substr(textureSep + LN_TO_PREFIX_LEN, samplerSep - (textureSep + LN_TO_PREFIX_LEN));
		m_externalUniforms.push_back(texture);
		uniform.m_textureExternalUnifromIndex = m_externalUniforms.size() - 1;

		ExternalUnifrom sampler;
		sampler.name = name.substr(samplerSep + LN_SO_PREFIX_LEN);
		m_externalUniforms.push_back(sampler);
		uniform.m_samplerExternalUnifromIndex = m_externalUniforms.size() - 1;
	}
	else {
		// 所定のキーワードを持たない場合は CombinedSampler

		ExternalUnifrom sampler;
		sampler.name = name;
		m_externalUniforms.push_back(sampler);
		uniform.m_textureExternalUnifromIndex = m_externalUniforms.size() - 1;
        uniform.m_samplerExternalUnifromIndex = m_externalUniforms.size() - 1;
	}

	m_table.push_back(uniform);
}

void GLLocalShaderSamplerBuffer::addIsRenderTargetUniform(const std::string& name, GLint uniformLocation)
{
    //auto keyword = name.find(LN_CIS_PREFIX);
    //auto textureSep = name.find(LN_TO_PREFIX);
    //auto samplerSep = name.find(LN_SO_PREFIX);
	auto rtMark = name.find(LN_IS_RT_POSTFIX);
    if (rtMark != std::string::npos)
    {
		auto targetName = name.substr(0, rtMark);
		auto itr = std::find_if(m_table.begin(), m_table.end(), [&](const Uniform& x) { return x.name == targetName; });
		if (itr != m_table.end()) {
			itr->isRenderTargetUniformLocation = uniformLocation;
		}
		else {
			LN_UNREACHABLE();	// ここには来ないはず
		}

   //     auto textureName = name.substr(textureSep + LN_TO_PREFIX_LEN, samplerSep - (textureSep + LN_TO_PREFIX_LEN));
   //     auto samplerName = name.substr(samplerSep + LN_SO_PREFIX_LEN);
   //     auto itr = std::find_if(m_table.begin(), m_table.end(), [&](const Uniform& x) {
   //         return x.textureRegisterName == textureName && x.samplerRegisterName == samplerName; });
   //     if (itr != m_table.end()) {
   //         itr->isRenderTargetUniformLocation = uniformLocation;
   //     }
   //     else {
			//Uniform e;
   //         e.textureRegisterName = name.substr(textureSep + LN_TO_PREFIX_LEN, samplerSep - (textureSep + LN_TO_PREFIX_LEN));
   //         e.samplerRegisterName = name.substr(samplerSep + LN_SO_PREFIX_LEN);
   //         e.isRenderTargetUniformLocation = uniformLocation;
   //         m_table.push_back(e);
   //     }
    }
	else {
		LN_UNREACHABLE();	// ここには来ないはず
	}
}

void GLLocalShaderSamplerBuffer::bind()
{
	for (int i = 0; i < m_table.size(); i++)
	{
        const Uniform& uniform = m_table[i];
		int unitIndex = i;

		LN_CHECK(uniform.m_textureExternalUnifromIndex >= 0);
		LN_CHECK(uniform.m_samplerExternalUnifromIndex >= 0);
		GLTextureBase* t = m_externalUniforms[uniform.m_textureExternalUnifromIndex].texture;
		GLSamplerState* samplerState = m_externalUniforms[uniform.m_samplerExternalUnifromIndex].samplerState;
		if (!samplerState) {
			samplerState = m_externalUniforms[uniform.m_textureExternalUnifromIndex].samplerState;
		}

		GL_CHECK(glActiveTexture(GL_TEXTURE0 + unitIndex));

        bool mipmap = false;
        bool renderTarget = false;
		if (t) {
			if (t->type() == DeviceTextureType::Texture3D) {
				GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0));
				GL_CHECK(glBindTexture(GL_TEXTURE_3D, t->id()));
			}
			else {
				GL_CHECK(glBindTexture(GL_TEXTURE_2D, t->id()));
				GL_CHECK(glBindTexture(GL_TEXTURE_3D, 0));
			}
            mipmap = t->mipmap();
            renderTarget = (t->type() == DeviceTextureType::RenderTarget);
		}
		else {
			GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0));
			GL_CHECK(glBindTexture(GL_TEXTURE_3D, 0));
		}
		//GL_CHECK(glBindSampler(unitIndex, (entry.samplerState) ? entry.samplerState->resolveId(t->mipmap()) : 0));
        GL_CHECK(glBindSampler(unitIndex, (samplerState) ? samplerState->resolveId(mipmap) : 0));
		GL_CHECK(glUniform1i(uniform.uniformLocation, unitIndex));

        if (uniform.isRenderTargetUniformLocation >= 0) {
            GL_CHECK(glUniform1i(uniform.isRenderTargetUniformLocation, (renderTarget) ? 1 : 0));
            //if (t->type() == DeviceTextureType::RenderTarget) {
            //    GL_CHECK(glUniform1i(entry.isRenderTargetUniformLocation, 1));
            //}
            //else {
            //    GL_CHECK(glUniform1i(entry.isRenderTargetUniformLocation, 0));
            //}
        }
	}
}

int GLLocalShaderSamplerBuffer::registerCount() const
{
	return m_externalUniforms.size();
}

const std::string& GLLocalShaderSamplerBuffer::getTextureRegisterName(int registerIndex) const
{
	return m_externalUniforms[registerIndex].name;
}

//const std::string& GLLocalShaderSamplerBuffer::getSamplerRegisterName(int registerIndex) const
//{
//	return m_externalUniforms[registerIndex].samplerRegisterName;
//}

void GLLocalShaderSamplerBuffer::setTexture(int registerIndex, ITexture* texture)
{
	m_externalUniforms[registerIndex].texture = static_cast<GLTextureBase*>(texture);
}

void GLLocalShaderSamplerBuffer::setSamplerState(int registerIndex, ISamplerState* state)
{
	m_externalUniforms[registerIndex].samplerState = static_cast<GLSamplerState*>(state);
}

} // namespace detail
} // namespace ln

