
#pragma once

#include "../Internal.h"
#include "../../include/Lumino/Graphics/GraphicsException.h"

namespace Lumino
{
LN_NAMESPACE_GRAPHICS_BEGIN

//=============================================================================
// CompilationException
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
CompilationException::CompilationException(const ShaderCompileResult& result)
{
	m_message.AssignCStr(result.Message);
	m_messageMBCS.AssignCStr(result.Message);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
CompilationException::~CompilationException() throw()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
const TCHAR* CompilationException::GetMessage() const
{
	return m_message.GetCStr();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
const char* CompilationException::what() const throw()
{
	return m_messageMBCS.GetCStr();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Exception* CompilationException::Copy() const
{
	return LN_NEW CompilationException(*this);
}

//=============================================================================
// OpenGLException
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
OpenGLException::OpenGLException(unsigned int gl_enum)
{
	const String& caption = InternalResource::GetString(InternalResource::OpenGLError);
	switch (gl_enum)
	{
	case GL_INVALID_ENUM:		SetMessage(caption, "GL_INVALID_ENUM"); break;
	case GL_INVALID_VALUE:		SetMessage(caption, "GL_INVALID_VALUE"); break;
	case GL_INVALID_OPERATION:	SetMessage(caption, "GL_INVALID_OPERATION"); break;
	case GL_STACK_OVERFLOW:		SetMessage(caption, "GL_STACK_OVERFLOW"); break;
	case GL_STACK_UNDERFLOW:	SetMessage(caption, "GL_STACK_UNDERFLOW"); break;
	case GL_OUT_OF_MEMORY:		SetMessage(caption, "GL_OUT_OF_MEMORY"); break;
	default:					SetMessage(caption, "GLenum %d", gl_enum); break;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
OpenGLException::~OpenGLException() throw()
{

}

LN_NAMESPACE_GRAPHICS_END
} // namespace Lumino
