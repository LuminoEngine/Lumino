
#pragma once

#include "../Internal.h"
#include "../../include/Lumino/Graphics/GraphicsException.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN

//=============================================================================
// CompilationException
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
CompilationException::CompilationException(const ShaderCompileResult& result)
{
	m_message.AssignCStr(result.Message.c_str());
	m_messageMBCS.AssignCStr(result.Message.c_str());
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
	return m_message.c_str();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
const char* CompilationException::what() const throw()
{
	return m_messageMBCS.c_str();
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
	case GL_INVALID_ENUM:		SetMessage(caption.c_str(), _T("GL_INVALID_ENUM")); break;
	case GL_INVALID_VALUE:		SetMessage(caption.c_str(), _T("GL_INVALID_VALUE")); break;
	case GL_INVALID_OPERATION:	SetMessage(caption.c_str(), _T("GL_INVALID_OPERATION")); break;
	case GL_STACK_OVERFLOW:		SetMessage(caption.c_str(), _T("GL_STACK_OVERFLOW")); break;
	case GL_STACK_UNDERFLOW:	SetMessage(caption.c_str(), _T("GL_STACK_UNDERFLOW")); break;
	case GL_OUT_OF_MEMORY:		SetMessage(caption.c_str(), _T("GL_OUT_OF_MEMORY")); break;
	default:					SetMessage(caption.c_str(), _T("GLenum %d"), gl_enum); break;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
OpenGLException::~OpenGLException() throw()
{

}

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
