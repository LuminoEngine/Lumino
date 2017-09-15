
#pragma once
#include "Common.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN

class CompilationException
	: public RuntimeException
{
public:
	CompilationException();
	CompilationException(const ShaderCompileResult& result);
	virtual ~CompilationException() throw();

public:
	// override Exception
	virtual Exception* copy() const;
	//virtual const char* what() const throw();
	virtual const Char* getMessage() const override;

private:
	void setMessage(const ShaderCompileResult& result);
	String		m_message;		///< コンパイルエラーのメッセージはベースで確保しているメッセージのバッファを余裕で超えることがあるので独自確保
	//std::string	m_messageMBCS;	
};

class OpenGLException
	: public RuntimeException
{
public:
	OpenGLException();
	OpenGLException(unsigned int gl_enum);
	virtual Exception* copy() const;

private:
	void setMessage(unsigned int gl_enum);
};

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
