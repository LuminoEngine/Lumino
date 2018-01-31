
#include "../Internal.h"
#include "../../Include/Lumino/Graphics/GraphicsException.h"
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

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
