
#pragma once

#include "Common.h"

namespace Lumino
{
namespace Graphics
{

class CompilationException
	: public Exception
{
public:
	CompilationException(const ShaderCompileResult& result);
	virtual ~CompilationException() throw();

public:
	// override Exception
	virtual const TCHAR* GetMessage() const;
	virtual Exception* Copy() const;
	virtual const char* what() const throw();

private:
	String	m_message;		///< コンパイルエラーのメッセージはベースで確保しているメッセージのバッファを余裕で超えることがあるので独自確保
	StringA	m_messageMBCS;	
};

class OpenGLException
	: public Exception
{
public:
	OpenGLException(unsigned int gl_enum);
	virtual ~OpenGLException() throw();
};

} // namespace Graphics
} // namespace Lumino
