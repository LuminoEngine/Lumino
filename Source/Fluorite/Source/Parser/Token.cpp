
#include "Internal.h"
#include "Token.h"
#include "TokenBuffer.h"

LN_NAMESPACE_BEGIN
namespace parser
{
	
//=============================================================================
// Token
//=============================================================================

const Token Token::EofToken(CommonTokenType::Eof/*, nullptr, nullptr*/);

const TokenChar* Token::GetBegin() const { return m_ownerBuffer->GetStringBegin(*this); }
const TokenChar* Token::GetEnd() const { return m_ownerBuffer->GetStringEnd(*this); }

} // namespace Parser
LN_NAMESPACE_END

