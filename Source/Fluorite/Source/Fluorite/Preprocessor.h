
#pragma once

namespace fl {
class InputFile;

class SymbolTable
{
public:

private:

};
	
/**
	@brief	ファイル1つ分のプリプロセスを行う。
	
	.cpp や .h ひとつ分。ヘッダファイルも1つ分。
*/
class Preprocessor
{
public:
	Preprocessor();

	//void Preprocess(
	//	InputFile* compileUnitFile,
	//	UnitFile* unitFile,
	//	const MacroMapContainer& parentMacroMap);
};

} // namespace fl

#if 0
#include <unordered_map>
#include "../../Common.h"
#include "../../TokenList.h"
#include "../../TokenBuffer.h"
#include "../../IdentifierMap.h"
#include "../RpnParser.h"

LN_NAMESPACE_BEGIN
namespace parser
{

class SourceLocation
{
public:
	uint32_t	loc;
};

class SourceRange
{
public:
	SourceLocation	begin;
	SourceLocation	end;
};

// マクロ定義の実体。同名マクロはありえる(undef 後の再定義しても前のは保持する)。その場合は通常、定義行が違う。
// 
class MacroDefine
	: public RefObject
{
public:
	TokenString		name;
	SourceRange		replacementRange;
	//TokenString		replacementContentString;	// TODO: いらないかも。置換要素を文字列で並べたもの (完全一致の確認で使う。前後の空白は消しておく)

	bool		undef = false;

	//void AppendReplacementToTokenList(TokenList* tokenList);	// TODO: 引数リスト

	MacroDefine(){}

private:
	LN_DISALLOW_COPY_AND_ASSIGN(MacroDefine);
};

class MacroMap
	: public RefObject
{
public:
	MacroMap();

	MacroDefine* Insert(const Token& name, const SourceRange& replacementRange);

	MacroDefine* Find(const TokenChar* nameBegin, const TokenChar* nameEnd = nullptr) const;
	MacroDefine* Find(const Token& name) const;

	bool IsDefined(const Token& name, MacroDefine** outDefinedMacro = nullptr) const;
	bool IsDefined(const TokenChar* name, MacroDefine** outDefinedMacro = nullptr) const;

	uint64_t getHashCode() const;

	void Copy(const MacroMap* srcMacroMap);

	//void SetFreeze(bool enabled) { m_freeze = enabled; }
	//bool IsFreeze() const { return m_freeze; }

private:
	Array<Ref<MacroDefine>>			m_allMacroList;	// 過去に定義された全てのマクロ
	IdentifierMap<MacroDefine*>	m_macroMap;		// 再定義されたりしたものは一番新しいマクロが格納される
	//bool						m_freeze;		// 変更禁止フラグ
};

// COW 共有の支援
//		# が1つも無い include ファイルのために MacroMap を作ったりするとメモリ効率が悪くなる。
//		include ファイルの解析開始で MAcroMap をコピーするか終了時にするかとか考えると複雑になったりする。
//		本当に必要なタイミングでコピーを作るためにこのクラスを用意した。
class MacroMapContainer
{
public:

	MacroMapContainer()
	{
		m_core = &m_sharedEmpty;
	}

	MacroMap* get()
	{
		if (m_core.get() == &m_sharedEmpty || m_core->GetReferenceCount() != 1)
		{
			auto newCore = Ref<MacroMap>::makeRef();
			if (!m_core.IsNull())
			{
				newCore->Copy(m_core);
				m_core = newCore;
			}
		}
		return m_core;
	}


	const MacroMap* GetConst() const
	{
		return m_core;
	}



private:
	Ref<MacroMap>	m_core;
	static MacroMap		m_sharedEmpty;
};


class RawReferenceMap
{

};




// プリプロセスしたファイル情報。
// .c か .h かは問わない。
// ×トークンリストは保持しないので注意。
/*
	トークンリストも保持する。UIColors.h とか。


	同名ファイルでも、入力マクロマップの内容が違えば違うインスタンスを作る。
*/
class PreprocessedFileCacheItem
{
public:
	std::string		filePath;
	MacroMap		inputMacroMap;

	MacroMap		outputMacroMap;
	RawReferenceMap	outputRawReferenceMap;

public:
	SourceRange SaveMacroTokens(const Token* begin, const Token* end);	// キャッシュに保存すると再配置の可能性があるので、格納された場所はポインタではなくインデックスで返す
	void GetMacroTokens(const SourceRange& range, const Token** outBegin, const Token** outEnd) const;

private:
	Array<Token>	m_tokensCache;
};

class HeaderFileManager
{
public:

};


/**
	@brief	CompileUnit 1つ分のプリプロセスの状態管理を行う。
*/
class PreprocessContext
{

};

	
/**
	@brief	ファイル1つ分のプリプロセスを行う。
	
	.cpp や .h ひとつ分。ヘッダファイルも1つ分。
*/
class Preprocessor
{
public:
	Preprocessor();

	// 指定する MacroMap は開始時点のマクロ定義状態
	ResultState BuildPreprocessedTokenList(
		Context* ownerContext,
		CompileUnitFile* compileUnitFile,
		TokenList* tokenList,
		UnitFile* unitFile,
		const Array<TokenPathName>* additionalIncludePaths,
		const MacroMapContainer& parentMacroMap,
		ReferenceTracker* referenceTracker,
		DiagnosticsItemSet* diag);

private:

	ResultState PollingDirectiveLine(Token* keyword, Token* lineEnd);
	ResultState AnalyzeIfElifDirective(Token* keyword, Token* lineEnd, bool isElse);
	ResultState AnalyzeIncludeDirective(Token* keyword, Token* lineEnd);
	ResultState AnalyzePragmaDirective(Token* keyword, Token* lineEnd);

	bool IsValidSection() const;

	//TokenList::iterator GetNextGenericToken(TokenList::iterator pos);





	enum class DirectiveSec
	{
		Idle,			// 何もしていない
		LineHead,		// 行頭である。または初期状態
		FindIdent,		// 識別子を探している
		FindLineEnd,	// "#" を見つけた
	};

	// #if ～ #endif までの判定状態
	enum class ConditionalSectionState
	{
		None,			// 判定前、セクション外
		Valid,			// 有効判定グループ内
		Invalid,		// 無効判定グループ内
		Skip,			// 有効無効判定終了後、#endifまでskip可能
	};

	// #if ～ #endif までの情報
	struct ConditionalSection
	{
		ConditionalSectionState	state = ConditionalSectionState::None;	// #if～#endif までの現在の判定状態
		bool					elseProcessed = false;					// #else受付後はtrue(#else～#else防止の為)
	};

	Context*					m_ownerContext;
	ConstantTokenBuffer			m_constTokenBuffer;
	TokenList*					m_tokenList;
	//PreprocessedFileCacheItem*	m_fileCache;
	CompileUnitFile*			m_compileUnitFile;
	UnitFile*					m_unitFile;
	MacroMapContainer			m_macroMap;
	const Array<TokenPathName>* m_additionalIncludePaths;
	ReferenceTracker*			m_referenceTracker;
	DiagnosticsItemSet*			m_diag;

	DirectiveSec				m_seqDirective;
	Stack<ConditionalSection>	m_conditionalSectionStack;	// #if ～ #endif までの情報のスタック
	Token*						m_preproLineHead;			// # の次のトークンを指している

	TokenList					m_preproExprTokenList;		// 前処理定数式のトークンを展開する作業領域
	RpnParser					m_rpnParser;
	RpnEvaluator				m_rpnEvaluator;

	Array<Token*>				m_funcMacroParams;

	bool						m_foundPragmaOnce;
};

} // namespace Parser
LN_NAMESPACE_END

#endif

