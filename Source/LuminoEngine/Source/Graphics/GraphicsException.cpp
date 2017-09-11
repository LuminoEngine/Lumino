
#include "../Internal.h"
#include "../../include/Lumino/Graphics/GraphicsException.h"
#include "Device/OpenGL/GLCommon.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN

//==============================================================================
// CompilationException
//==============================================================================

//------------------------------------------------------------------------------
CompilationException::CompilationException()
{
}

CompilationException::CompilationException(const ShaderCompileResult& result)
	: CompilationException()
{
	setMessage(result);
}

//------------------------------------------------------------------------------
CompilationException::~CompilationException() throw()
{
}

void CompilationException::setMessage(const ShaderCompileResult& result)
{
	m_message = String::fromCString(result.Message.c_str());
}

////------------------------------------------------------------------------------
//const char* CompilationException::what() const throw()
//{
//	return m_messageMBCS.c_str();
//}

//------------------------------------------------------------------------------
Exception* CompilationException::copy() const
{
	return LN_NEW CompilationException(*this);
}

//------------------------------------------------------------------------------
const Char* CompilationException::getMessage() const
{
	return m_message.c_str();
}

//==============================================================================
// OpenGLException
//==============================================================================

OpenGLException::OpenGLException()
{
	setCaption(InternalResource::getString(InternalResource::OpenGLError).c_str());
}

OpenGLException::OpenGLException(unsigned int gl_enum)
	: OpenGLException()
{
	setMessage(gl_enum);
}

Exception* OpenGLException::copy() const
{
	return LN_NEW OpenGLException(*this);
}

void OpenGLException::setMessage(unsigned int gl_enum)
{
	switch (gl_enum)
	{
	case GL_INVALID_ENUM:		RuntimeException::setMessage(_T("GL_INVALID_ENUM")); break;
	case GL_INVALID_VALUE:		RuntimeException::setMessage(_T("GL_INVALID_VALUE")); break;
	case GL_INVALID_OPERATION:	RuntimeException::setMessage(_T("GL_INVALID_OPERATION")); break;
	case GL_STACK_OVERFLOW:		RuntimeException::setMessage(_T("GL_STACK_OVERFLOW")); break;
	case GL_STACK_UNDERFLOW:	RuntimeException::setMessage(_T("GL_STACK_UNDERFLOW")); break;
	case GL_OUT_OF_MEMORY:		RuntimeException::setMessage(_T("GL_OUT_OF_MEMORY")); break;

		// 0x0506 1286
	case GL_INVALID_FRAMEBUFFER_OPERATION: RuntimeException::setMessage(_T("GL_OUT_OF_MEMORY")); break;
	default:					RuntimeException::setMessage(_T("GLenum %d"), gl_enum); break;
	}
}

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
