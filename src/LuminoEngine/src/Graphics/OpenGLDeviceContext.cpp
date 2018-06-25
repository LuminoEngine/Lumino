
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

};


//=============================================================================
// OpenGLDeviceContext

OpenGLDeviceContext::OpenGLDeviceContext()
	: m_glContext(nullptr)
	, m_uniformTempBuffer()
	, m_uniformTempBufferWriter(&m_uniformTempBuffer)
	, m_activeShaderPass(nullptr)
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

}

void OpenGLDeviceContext::dispose()
{
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

Ref<ISwapChain> OpenGLDeviceContext::onCreateSwapChain(PlatformWindow* window, const SizeI& backbufferSize)
{
	return m_glContext->createSwapChain(window, backbufferSize);
}

Ref<IShaderPass> OpenGLDeviceContext::onCreateShaderPass(const byte_t* vsCode, int vsCodeLen, const byte_t* psCode, int psCodeLen, ShaderCompilationDiag* diag)
{
	auto ptr = makeRef<GLShaderPass>();
	ptr->initialize(this, vsCode, vsCodeLen, psCode, psCodeLen, diag);
	return ptr;
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

void OpenGLDeviceContext::onPresent(ISwapChain* swapChain)
{
	m_glContext->swap(static_cast<GLSwapChain*>(swapChain));
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
// GLShaderUniform

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

//=============================================================================
// GLShaderPass

GLShaderPass::GLShaderPass()
	: m_context(nullptr)
	, m_program(0)
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

} // namespace detail
} // namespace ln

