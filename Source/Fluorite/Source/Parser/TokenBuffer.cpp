
#include "Internal.h"
#include "Frontend/Cpp/CppLexer.h"
#include "TokenBuffer.h"

LN_NAMESPACE_BEGIN
namespace parser
{

//=============================================================================
// TokenBuffer
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
TokenBuffer::TokenBuffer()
{
	m_stringCache.Reserve(8192);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
TokenBuffer::~TokenBuffer()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Token TokenBuffer::CreateToken(CommonTokenType commonType, const TokenChar* begin, const TokenChar* end, int langTokenType)
{
	Token t;
	t.m_ownerBuffer = this;
	t.m_commonType = commonType;
	t.m_langTokenType = langTokenType;
	if (begin != nullptr)
	{
		int len = end - begin;
		t.m_locBegin = m_stringCache.Append(begin, len);
		t.m_locEnd = t.m_locBegin + len;
	}
	else
	{
		t.m_locBegin = 0;
		t.m_locEnd = 0;
	}
	return t;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
//void TokenBuffer::GetString(const Token& token, const TokenChar** outBegin, const TokenChar** outEnd)
//{
//	*outBegin = m_stringCache.GetData(token.m_);
//}

//=============================================================================
// ConstantTokenBuffer
//=============================================================================

static const TokenChar* ConstToken_0_Buf = "0";
static const TokenChar* ConstToken_1_Buf = "1";

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ConstantTokenBuffer::ConstantTokenBuffer()
{
	m_0 = m_buffer.CreateToken(CommonTokenType::ArithmeticLiteral, ConstToken_0_Buf, ConstToken_0_Buf + 1, TT_NumericLitaralType_Int32);
	m_1 = m_buffer.CreateToken(CommonTokenType::ArithmeticLiteral, ConstToken_1_Buf, ConstToken_1_Buf + 1, TT_NumericLitaralType_Int32);
}

} // namespace Parser
LN_NAMESPACE_END

