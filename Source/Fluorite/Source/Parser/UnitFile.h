
#pragma once
#include "Common.h"
#include "TokenList.h"
#include "Frontend\Cpp\Preprocessor.h"

LN_NAMESPACE_BEGIN
namespace parser
{

// .cpp や .h、1つの解析単位となるファイル。パス名と LocationStateKey によって辞書管理。
// TODO: CodeFile とかの名前のほうがいいかな？
class UnitFile
	: public RefObject
{
public:

	void Initialize(const TokenPathName& fileAbsPath);

	const TokenPathName& GetAbsolutePath() const { return m_fileAbsPath; }
	const TokenPathName& GetDirectoryPath() const { return m_directoryAbsPath; }

	void SetTokenList(TokenListPtr tokenList) { m_tokenList = tokenList; }
	TokenListPtr GetTokenList() const { m_tokenList; }

	//void CopyMacroMap(const MacroMap* macroMap);

	void SetMacroMap(const MacroMapContainer& macroMap) { m_macroMap = macroMap; }
	MacroMapContainer* GetMacroMapPtr() { return &m_macroMap; }

public:
	SourceRange SaveMacroReplacementTokens(const Token* begin, const Token* end);	// キャッシュに保存すると再配置の可能性があるので、格納された場所はポインタではなくインデックスで返す
	void GetMacroReplacementTokens(const SourceRange& range, const Token** outBegin, const Token** outEnd) const;


LN_INTERNAL_ACCESS:
	TokenPathName		m_fileAbsPath;
	TokenPathName		m_directoryAbsPath;	// m_fileAbsPath の parent。incldue ファイルの検索とかで頻繁に使うので、メモリ効率を考慮してあらかじめ取り出しておく
	TokenListPtr		m_tokenList;

	//RefPtr<MacroMap>	m_macroMap;				// このファイルの中で定義されたマクロ。undef は undef として記録する
	MacroMapContainer	m_macroMap;
	uint64_t			m_inputMacroMapHash;
	Array<Token>		m_tokensCache;	// TODO: TokenList でもいい？
	TokenBuffer			m_tokenBuffer;

	// マクロの置換要素はずっと保存しておかなければならない。置き場所はこのクラスが一番イメージしやすいと思う。
	// 一方、元のトークンリストは保存する必要は無い。どんな定義があるのかだけ分かればいい。(関数名は必要だけど () なんかはいらない)
	// [2016/2/19] トークンリストも持っておかなければならない。
	// UIColors.h とか、ハードコーディングで埋め込んだリソースとか。
	// 全部メモリに持っておくのもツライので、一時ファイルに保存することも検討する必要があるかも。
};


// 
class IncludeFile
	: public UnitFile
{
public:
};

// コンパイル単位ファイル。解析への入力。 (.cppなど。includeファイルではない。FileEntity は出力と考えるべき、かも)
class CompileUnitFile
	: public UnitFile
{
public:
	Array<IncludeFile*>	includedFiles;
};


} // namespace Parser
LN_NAMESPACE_END

