
#include "Internal.h"
#include "UnitFile.h"

LN_NAMESPACE_BEGIN
namespace parser
{

//=============================================================================
// UnitFile
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UnitFile::Initialize(const TokenPathName& fileAbsPath)
{
	assert(fileAbsPath.IsAbsolute());
	m_fileAbsPath = fileAbsPath;
	//m_macroMap = RefPtr<MacroMap>::MakeRef();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
//void UnitFile::CopyMacroMap(const MacroMap* macroMap)
//{
//	m_macroMap->Copy(macroMap);
//}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SourceRange UnitFile::SaveMacroReplacementTokens(const Token* begin, const Token* end)
{
	SourceRange range;
	range.begin.loc = m_tokensCache.GetCount();
	for (const Token* pos = begin; pos < end; ++pos)
	{
		m_tokensCache.Add(*pos);
	}
	range.end.loc = m_tokensCache.GetCount();
	m_tokensCache.Add(Token::EofToken);	// Eof を入れておくことでオーバーランや m_tokensCache[range.end.loc] へのアクセスに備える
	return range;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UnitFile::GetMacroReplacementTokens(const SourceRange& range, const Token** outBegin, const Token** outEnd) const
{
	assert(outBegin != nullptr);
	assert(outEnd != nullptr);
	*outBegin = &m_tokensCache[range.begin.loc];
	*outEnd = &m_tokensCache[range.end.loc];
}



} // namespace Parser
LN_NAMESPACE_END

