
#pragma once
#include "Common.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN

class CompilationException
	: public Exception
{
public:
	CompilationException(const ShaderCompileResult& result);
	virtual ~CompilationException() throw();

public:
	// override Exception
	virtual Exception* Copy() const;
	virtual const char* what() const throw();
	virtual const TCHAR* GetMessageOverride() const override;

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

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
