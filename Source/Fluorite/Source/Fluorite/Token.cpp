
#include "Internal.h"
#include <Fluorite/AnalyzerContext.h>
#include <Fluorite/Token.h>

namespace fl
{

//==============================================================================
// Token
//==============================================================================

//------------------------------------------------------------------------------
Token::Token()
	: m_ownerFile(nullptr)
	, m_locBegin(0)
	, m_locEnd(0)
	, m_firstLineNumber(0)
	, m_firstColumn(0)
	, m_lastLineNumber(0)
	, m_lastColumn(0)
	, m_group(TokenGroup::Unknown)
	, m_tokenType(0)
	, m_valid(false)
{
}

//------------------------------------------------------------------------------
Token::Token(InputFile* ownerFile, TokenGroup group, SourceLocation locBegin, SourceLocation locEnd)
	: Token()
{
	m_ownerFile = ownerFile;
	m_locBegin = locBegin;
	m_locEnd = locEnd;
	m_group = group;
}

//------------------------------------------------------------------------------
Token::Token(InputFile* ownerFile, TokenGroup group, SourceLocation locBegin, SourceLocation locEnd, int tokenType)
	: Token()
{
	m_ownerFile = ownerFile;
	m_locBegin = locBegin;
	m_locEnd = locEnd;
	m_group = group;
	m_tokenType = tokenType;
}

//------------------------------------------------------------------------------
Token::~Token()
{
}

//------------------------------------------------------------------------------
const flChar* Token::getBegin() const
{
	if (LN_REQUIRE(m_ownerFile != nullptr)) return nullptr;
	const flChar* begin = (const flChar*)m_ownerFile->GetCodeBuffer()->getConstData();
	return begin + m_locBegin;
}

//------------------------------------------------------------------------------
const flChar* Token::getEnd() const
{
	return getBegin() + getLength();
}

//------------------------------------------------------------------------------
const flChar* Token::GetCStr(InputFile* file) const
{
	const flChar* begin = (const flChar*)file->GetCodeBuffer()->getConstData();
	return begin + m_locBegin;
}

//------------------------------------------------------------------------------
flString Token::getString(InputFile* file) const
{
	const flChar* begin = (const flChar*)file->GetCodeBuffer()->getConstData();
	return flString(begin + m_locBegin, m_locEnd - m_locBegin);
}

//------------------------------------------------------------------------------
flString Token::getString() const
{
	const flChar* begin = (const flChar*)m_ownerFile->GetCodeBuffer()->getConstData();
	return flString(begin + m_locBegin, m_locEnd - m_locBegin);
}

//------------------------------------------------------------------------------
bool Token::EqualString(const char* str, int len) const
{
	len = (len < 0) ? strlen(str) : len;
	if (getLength() != len) return false;
	return StringTraits::strncmp(getBegin(), str, len) == 0;	// TODO: Case
}

//------------------------------------------------------------------------------
bool Token::EqualChar(char ch) const
{
	if (getLength() != 1) return false;
	return *getBegin() == ch;	// TODO: Case
}

//------------------------------------------------------------------------------
bool Token::EqualGroupAndString(TokenGroup group, const char* str, int len) const
{
	if (m_group != group) return false;
	return EqualString(str, len);
}

} // namespace fl

