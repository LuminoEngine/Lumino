/*
	#if
	定数式
	defined

	#include
		検索順序(microsoft specific)
			include "xxxx"の場合		1	対象ファイルと同じディレクトリを探す	In the same directory as the file that contains the #include statement.
									2	入れ子入れ子でインクルードされている場合、インクルード元のファイルの存在するディレクトリを探す。入れ子の一番親のファイルまでさかのぼる。	In the directories of any previously opened include files in the reverse order in which they were opened. The search starts from the directory of the include file that was opened last and continues through the directory of the include file that was opened first.
									3	-Iコンパイルオプションで指定したインクルードパスのディレクトリを探す。	Along the path specified by each /I compiler option.
									4	環境変数で指定したディレクトリを探す。	Along the paths specified by the INCLUDE environment variable.
			include <xxxx>の場合		1	-Iコンパイルオプションで指定したインクルードパスのディレクトリを探す。	Along the path specified by each /I compiler option.
									2	環境変数で指定したディレクトリを探す。	When compiling from the command line, along the paths that are specified by the INCLUDE environment variable


	規格で定義されたマクロ
		__DATE__					変動
		__FILE__					変動
		__LINE__					変動
		__TIME__					変動
		__STDC__					環境依存(.c)
		__STDC_HOSTED__				環境依存(.c)
		__STDC_VERSION__			環境依存(.c)
		__STDC_IEC_559__			環境依存(.c)
		__STDC_IEC_559_COMPLEX__	環境依存(.c)
		__STDC_ISO_10646__			環境依存(.c)
		__cplusplus					環境依存(.cpp)

*/



/*
	preprocessing-file:
		groupopt
	group:
		group-part
		group group-part
	group-part:
		if-section
		control-line
		text-line
		# non-directive
	if-section:
		if-group elif-groupsopt else-groupopt endif-line
	if-group:
		# if constant-expression new-line groupopt
		# ifdef identifier new-line groupopt
		# ifndef identifier new-line groupopt
	elif-groups:
		elif-group
		elif-groups elif-group
	elif-group:
		# elif constant-expression new-line groupopt
	else-group:
		# else new-line groupopt
	endif-line:
		# endif new-line
	control-line:
		# include pp-tokens new-line
		# define identifier replacement-list new-line
		# define identifier lparen identifier-listopt) replacement-list new-line
		# define identifier lparen ... ) replacement-list new-line
		# define identifier lparen identifier-list, ... ) replacement-list new-line
		# undef identifier new-line
		# line pp-tokens new-line
		# error pp-tokensopt new-line
		# pragma pp-tokensopt new-line
		# new-line
	text-line:
		pp-tokensopt new-line

	non-directive:
		pp-tokens new-line
	lparen:
		a ( character not immediately preceded by white-space
	identifier-list:
		identifier
		identifier-list , identifier
	replacement-list:
		pp-tokensopt
	pp-tokens:
		preprocessing-token
		pp-tokens preprocessing-token
	new-line:
		the new-line character

	......
	sizeof
	alignof
	defined ( 識別子 )

	__STDC__


	-----------------------------------
	JISX3014 プログラム言語C++

	16.1
	# ～キーワード前の間は 空白と水平タブのみ可、だけどとりあえず「空白類」としている。

	16.1注(136)
	制御定数式内の全ての識別子はマクロ名であるか否かのいずれかとなる。
	すなわち、キーワード、列挙定数などはまだ存在しない。
	↓
	キャスト式不可能。


	■ "#if AAA" で AAA が未定義のとき
		> 16.1
		> マクロ展開及び defined 単項演算子による全ての置換の実行後、残っている全ての識別子及び
		> true と false　を除くキーワードを前処理数 0 で置き換えてから、各前処理字句を字句に変換する。
		↓
		未定義マクロ (undef されたものも含む) は #if の条件式で使用でき、値は必ず 0 となる。 

	■ マクロ再定義
		#define CCC 1+1			// オリジナル
		#define CCC 1+1\s		// OK。前後の空白は許可
		#define CCC 1 + 1		// NG。(VisualC++では警告)
		#define CCC 1/＊＊/+1	// NG。(VisualC++では警告)

	■ 置換要素の中に defined があるとき
		#define HHH 1
		#define GGG defined(HHH)
		#if GGG

		未定義動作。VisualStudio では defined として解釈されない。
		ただ、↑の例は結果 0 でエラーにはならない。しかし、defined を aaa のようなほかの識別子とすると警告が発生する。

		本ライブラリとしては通常の識別子扱いし、defined=0で解釈する。
		その結果 0(1) という展開結果になるため式の解析でエラーが出ることになる。

	■ #pragma once について
		VisualC++ のは本当に単純に、同パスのファイルを incluide しないようにするだけ。
		マクロ定義が変わるとかは関係ない。
		次の結果は、"1"が2つ出力される。
		```````````````````````````````.h
		#pragma once
		#ifdef AAA
		#define BBB 2
		#else
		#define BBB 1
		#endif
		```````````````````````````````
		```````````````````````````````.cpp
		#include "Header.h"
			printf("%d\n", BBB);	// 1
		#define AAA
		#include "Header.h"
			printf("%d\n", BBB);	// 1
		```````````````````````````````
*/
#if 0
#include "../../Internal.h"
#include "../../DiagnosticsManager.h"
#include "../../ParserUtils.h"
#include "../../UnitFile.h"
#include "../../Context.h"
#include "../../EntityDatabase.h"
#include "CppLexer.h"
#include "Preprocessor.h"

LN_NAMESPACE_BEGIN
namespace parser
{

//=============================================================================
// MacroMap
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
//void MacroEntity::AppendReplacementToTokenList(TokenList* tokenList)
//{
//	for (const Token* pos = replacementBegin; pos < replacementEnd; ++pos)
//	{
//		tokenList->Add(*pos);
//	}
//}

//=============================================================================
// MacroMap
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MacroMap::MacroMap()
	//: m_freeze(false)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MacroDefine* MacroMap::Insert(const Token& name, const SourceRange& replacementRange)
{
	////LN_THROW(!IsFreeze(), InvalidOperationException);

	auto macro = Ref<MacroDefine>::makeRef();
	macro->name = name.ToString();
	macro->replacementRange = replacementRange;
	//macro.replacementContentString = TokenString(replacementBegin->GetBegin(), replacementEnd->GetEnd() - replacementBegin->GetBegin());

	m_allMacroList.Add(macro);
	//MacroDefine* m = m_allMacroList.GetLast();
	m_macroMap.Insert(name.GetBegin(), name.GetEnd(), macro);
	return macro;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MacroDefine* MacroMap::Find(const TokenChar* nameBegin, const TokenChar* nameEnd) const
{
	if (nameEnd == nullptr) {
		nameEnd = nameBegin + StringTraits::tcslen(nameBegin);
	}

	MacroDefine* e;
	if (m_macroMap.Find(nameBegin, nameEnd, &e, CaseSensitivity::CaseSensitive))	// TODO: 大文字小文字
		return e;
	else
		return nullptr;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MacroDefine* MacroMap::Find(const Token& name) const
{
	return Find(name.GetBegin(), name.GetEnd());
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool MacroMap::IsDefined(const Token& name, MacroDefine** outDefinedMacro) const
{
	MacroDefine* e = Find(name);
	if (e != nullptr)
	{
		if (outDefinedMacro) { *outDefinedMacro = e; }
		return !e->undef;
	}
	return false;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool MacroMap::IsDefined(const TokenChar* name, MacroDefine** outDefinedMacro) const
{
	MacroDefine* e = Find(name);
	if (e != nullptr)
	{
		if (outDefinedMacro) { *outDefinedMacro = e; }
		return !e->undef;
	}
	return false;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
uint64_t MacroMap::getHashCode() const
{
	uint64_t value = 0;
	for (auto& macro : m_allMacroList)
	{
		value += Hash::calcHash(macro->name.c_str(), macro->name.getLength());
	}
	return value + m_allMacroList.GetCount();	// ついでに個数でもいれておこうか
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void MacroMap::Copy(const MacroMap* srcMacroMap)
{
	//LN_THROW(!IsFreeze(), InvalidOperationException);

	m_allMacroList = srcMacroMap->m_allMacroList;
	m_macroMap.Copy(srcMacroMap->m_macroMap);
}

//=============================================================================
// MacroMapContainer
//=============================================================================
MacroMap	MacroMapContainer::m_sharedEmpty;

//=============================================================================
// Preprocessor
//=============================================================================

////-----------------------------------------------------------------------------
////
////-----------------------------------------------------------------------------
//SourceRange PreprocessedFileCacheItem::SaveMacroTokens(const Token* begin, const Token* end)
//{
//	SourceRange range;
//	range.begin.loc = m_tokensCache.GetCount();
//	for (const Token* pos = begin; pos < end; ++pos)
//	{
//		m_tokensCache.Add(*pos);
//	}
//	range.end.loc = m_tokensCache.GetCount();
//	m_tokensCache.Add(Token::EofToken);	// Eof を入れておくことでオーバーランや m_tokensCache[range.end.loc] へのアクセスに備える
//	return range;
//}
//
////-----------------------------------------------------------------------------
////
////-----------------------------------------------------------------------------
//void PreprocessedFileCacheItem::GetMacroTokens(const SourceRange& range, const Token** outBegin, const Token** outEnd) const
//{
//	assert(outBegin != nullptr);
//	assert(outEnd != nullptr);
//	*outBegin = &m_tokensCache[range.begin.loc];
//	*outEnd = &m_tokensCache[range.end.loc];
//}
//
//=============================================================================
// Preprocessor
//-----------------------------------------------------------------------------
/*
	このクラスと CompileUnitFile は同じクラスとしてまとめないの？
	→	Preprocessor は処理担当。一時データをたくさんメンバ変数に保持する。
		CompileUnitFile は解析データの保持担当。
*/
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Preprocessor::Preprocessor()
	: m_tokenList(nullptr)
	, m_unitFile(nullptr)
	, m_seqDirective(DirectiveSec::Idle)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ResultState Preprocessor::BuildPreprocessedTokenList(
	Context* ownerContext,
	CompileUnitFile* compileUnitFile,
	TokenList* tokenList,
	UnitFile* unitFile,
	const Array<TokenPathName>* additionalIncludePaths,
	const MacroMapContainer& parentMacroMap,
	ReferenceTracker* referenceTracker,
	DiagnosticsItemSet* diag)
{
	m_ownerContext = ownerContext;
	m_tokenList = tokenList;
	m_compileUnitFile = compileUnitFile;
	m_unitFile = unitFile;
	m_additionalIncludePaths = additionalIncludePaths;
	m_referenceTracker = referenceTracker;
	m_diag = diag;
	m_seqDirective = DirectiveSec::LineHead;
	m_conditionalSectionStack.Clear();
	m_foundPragmaOnce = false;

	// このファイルの解析開始時点のマクロ定義を設定。
	// CodeFile は固有の MacroMap を持たなければキャッシュする意味がないので、参照ではなくコピーする必要がある。
	//m_unitFile->CopyMacroMap(macroMap);
	m_macroMap = parentMacroMap;

	int tokenCount = m_tokenList->GetCount();
	for (int iToken = 0; iToken < tokenCount; ++iToken)
	{
		Token& token = m_tokenList->GetAt(iToken);

		// プリプロディレクティブ処理中でなければ、トークンの valid は現在の状態に従い、処理中は全て無効扱いにする
		if (m_seqDirective == DirectiveSec::Idle ||
			m_seqDirective == DirectiveSec::LineHead)
		{
			token.SetValid(IsValidSection());
		}
		else
		{
			token.SetValid(false);
		}

		// 何もしていない。改行を探す。
		if (m_seqDirective == DirectiveSec::Idle)
		{
			if (token.GetCommonType() == CommonTokenType::NewLine)
			{
				m_seqDirective = DirectiveSec::LineHead;		// 改行が見つかった。行頭状態へ
			}
		}
		// 行頭にいる。# を探す。
		else if (m_seqDirective == DirectiveSec::LineHead)
		{
			if (token.GetCommonType() == CommonTokenType::Operator &&
				token.EqualString("#", 1))
			{
				m_seqDirective = DirectiveSec::FindIdent;	// "#" を見つけた。識別子を探す
				token.SetValid(false);						// ディレクティブの解析が始まるタイミングなので、ここからトークンを無効にし始める
			}
			else {
				m_seqDirective = DirectiveSec::Idle;		// "#" 以外のトークンだった。Idle へ。
			}
		}
		// 識別子を探している。
		else if (m_seqDirective == DirectiveSec::FindIdent)
		{
			if (token.IsSpaceOrComment())
			{
				// 続行
			}
			else if (
				token.GetCommonType() == CommonTokenType::Identifier ||
				token.GetCommonType() == CommonTokenType::Keyword)		// #else はキーワードで飛んでくる
			{
				// 見つけた
				m_preproLineHead = &token;
				m_seqDirective = DirectiveSec::FindLineEnd;	// 行末を探しに行く
			}
			else if (
				token.GetCommonType() == CommonTokenType::NewLine ||
				token.GetCommonType() == CommonTokenType::Eof)
			{
				// # しかない行だった。シーケンスを戻す
				m_seqDirective = DirectiveSec::LineHead;
			}
			else
			{
				// Error: # の次になんか変なトークンが来た
				m_diag->Report(DiagnosticsCode::Preprocessor_UnexpectedDirectiveToken, token.ToString());
				return ResultState::Error;
			}
		}
		// 行末を探している。
		else if (m_seqDirective == DirectiveSec::FindLineEnd)
		{
			if (token.GetCommonType() == CommonTokenType::NewLine ||
				token.GetCommonType() == CommonTokenType::Eof)
			{
				LN_RESULT_CALL(PollingDirectiveLine(m_preproLineHead, &token));

				if (token.GetCommonType() == CommonTokenType::NewLine) {
					m_seqDirective = DirectiveSec::LineHead;	// 改行なので行頭状態へ
				}
				else {
					m_seqDirective = DirectiveSec::Idle;
				}
			}
		}
	}

	// Error: #endif が無かった
	if (!m_conditionalSectionStack.IsEmpty())
	{
		m_diag->Report(DiagnosticsCode::Preprocessor_NoExistsEndif);
		return ResultState::Error;
	}

	m_unitFile->SetMacroMap(m_macroMap);

	return ResultState::Success;
}

//-----------------------------------------------------------------------------
// lineBegin は 識別子を指している。#include なら include。
// lineEnd は NewLine か Eof を指している。間に 行末\がある場合は飛ばされている。
//-----------------------------------------------------------------------------
ResultState Preprocessor::PollingDirectiveLine(Token* keyword, Token* lineEnd)
{
	//---------------------------------------------------------
	// #define
	//		:: # define identifier replacement-list new-line
	//		:: # define identifier lparen identifier-listopt) replacement-list new-line
	//		:: # define identifier lparen ... ) replacement-list new-line
	//		:: # define identifier lparen identifier-list, ... ) replacement-list new-line
	if (keyword->EqualString("define", 6))
	{
		// 無効領域内なら何もしない
		if (!IsValidSection()) {
			return ResultState::Success;
		}

		// 識別子(マクロ名)まで進める
		Token* macroName = ParserUtils::SkipNextSpaceOrComment(keyword, lineEnd);
		if (macroName->GetCommonType() != CommonTokenType::Identifier)
		{
			// Error: 識別子ではなかった
			m_diag->Report(DiagnosticsCode::Preprocessor_SyntaxError);
			return ResultState::Error;
		}

		// スペースを飛ばす
		Token* parenOrReplacement = ParserUtils::SkipNextSpaceOrComment(keyword, lineEnd);

		// end はスペースがではなくなるまで戻す
		lineEnd = ParserUtils::SkipPrevSpaceOrComment(keyword, lineEnd);

		Token* replacementBegin = nullptr;
		if (parenOrReplacement->GetCommonType() == CommonTokenType::Operator && parenOrReplacement->EqualChar('('))
		{
			// 関数形式だった。識別子を仮引数として取り出す。TODO: , とか見てないけど・・・
			m_funcMacroParams.Clear();
			Token* pos = parenOrReplacement + 1;
			for (; pos < lineEnd; ++pos)
			{
				if (pos->GetCommonType() == CommonTokenType::Operator && pos->EqualChar(')'))
				{
					replacementBegin = ParserUtils::SkipNextSpaceOrComment(pos, lineEnd);
					break;
				}
				else if (pos->GetCommonType() == CommonTokenType::Identifier)
				{
					m_funcMacroParams.Add(pos);
				}
				else if (pos->GetCommonType() == CommonTokenType::Operator && pos->GetLangTokenType() == TT_CppOP_Ellipsis)
				{
					m_funcMacroParams.Add(pos);	// "..."
				}
			}

			// TODO: ) error
		}
		else
		{
			replacementBegin = parenOrReplacement;
		}

		// 定義内容を入力トークンリストから取り出して保持する
		SourceRange range = m_unitFile->SaveMacroReplacementTokens(keyword, lineEnd + 1);

		// マクロ登録
		// TODO: マクロの上書き確認
		//m_unitFile->m_macroMap->Insert(*macroName, range);
		m_macroMap.get()->Insert(*macroName, range);

		// Reference 作成 (Macro Define)
		m_referenceTracker->InjectReference(nullptr, macroName->GetStringRef(), EntityKind::Macro, ReferenceKindFlags::Define);
	}
	//---------------------------------------------------------
	// #if
	//		:: # if constant-expression new-line groupopt
	else if (keyword->EqualString("if", 2))
	{
		LN_RESULT_CALL(AnalyzeIfElifDirective(keyword, lineEnd, false));
	}
	//---------------------------------------------------------
	// #elif
	//		:: # elif constant-expression new-line groupopt
	else if (keyword->EqualString("elif", 4))
	{
		LN_RESULT_CALL(AnalyzeIfElifDirective(keyword, lineEnd, true));
	}
	//---------------------------------------------------------
	// #ifdef
	//		:: # ifdef identifier new-line groupopt
	// #ifndef
	//		:: # ifndef identifier new-line groupopt
	else if (
		keyword->EqualString("ifdef", 5) ||
		keyword->EqualString("ifndef", 6))
	{
		// 新しいセクションを開始する
		m_conditionalSectionStack.push(ConditionalSection());

		// 次の識別子まで進める
		Token* pos = ParserUtils::SkipNextSpaceOrComment(keyword, lineEnd);
		// Error: 識別子ではなかった
		LN_DIAG_REPORT_ERROR(pos->GetCommonType() == CommonTokenType::Identifier, DiagnosticsCode::Preprocessor_SyntaxError);

		// Reference 作成 (Macro Use)
		m_referenceTracker->InjectReference(nullptr, pos->GetStringRef(), EntityKind::Macro, ReferenceKindFlags::Use);

		// 現時点でマクロが定義されているかチェック
		bool isDefined = m_macroMap.GetConst()->IsDefined(*pos);

		// "ifndef" なら条件を反転
		if (keyword->getLength() == 6) {
			isDefined = !isDefined;
		}

		if (isDefined)
		{
			m_conditionalSectionStack.getTop().state = ConditionalSectionState::Valid;
		}
		else
		{
			m_conditionalSectionStack.getTop().state = ConditionalSectionState::Invalid;
		}
	}
	//---------------------------------------------------------
	// #else
	//		::	# else new-line groupopt
	else if (keyword->EqualString("else", 4))
	{
		if (m_conditionalSectionStack.IsEmpty() ||				// #if がない
			m_conditionalSectionStack.getTop().elseProcessed)	// 既に #else 受領済み
		{
			// Error: 予期しない #else
			m_diag->Report(DiagnosticsCode::Preprocessor_UnexpectedElse);
			return ResultState::Error;
		}

		if (m_conditionalSectionStack.getTop().state == ConditionalSectionState::Valid)
		{
			// 有効領域のあとの #else なので、後は何があろうと全て無効領域となる
			m_conditionalSectionStack.getTop().state = ConditionalSectionState::Skip;
		}
		else if (m_conditionalSectionStack.getTop().state == ConditionalSectionState::Invalid)
		{
			// いままで無効だったのでここから有効になる
			m_conditionalSectionStack.getTop().state = ConditionalSectionState::Valid;
		}
		else
		{
			// Skip のまま維持する
			m_conditionalSectionStack.getTop().state = ConditionalSectionState::Skip;
		}

		// else を処理した
		m_conditionalSectionStack.getTop().elseProcessed = true;
	}
	//---------------------------------------------------------
	// #endif
	//		::	# endif new-line
	else if (keyword->EqualString("endif", 5))
	{
		if (m_conditionalSectionStack.IsEmpty())
		{
			// Error: 予期しない #endif
			m_diag->Report(DiagnosticsCode::Preprocessor_UnexpectedEndif);
			return ResultState::Error;
		}
		m_conditionalSectionStack.pop();
	}
	//---------------------------------------------------------
	// #line
	//		::	# line pp-tokens new-line
	else if (keyword->EqualString("line", 4))
	{
		LN_NOTIMPLEMENTED();
	}
	//---------------------------------------------------------
	// #include
	//		:: # include pp-tokens new-line
	else if (keyword->EqualString("include", 7))
	{
		// 無効領域内なら何もしない
		if (!IsValidSection()) {
			return ResultState::Success;
		}

		// TODO: include にマクロ使えた気がする・・・

		AnalyzeIncludeDirective(keyword, lineEnd);
	}
	//---------------------------------------------------------
	// #pragma
	//		:: # pragma pp-tokensopt new-line
	else if (keyword->EqualString("pragma", 6))
	{
		// 無効領域内なら何もしない
		if (!IsValidSection()) {
			return ResultState::Success;
		}

		AnalyzePragmaDirective(keyword, lineEnd);
	}

	return ResultState::Success;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ResultState Preprocessor::AnalyzeIfElifDirective(Token* keyword, Token* lineEnd, bool isElse)
{
	// #if の場合
	if (!isElse)
	{
		// 新しいセクションを開始する
		m_conditionalSectionStack.push(ConditionalSection());
	}
	// #elif の場合
	else
	{
		if (m_conditionalSectionStack.IsEmpty() ||				// #if がない
			m_conditionalSectionStack.getTop().elseProcessed)	// 既に #else 受領済み
		{
			LN_DIAG_REPORT_ERROR(0, DiagnosticsCode::Preprocessor_UnexpectedElif);
		}

		if (m_conditionalSectionStack.getTop().state == ConditionalSectionState::Valid)
		{
			// 有効領域のあとの #else なので、後は何があろうと全て無効領域となる
			m_conditionalSectionStack.getTop().state = ConditionalSectionState::Skip;
			return ResultState::Success;
		}
		else if (m_conditionalSectionStack.getTop().state == ConditionalSectionState::Skip)
		{
			// 既に Skip 状態
			return ResultState::Success;
		}
	}

	// スペースを飛ばす
	Token* pos = ParserUtils::SkipNextSpaceOrComment(keyword, lineEnd);

	// Error: 定数式が無かった
	LN_DIAG_REPORT_ERROR(pos < lineEnd, DiagnosticsCode::Preprocessor_InvalidConstantExpression);

	// 定数式内のマクロを展開して RpnParser に掛けるためのトークンリストを作る
	m_preproExprTokenList.Clear();
	m_preproExprTokenList.Reserve(lineEnd - pos);		// マクロ展開で増えることはあるが、とりあえずこれだけあらかじめ確保しておく
	for (; pos < lineEnd;)
	{
		if (pos->GetCommonType() == CommonTokenType::Identifier)
		{
			MacroDefine* definedMacro;

			// defined ならその処理へ
			if (pos->EqualString("defined", 7))
			{
				Token* ident = nullptr;

				// スペースを飛ばす
				pos = ParserUtils::SkipNextSpaceOrComment(pos, lineEnd);
				if (pos->GetCommonType() == CommonTokenType::Identifier)
				{
					// 識別子だった。"#if defined AAA" のような形式。
					ident = pos;
				}
				else if (pos->GetCommonType() == CommonTokenType::Operator && pos->EqualChar('('))
				{
					// ( だった。さらに飛ばすと識別子、もうひとつ飛ばすと ')'
					ident = ParserUtils::SkipNextSpaceOrComment(pos, lineEnd);
					LN_DIAG_REPORT_ERROR(ident->GetCommonType() == CommonTokenType::Identifier, DiagnosticsCode::Preprocessor_ExpectedDefinedId);
					Token* paren = ParserUtils::SkipNextSpaceOrComment(ident, lineEnd);
					LN_DIAG_REPORT_ERROR(paren->GetCommonType() == CommonTokenType::Operator && paren->EqualChar(')'), DiagnosticsCode::Preprocessor_ExpectedDefinedId);
					++pos;
					++pos;
				}
				else
				{
					// Error: defined の後に識別子が必要
					LN_DIAG_REPORT_ERROR(0, DiagnosticsCode::Preprocessor_ExpectedDefinedId);
				}

				// マクロを探す
				if (m_macroMap.GetConst()->IsDefined(*ident)) {
					m_preproExprTokenList.Add(m_constTokenBuffer.Get1());	// "1" に展開
				}
				else {
					m_preproExprTokenList.Add(m_constTokenBuffer.Get0());	// "0" に展開
				}
				++pos;
			}
			// TODO: C++特有。CではNG?
			else if (pos->EqualString("true", 4))
			{
				m_preproExprTokenList.Add(m_constTokenBuffer.Get1());	// "1" に展開
				++pos;
			}
			// マクロかも
			else if (m_macroMap.GetConst()->IsDefined(*pos, &definedMacro))
			{
				const Token* begin;
				const Token* end;
				m_unitFile->GetMacroReplacementTokens(definedMacro->replacementRange, &begin, &end);
				for (; begin < end; ++begin) {
					m_preproExprTokenList.Add(*begin);
				}
				//definedMacro->AppendReplacementToTokenList(&m_preproExprTokenList);
				++pos;
			}
			// それ以外のただの識別子はすべて 0 にしなければならない
			else
			{
				m_preproExprTokenList.Add(m_constTokenBuffer.Get0());	// "0" に展開
				++pos;
			}
		}
		else
		{
			m_preproExprTokenList.Add(*pos);
			++pos;
		}
	}

	// 定数式を評価する
	LN_RESULT_CALL(m_rpnParser.ParseCppConstExpression2(m_preproExprTokenList.cbegin(), m_preproExprTokenList.cend(), m_diag));
	RpnOperand result;
	LN_RESULT_CALL(m_rpnEvaluator.TryEval(m_rpnParser.GetTokenList(), m_diag, &result));

	// 整数型と bool を許可
	if (result.IsIntager() || result.type == RpnOperandType::Boolean)
	{
		if (result.IsFuzzyTrue()) {	// 0 以外または true
			m_conditionalSectionStack.getTop().state = ConditionalSectionState::Valid;
		}
		else {
			m_conditionalSectionStack.getTop().state = ConditionalSectionState::Invalid;
		}
	}
	else
	{
		// Error: 整数定数式が必要です
		LN_DIAG_REPORT_ERROR(pos < lineEnd, DiagnosticsCode::Preprocessor_InvalidConstantExpression);
	}

	return ResultState::Success;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ResultState Preprocessor::AnalyzeIncludeDirective(Token* keyword, Token* lineEnd)
{
	// スペースを飛ばす
	Token* pos = ParserUtils::SkipNextSpaceOrComment(keyword, lineEnd);
	LN_DIAG_REPORT_ERROR(pos < lineEnd, DiagnosticsCode::Preprocessor_InvalidHeaderName);	// Error: ヘッダ名が無効です
	LN_DIAG_REPORT_ERROR(pos->GetLangTokenType() == TT_HeaderName, DiagnosticsCode::Preprocessor_InvalidHeaderName);
	
	// パス名前後のクォーテーションを除いて、パス名の範囲を求める
	const TokenChar* pathBegin = pos->GetBegin() + 1;
	const TokenChar* pathEnd = pos->GetEnd() - 1;

	// m_unitFile を基準に絶対パスにする。もし pathBegin～ が絶対パスならそれがそのまま使われる
	//TokenPathName absPath(m_unitFile->GetDirectoryPath(), TokenStringRef(pathBegin, pathEnd));

	// プリプロセス済みのトークンリストを持ったコードを探す (見つかり次第、プリプロ解析が行われる。再帰。)
	TokenPathName filePath(TokenStringRef(pathBegin, pathEnd));
	IncludeFile* includeFile;
	LN_RESULT_CALL(m_ownerContext->LookupPreprocessedIncludeFile(m_compileUnitFile, m_unitFile->GetDirectoryPath(), filePath, m_additionalIncludePaths, m_macroMap, m_referenceTracker, m_diag, &includeFile));

	// include したファイルとして記憶
	m_compileUnitFile->includedFiles.Add(includeFile);

	// マクロマップを付け替える
	m_macroMap = *includeFile->GetMacroMapPtr();
	//m_unitFile->SetMacroMap(*includeFile->GetMacroMapPtr());



	return ResultState::Success;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ResultState Preprocessor::AnalyzePragmaDirective(Token* keyword, Token* lineEnd)
{
	// 空白類を飛ばす
	Token* pos = ParserUtils::SkipNextSpaceOrComment(keyword, lineEnd);
	if (pos->GetCommonType() == CommonTokenType::Identifier)
	{
		if (pos->EqualString("once", 4))
		{
			m_foundPragmaOnce = true;
		}
	}

	return ResultState::Success;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool Preprocessor::IsValidSection() const
{
	if (m_conditionalSectionStack.IsEmpty())
	{
		return true;
	}
	else if (
		m_conditionalSectionStack.getTop().state == ConditionalSectionState::None ||
		m_conditionalSectionStack.getTop().state == ConditionalSectionState::Valid)
	{
		return true;
	}
	return false;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
//TokenList::iterator Preprocessor::GetNextGenericToken(TokenList::iterator pos)
//{
//	while (!pos->IsEof())
//	{
//		++pos;
//		if (pos->GetCommonType() != CommonTokenType::SpaceSequence &&
//			pos->GetCommonType() != CommonTokenType::Comment)
//		{
//			return pos;
//		}
//	}
//	return pos;
//}

} // namespace Parser
LN_NAMESPACE_END

#endif
