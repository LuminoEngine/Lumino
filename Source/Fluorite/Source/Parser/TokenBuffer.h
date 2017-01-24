
#pragma once
#include "Token.h"
#include "CacheBuffer.h"

LN_NAMESPACE_BEGIN
namespace parser
{

/**
	
*/
class TokenBuffer	// TODO: ÇøÇÂÇ¡Ç∆ñºëOÇ™Ç‹Ç¨ÇÁÇÌÇµÇ¢ÅBTokenStringBuffer Ç∆Ç©ÇÃÇŸÇ§Ç™Ç¢Ç¢ÅH
	: public RefObject
{
public:
	TokenBuffer();
	virtual ~TokenBuffer();

	Token CreateToken(CommonTokenType commonType, const TokenChar* begin, const TokenChar* end, int langTokenType = 0);

	//void GetString(const Token& token, const TokenChar** outBegin, const TokenChar** outEnd);

	const TokenChar* GetStringBegin(const Token& token) const { return m_stringCache.GetData(token.m_locBegin); }
	const TokenChar* GetStringEnd(const Token& token) const { return m_stringCache.GetData(token.m_locEnd); }

private:
	CacheBuffer<TokenChar>	m_stringCache;
};

class ConstantTokenBuffer
{
public:
	ConstantTokenBuffer();

	const Token& Get0() const { return m_0; }
	const Token& Get1() const { return m_1; }

private:
	TokenBuffer	m_buffer;
	Token		m_0;
	Token		m_1;
};

} // namespace Parser
LN_NAMESPACE_END


