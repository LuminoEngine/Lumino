
#include "../Internal.h"
#include "../../include/Lumino/Graphics/GraphicsException.h"
#include "Device/OpenGL/GLCommon.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN

//==============================================================================
// CompilationException
//==============================================================================

//------------------------------------------------------------------------------
CompilationException::CompilationException(const ShaderCompileResult& result)
{
	m_message.assignCStr(result.Message.c_str());
	m_messageMBCS = result.Message;
}

//------------------------------------------------------------------------------
CompilationException::~CompilationException() throw()
{
}

//------------------------------------------------------------------------------
const char* CompilationException::what() const throw()
{
	return m_messageMBCS.c_str();
}

//------------------------------------------------------------------------------
Exception* CompilationException::copy() const
{
	return LN_NEW CompilationException(*this);
}

//------------------------------------------------------------------------------
const Char* CompilationException::getMessageOverride() const
{
	return m_message.c_str();
}

//==============================================================================
// OpenGLException
//==============================================================================

//------------------------------------------------------------------------------
OpenGLException::OpenGLException(unsigned int gl_enum)
{
	const String& caption = InternalResource::getString(InternalResource::OpenGLError);
	switch (gl_enum)
	{
	case GL_INVALID_ENUM:		setMessage(caption.c_str(), _T("GL_INVALID_ENUM")); break;
	case GL_INVALID_VALUE:		setMessage(caption.c_str(), _T("GL_INVALID_VALUE")); break;
	case GL_INVALID_OPERATION:	setMessage(caption.c_str(), _T("GL_INVALID_OPERATION")); break;
	case GL_STACK_OVERFLOW:		setMessage(caption.c_str(), _T("GL_STACK_OVERFLOW")); break;
	case GL_STACK_UNDERFLOW:	setMessage(caption.c_str(), _T("GL_STACK_UNDERFLOW")); break;
	case GL_OUT_OF_MEMORY:		setMessage(caption.c_str(), _T("GL_OUT_OF_MEMORY")); break;
            
        // 0x0506 1286
        case GL_INVALID_FRAMEBUFFER_OPERATION:setMessage(caption.c_str(), _T("GL_OUT_OF_MEMORY")); break;
	default:					setMessage(caption.c_str(), _T("GLenum %d"), gl_enum); break;
	}
}

//------------------------------------------------------------------------------
OpenGLException::~OpenGLException() throw()
{

}

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
