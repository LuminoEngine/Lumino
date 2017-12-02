
#include "Internal.h"
#include <Fluorite/AnalyzerContext.h>
#include <Fluorite/Token.h>

namespace fl
{

//==============================================================================
// SourceToken
//==============================================================================

//------------------------------------------------------------------------------
SourceToken::SourceToken()
	: m_ownerFile(nullptr)
	, m_locBegin(0)
	, m_locEnd(0)
	, m_firstLineNumber(0)
	, m_firstColumn(0)
	, m_lastLineNumber(0)
	, m_lastColumn(0)
	, m_group(TokenGroup::Unknown)
	, m_tokenType(0)
{
}

//------------------------------------------------------------------------------
SourceToken::SourceToken(InputFile* ownerFile, TokenGroup group, SourceLocation locBegin, SourceLocation locEnd)
	: SourceToken()
{
	m_ownerFile = ownerFile;
	m_locBegin = locBegin;
	m_locEnd = locEnd;
	m_group = group;
}

//------------------------------------------------------------------------------
SourceToken::SourceToken(InputFile* ownerFile, TokenGroup group, SourceLocation locBegin, SourceLocation locEnd, int tokenType)
	: SourceToken()
{
	m_ownerFile = ownerFile;
	m_locBegin = locBegin;
	m_locEnd = locEnd;
	m_group = group;
	m_tokenType = tokenType;
}

//------------------------------------------------------------------------------
SourceToken::~SourceToken()
{
}

//------------------------------------------------------------------------------
const flChar* SourceToken::getBegin() const
{
	if (LN_REQUIRE(m_ownerFile != nullptr)) return nullptr;
	const flChar* begin = (const flChar*)m_ownerFile->GetCodeBuffer()->getConstData();
	return begin + m_locBegin;
}

//------------------------------------------------------------------------------
const flChar* SourceToken::getEnd() const
{
	return getBegin() + getLength();
}

//------------------------------------------------------------------------------
const flChar* SourceToken::GetCStr(InputFile* file) const
{
	const flChar* begin = (const flChar*)file->GetCodeBuffer()->getConstData();
	return begin + m_locBegin;
}

//------------------------------------------------------------------------------
flString SourceToken::getString(InputFile* file) const
{
	const flChar* begin = (const flChar*)file->GetCodeBuffer()->getConstData();
	return flString(begin + m_locBegin, m_locEnd - m_locBegin);
}

//------------------------------------------------------------------------------
flString SourceToken::getString() const
{
	const flChar* begin = (const flChar*)m_ownerFile->GetCodeBuffer()->getConstData();
	return flString(begin + m_locBegin, m_locEnd - m_locBegin);
}

//------------------------------------------------------------------------------
bool SourceToken::EqualString(const char* str, int len) const
{
	len = (len < 0) ? strlen(str) : len;
	if (getLength() != len) return false;
	return StringTraits::strncmp(getBegin(), str, len) == 0;	// TODO: Case
}

//------------------------------------------------------------------------------
bool SourceToken::EqualChar(char ch) const
{
	if (getLength() != 1) return false;
	return *getBegin() == ch;	// TODO: Case
}

//------------------------------------------------------------------------------
bool SourceToken::EqualGroupAndString(TokenGroup group, const char* str, int len) const
{
	if (m_group != group) return false;
	return EqualString(str, len);
}



//std::string TokenList::toStringValidCode() const
//{
//	std::stringstream ss;
//	for (int i = 0; i < getCount(); ++i)
//	{
//		if (getAt(i).isValid())
//		{
//			ss << std::string(getAt(i).getBegin(), getAt(i).getLength());
//		}
//	}
//	return ss.str();
//}


//==============================================================================
// Token
//==============================================================================
Token::Token(SourceLocation loc)
	: m_loc(loc)
	, m_sourceToken(nullptr)
	, m_valid(true)
{
}

} // namespace fl

