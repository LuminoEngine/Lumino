
#include "../Internal.h"
#include <Fluorite/Diagnostics.h>
#include "CppLexer.h"

namespace fl {

//==============================================================================
// CppLexer
//==============================================================================

//------------------------------------------------------------------------------
CppLexer::CppLexer()
	: m_seqPPDirective(PPDirectiveSeq::LineHead)
{
}

//------------------------------------------------------------------------------
CppLexer::~CppLexer()
{
}

//------------------------------------------------------------------------------
int CppLexer::ReadToken(const Range& buffer)
{
	int len;

	// 空白並び
	len = ReadSpaceSequence(buffer);
	if (len > 0) { return len; }
	// #include のヘッダ名
	if (m_seqPPDirective == PPDirectiveSeq::ReadingPPHeaderName)
	{
		len = ReadPPHeaderName(buffer);
		if (len > 0) { return len; }
	}
	// プリプロセッサトークン列
	else if (m_seqPPDirective == PPDirectiveSeq::ReadingPPTokens)
	{
		len = ReadPPTokens(buffer);
		if (len > 0) { return len; }
	}
	// 改行
	len = ReadNewLine(buffer);
	if (len > 0) { return len; }
	// 予約語
	len = ReadKeyword(buffer);
	if (len > 0) { return len; }
	// 文字リテラル
	len = ReadCharLiteral(buffer);
	if (len > 0) { return len; }
	// 文字列リテラル
	len = ReadStringLiteral(buffer);
	if (len > 0) { return len; }
	// 識別子
	len = ReadIdentifier(buffer);
	if (len > 0) { return len; }
	// 数値リテラル
	len = ReadNumericLiteral(buffer);
	if (len > 0) { return len; }
	// ブロックコメント
	len = ReadBlockComment(buffer);
	if (len > 0) { return len; }
	// 行コメント (演算子より先に見ておく)
	len = ReadLineComment(buffer);
	if (len > 0) { return len; }
	// 演算子
	len = ReadOperator(buffer);
	if (len > 0) { return len; }
	// 行末エスケープ
	len = ReadEscapeNewLine(buffer);
	if (len > 0) { return len; }
	// マルチバイト文字並び
	len = ReadMBSSequence(buffer);
	if (len > 0) { return len; }
	

	/* 文字列リテラルは識別子の前に解析する。これは、L 等のプレフィックス対応のため。
	* なお、CheckHexLiteralStart() は 0x をプレフィックスとして特別扱いしているが、
	* 文字列ではこのように L を特別扱いはしない。
	* これは C++11 の raw string 対応を視野に入れたもの。
	*
	* raw string は次のように書くことができる。
	*		str = R"**(AAA )" BBB)**";
	* このリテラルでは、
	* ・開始トークン		R"**(
	* ・文字列			AAA )" BBB
	* ・終端トークン		)**"
	* である。
	*
	* プレフィックスは L R U 等様々あり、これらを特別扱いしてパースしても、
	* 次の " 時には「何の種類であるか」を CheckStringStart() に渡さなければならない。
	* プレフィックスと " の間に空白が可能であればまだしも、わざわざ特別扱いして関数を分けると逆に複雑になってしまう。
	*/
	return 0;
}

//------------------------------------------------------------------------------
void CppLexer::PollingToken(Token* token)
{
	// 何もしていない。改行を探す。
	if (m_seqPPDirective == PPDirectiveSeq::Idle)
	{
		if (token->GetTokenGroup() == TokenGroup::NewLine)
		{
			m_seqPPDirective = PPDirectiveSeq::LineHead;		// 改行が見つかった。行頭状態へ
		}
	}
	// 行頭にいる。# を探す。
	else if (m_seqPPDirective == PPDirectiveSeq::LineHead)
	{
		if (token->GetTokenGroup() == TokenGroup::Operator &&
			token->getTokenType() == TT_CppOP_Sharp)
		{
			m_seqPPDirective = PPDirectiveSeq::FoundSharp;	// "#" を見つけた
		}
		else if (token->GetTokenGroup() == TokenGroup::NewLine)
		{
			// LineHead のまま
		}
		else
		{
			m_seqPPDirective = PPDirectiveSeq::Idle;		// "#" 以外のトークンだった。Idle へ。
		}
	}
	// # まで見つけている。次の "include" を探す。
	else if (m_seqPPDirective == PPDirectiveSeq::FoundSharp)
	{
		if (EqualsString(token, "if", 2) ||
			EqualsString(token, "ifdef", 5) ||
			EqualsString(token, "ifndef", 6) ||
			EqualsString(token, "elif", 4) ||
			EqualsString(token, "else", 4) ||
			EqualsString(token, "define", 6) ||
			EqualsString(token, "undef", 5))
		{
			m_seqPPDirective = PPDirectiveSeq::Idle;	// 以降、pp-tokens として解析せず、普通のトークン分割をまわす
			//m_seqPPDirective = PPDirectiveSeq::FoundInclude;	// "include" を見つけた
		}
		else if (EqualsString(token, "include", 7))
		{
			m_seqPPDirective = PPDirectiveSeq::ReadingPPHeaderName;	// "include" を見つけたので HeaderName の解析へ
		}
		else
		{
			m_seqPPDirective = PPDirectiveSeq::ReadingPPTokens;	// 以降、改行までは pp-tokens として読み取る
			//m_seqPPDirective = PPDirectiveSeq::Idle;		// #" 以外のトークンだった。"include" 以外のプリプロディレクティブ。
		}
	}
	// ～ 行末
	else if (m_seqPPDirective == PPDirectiveSeq::ReadingPPHeaderName ||
			 m_seqPPDirective == PPDirectiveSeq::ReadingPPTokens)
	{
		if (token->GetTokenGroup() == TokenGroup::NewLine)
		{
			m_seqPPDirective = PPDirectiveSeq::LineHead;		// 改行が見つかった。行頭状態へ
		}
	}
}

//------------------------------------------------------------------------------
void CppLexer::onStart()
{
	AbstractLexer::onStart();

	// 初期状態は改行直後扱いとする。ファイル先頭に # ディレクティブがあることに備える。
	m_seqPPDirective = PPDirectiveSeq::LineHead;
}

//------------------------------------------------------------------------------
int CppLexer::IsSpaceChar(const Range& r)
{
	if (r.pos[0] == ' ' || r.pos[0] == '\t' || r.pos[0] == '\f' || r.pos[0] == '\v')
	{
		return 1;
	}

	//if (isspace(*buffer))
	//	return 1;
	//if (C# の時は UTF-8全角スペースとか)
	//	return 3;
	return 0;
}

//------------------------------------------------------------------------------
int CppLexer::ReadSpaceSequence(const Range& buffer)
{
	// 連続するスペース文字の数を返す
	// (全角スペースを許容する場合はそれ全体の文字数もカウント)
	Range r = buffer;
	while (r.pos < r.end)
	{
		int len = IsSpaceChar(r);
		if (len == 0) {
			break;
		}
		r.pos += len;
	}

	// トークン作成
	if (buffer.pos < r.pos) {
		AddToken(TokenGroup::SpaceSequence, buffer.pos, r.pos);
	}
	return r.pos - buffer.pos;
}

//------------------------------------------------------------------------------
int CppLexer::IsKeyword(const Range& buffer, int* langTokenType)
{
	struct WordData
	{
		const char*		word;
		int				length;
		int				type;
	};
	static const WordData wordList[] =
	{
#define TChar TokenChar	// 以下、古いコードのテーブルを使いまわしているのでそのケア
#undef LN_T
#define LN_T(t, s) s
		{ LN_T(TChar, "asm"),					3,	TT_CppKW_asm },
		{ LN_T(TChar, "auto"),					4,	TT_CppKW_auto },
		{ LN_T(TChar, "bool"),					4,	TT_CppKW_bool },
		{ LN_T(TChar, "break"),					5,	TT_CppKW_break },
		{ LN_T(TChar, "case"),					4,	TT_CppKW_case },
		{ LN_T(TChar, "catch"),					5,	TT_CppKW_catch },
		{ LN_T(TChar, "char"),					4,	TT_CppKW_char },
		{ LN_T(TChar, "class"),					5,	TT_CppKW_class },
		{ LN_T(TChar, "const"),					5,	TT_CppKW_const },
		{ LN_T(TChar, "const_cast"),			10,	TT_CppKW_const_cast },
		{ LN_T(TChar, "continue"),				8,	TT_CppKW_continue },
		{ LN_T(TChar, "default"),				7,	TT_CppKW_default },
		{ LN_T(TChar, "delete"),				6,	TT_CppKW_delete },
		{ LN_T(TChar, "do"),					2,	TT_CppKW_do },
		{ LN_T(TChar, "double"),				6,	TT_CppKW_double },
		{ LN_T(TChar, "dynamic_cast"),			12,	TT_CppKW_dynamic_cast },
		{ LN_T(TChar, "else"),					4,	TT_CppKW_else },
		{ LN_T(TChar, "enum"),					4,	TT_CppKW_enum },
		{ LN_T(TChar, "explicit"),				8,	TT_CppKW_explicit },
		{ LN_T(TChar, "export"),				6,	TT_CppKW_export },
		{ LN_T(TChar, "extern"),				6,	TT_CppKW_extern },
		{ LN_T(TChar, "false"),					5,	TT_CppKW_false },
		{ LN_T(TChar, "float"),					5,	TT_CppKW_float },
		{ LN_T(TChar, "for"),					3,	TT_CppKW_for },
		{ LN_T(TChar, "friend"),				6,	TT_CppKW_friend },
		{ LN_T(TChar, "goto"),					4,	TT_CppKW_goto },
		{ LN_T(TChar, "if"),					2,	TT_CppKW_if },
		{ LN_T(TChar, "inline"),				6,	TT_CppKW_inline },
		{ LN_T(TChar, "int"),					3,	TT_CppKW_int },
		{ LN_T(TChar, "long"),					4,	TT_CppKW_long },
		{ LN_T(TChar, "mutable"),				7,	TT_CppKW_mutable },
		{ LN_T(TChar, "namespace"),				9,	TT_CppKW_namespace },
		{ LN_T(TChar, "new"),					3,	TT_CppKW_new },
		{ LN_T(TChar, "operator"),				8,	TT_CppKW_operator },
		{ LN_T(TChar, "private"),				7,	TT_CppKW_private },
		{ LN_T(TChar, "protected"),				9,	TT_CppKW_protected },
		{ LN_T(TChar, "public"),				6,	TT_CppKW_public },
		{ LN_T(TChar, "register"),				8,	TT_CppKW_register },
		{ LN_T(TChar, "reinterpret_cast"),		16,	TT_CppKW_reinterpret_cast },
		{ LN_T(TChar, "return"),				6,	TT_CppKW_return },
		{ LN_T(TChar, "short"),					5,	TT_CppKW_short },
		{ LN_T(TChar, "signed"),				6,	TT_CppKW_signed },
		{ LN_T(TChar, "sizeof"),				6,	TT_CppKW_sizeof },
		{ LN_T(TChar, "static"),				6,	TT_CppKW_static },
		{ LN_T(TChar, "static_cast"),			11,	TT_CppKW_static_cast },
		{ LN_T(TChar, "struct"),				6,	TT_CppKW_struct },
		{ LN_T(TChar, "switch"),				6,	TT_CppKW_switch },
		{ LN_T(TChar, "template"),				8,	TT_CppKW_template },
		{ LN_T(TChar, "this"),					4,	TT_CppKW_this },
		{ LN_T(TChar, "throw"),					5,	TT_CppKW_throw },
		{ LN_T(TChar, "true"),					4,	TT_CppKW_true },
		{ LN_T(TChar, "try"),					3,	TT_CppKW_try },
		{ LN_T(TChar, "typedef"),				7,	TT_CppKW_typedef },
		{ LN_T(TChar, "typeid"),				6,	TT_CppKW_typeid },
		{ LN_T(TChar, "typename"),				8,	TT_CppKW_typename },
		{ LN_T(TChar, "union"),					5,	TT_CppKW_union },
		{ LN_T(TChar, "unsigned"),				8,	TT_CppKW_unsigned },
		{ LN_T(TChar, "using"),					5,	TT_CppKW_using },
		{ LN_T(TChar, "virtual"),				7,	TT_CppKW_virtual },
		{ LN_T(TChar, "void"),					4,	TT_CppKW_void },
		{ LN_T(TChar, "volatile"),				8,	TT_CppKW_volatile },
		{ LN_T(TChar, "wchar_t"),				7,	TT_CppKW_wchar_t },
		{ LN_T(TChar, "while"),					5,	TT_CppKW_while },
	};

	const int len = LN_ARRAY_SIZE_OF(wordList);
	int keyLen = 0;
	int type = 0;
	for (int i = 0; i < len; ++i) 
	{
		if (wordList[i].word[0] == buffer.pos[0] &&		// まずは先頭文字を調べて
			StringTraits::strncmp(wordList[i].word, buffer.pos, wordList[i].length) == 0)	// 先頭が一致したら残りを調べる
		{ 
			type = (int)wordList[i].type;
			keyLen = wordList[i].length;
			break;
		}
	}

	// 本当にキーワードが識別子として完結している？
	if (keyLen > 0 && IsIdentifierLetter(Range(buffer.pos + keyLen, buffer.end)) == 0)
	{
		*langTokenType = type;
		return keyLen;
	}
	return 0;
}

//------------------------------------------------------------------------------
int CppLexer::ReadKeyword(const Range& buffer)
{
	int lnagTokenType = 0;
	int len = IsKeyword(buffer, &lnagTokenType);
	if (len > 0) {
		AddToken(TokenGroup::Keyword, buffer.pos, buffer.pos + len, lnagTokenType);
		return len;
	}
	return 0;
}

//------------------------------------------------------------------------------
int CppLexer::ReadEnclosingTokenHelper(const Range& buffer, TokenCheckCallback startCallback, TokenCheckCallback endCallback, const char* chars, bool* outNotFoundEndToken)
{
	Range r = buffer;
	*outNotFoundEndToken = true;

	// 文字列の開始チェック
	int startCount = startCallback(r);
	if (startCount == 0) {
		return 0;
	}
	r.pos += startCount;

	while (r.pos < r.end)
	{
		// エスケープシーケンスのチェック
		int len = 0;
		if (chars != nullptr && r.pos[0] == '\\')
		{
			for (const char* c = chars; *c != '\0'; ++c)
			{
				if (r.pos[1] == *c)
				{
					len = 2;
					break;
				}
			}
		}
		if (len > 0)
		{
			r.pos += len;
			continue;	// この後の解析には回さないで次の文字へ
		}

		// 終端チェック
		len = endCallback(r);
		if (len > 0) {
			r.pos += len;
			*outNotFoundEndToken = false;
			break;		// 終了
		}

		r.pos++;
	}

	return r.pos - buffer.pos;
}

//------------------------------------------------------------------------------
int CppLexer::ReadCharLiteral(const Range& buffer)
{
	//const TokenChar chars[] = { '\'', '"', '?', '\\', 'a', 'b', 'f', 'n', 'r', 't', 'v', '\0' };
	const char chars[] = { '\'', '\0' };	// 分割が目的なので ' だけエスケープでOK
	bool notFoundEndToken;
	int len = ReadEnclosingTokenHelper(buffer, IsCharLiteralStart, IsCharLiteralEnd, chars, &notFoundEndToken);

	if (len > 0)
	{
		int type = TT_NumericLitaralType_Char;
		if (buffer.pos[0] == 'L') {
			type = TT_NumericLitaralType_WideChar;
		}
		AddToken(TokenGroup::ArithmeticLiteral, buffer.pos, buffer.pos + len, type);
	}
	return len;
}

//------------------------------------------------------------------------------
int CppLexer::IsCharLiteralStart(const Range& buffer)
{
	if (buffer.pos[0] == '\'') {
		return 1;
	}
	if (buffer.pos[0] == 'L' && buffer.pos[1] == '\'') {
		return 2;	// L プレフィックス付き文字列リテラル
	}
	return 0;
}

//------------------------------------------------------------------------------
int CppLexer::IsCharLiteralEnd(const Range& buffer)
{
	if (buffer.pos[0] == '\'') {
		return 1;
	}
	return 0;
}

//------------------------------------------------------------------------------
//int CppLexer::IsAnyChar(const Range& buffer, const TokenChar* chars, int len)
//{
//	for (int i = 0; i < len; ++i)
//	{
//		if (buffer.pos[0] == chars[i])
//		{
//			return 1;
//		}
//	}
//	return 0;
//}


//------------------------------------------------------------------------------
int CppLexer::ReadStringLiteral(const Range& buffer)
{
	bool notFoundEndToken;

	int len = 0;
	//if (m_seqPPDirective != PPDirectiveSeq::FoundInclude)
	//{
		//const TokenChar chars[] = { '\'', '"', '?', '\\', 'a', 'b', 'f', 'n', 'r', 't', 'v', '\0' };
		const char chars[] = { '"', '\0' };	// 分割が目的なので " だけエスケープでOK
		len = ReadEnclosingTokenHelper(buffer, IsStringLiteralStart, IsStringLiteralEnd, chars, &notFoundEndToken);
	//}
	//else
	//{
	//	// #include 内の場合はこちら。エスケープは無い
	//	len = ReadEnclosingTokenHelper(buffer, IsStringLiteralStartInIncludeDirective, IsStringLiteralEndIncludeDirective, nullptr, &notFoundEndToken);
	//}

	if (len > 0)
	{
		int type = TT_NumericLitaralType_AsciiString;
		if (buffer.pos[0] == 'L') {
			type = TT_NumericLitaralType_WideString;
		}
		AddToken(TokenGroup::StringLiteral, buffer.pos, buffer.pos + len, type);
	}
	return len;
}

//------------------------------------------------------------------------------
int CppLexer::IsStringLiteralStart(const Range& buffer)
{
	if (buffer.pos[0] == '"') {
		return 1;
	}
	if (buffer.pos[0] == 'L' && buffer.pos[1] == '"') {
		return 2;	// L プレフィックス付き文字列リテラル
	}
	return 0;
}

//------------------------------------------------------------------------------
int CppLexer::IsStringLiteralEnd(const Range& buffer)
{
	if (buffer.pos[0] == '"') {
		return 1;
	}
	return 0;
}

//------------------------------------------------------------------------------
int CppLexer::ReadIdentifier(const Range& buffer)
{
	/*
		a b c d e f g h i j k l m
		n o p q r s t u v w x y z
		A B C D E F G H I J K L M
		N O P Q R S T U V W X Y Z _
		0 1 2 3 4 5 6 7 8 9
	*/
	Range r = buffer;

	// 識別子の先頭文字か？
	int len = IsIdentifierStart(buffer);
	if (len == 0) {
		return 0;	// 識別子ではない
	}
	r.pos += len;

	// 後に続く文字は正しいか？
	while (r.pos < r.end)
	{
		len = IsIdentifierLetter(r);
		if (len == 0) {
			break;
		}
		r.pos += len;
	}

	// トークン作成
	AddToken(TokenGroup::Identifier, buffer.pos, r.pos);
	return r.pos - buffer.pos;
}

//------------------------------------------------------------------------------
int CppLexer::IsIdentifierStart(const Range& buffer)
{
	if (isalpha(buffer.pos[0]) || buffer.pos[0] == '_')
	{
		return 1;
	}
	return 0;
}

//------------------------------------------------------------------------------
int CppLexer::IsIdentifierLetter(const Range& buffer)
{
	if (isalnum(buffer.pos[0]) || buffer.pos[0] == '_')
	{
		return 1;
	}
	return 0;
}

//------------------------------------------------------------------------------
int CppLexer::ReadNumericLiteral(const Range& buffer)
{
	/* 123	10進数リテラル
	* 012	8進数リテラル
	*
	*/

	// TODO: この内容は LangDef に持っていくべきかも？

	bool	bExistPeriod = false;		/* 小数点が登場したらTRUE */
	bool	bExistExp = false;		/* 指数が登場したらTRUE */
	bool	bExistSuffix = false;		/* 接尾語が登場したらTRUE */
	bool	bHexMode = false;		/* 16進数で有る場合にTRUE */

	// 数値リテラルの型。サフィックスとかなければデフォルトは Int32
	int	litaralType = TT_NumericLitaralType_Int32;

	// 16進数の先頭チェック
	//const TokenChar* pPos = buffer.pos;
	Range r = buffer;
	int len = IsHexLiteralStart(r);
	if (len > 0) {
		r.pos += len;
		bHexMode = true;	// 16 進数である
	}

	int nStep = 0;	// 状態遷移を表す

	bool bEnd = false;
	while (r.pos < r.end && !bEnd)
	{
		if (r.pos[0] > 255) {	// wchar_t の範囲チェック
			break;
		}
		AlphaNumTypeFlags charType = GetAlphaNumType(r.pos[0]);

		// マルチバイトコードまたは制御文字または . 以外の演算子であれば終了
		if (charType.TestFlag(AlphaNumTypeFlags::MBC) ||
			charType.TestFlag(AlphaNumTypeFlags::Control) ||
			(charType.TestFlag(AlphaNumTypeFlags::OpChar) && r.pos[0] != '.')){
			break;	// 終了
		}

		// サフィックスのチェック (サフィックスは一番最後だが、いろいろなところに書ける。1f 1.0f .1f 1.0-e2f)
		if (charType.TestFlag(AlphaNumTypeFlags::Alphabet) /*|| charType.TestFlag(AlphaNumTypeFlags::HexAlpha)*/)
		{
			// 整数型
			len = IsIntegerSuffix(r, &litaralType);
			if (len > 0) {
				r.pos += len;
				bExistSuffix = true;	// サフィックスを見つけた
				nStep = 10;
				continue;
			}
			// 実数型
			if (!bHexMode)	// 16新数の F と float の F を区別したい
			{
				len = IsRealSuffix(r, &litaralType);
				if (len > 0) {
					r.pos += len;
					bExistSuffix = true;	// サフィックスを見つけた
					nStep = 10;
					continue;
				}
			}
			// ※ 50LL のように重複の可能性があるため、複数見つかってもエラーにしない
		}

		if (bExistSuffix) {
			return 0;	// サフィックスの後に、サフィックスではない文字が来た。おかしい
		}

		switch (nStep)
		{
		case 0:		// 整数部分
		{
			if (bHexMode && charType.TestFlag(AlphaNumTypeFlags::HexAlpha)) {
				r.pos++;		// 16 進数なら HexAlpha を許可して継続
			}
			else if (charType.TestFlag(AlphaNumTypeFlags::Number)) {
				r.pos++;		// 普通の数値も OK
			}
			else if (charType.TestFlag(AlphaNumTypeFlags::OpChar) && r.pos[0] == '.') {
				nStep = 1;	// . が見つかった。小数部分へ移行
				r.pos++;
				litaralType = TT_NumericLitaralType_Double;	// とりあえず Double 型とする
			}
			else {
				len = IsExponentStart(r);
				if (len > 0)		// e E + -
				{
					nStep = 2;		// e- とかが見つかった。指数部分へ移行
					r.pos += len;
					litaralType = TT_NumericLitaralType_Double;	// 指数部があるのでとりあえず Double 型とする
				}
				else {
					return 0;		// 10.5G 等、変なサフィックスとか、次の識別子との間にスペースが無いとか
				}
			}
			break;
		}
		case 1:		// 小数部分
		{
			if (bHexMode && charType.TestFlag(AlphaNumTypeFlags::HexAlpha)) {
				r.pos++;		// 16 進数なら HexAlpha を許可して継続
			}
			else if (charType.TestFlag(AlphaNumTypeFlags::Number)) {
				r.pos++;		// 普通の数値も OK
			}
			else
			{
				len = IsExponentStart(r);
				if (len > 0)		// e E + -
				{
					nStep = 2;		// e- とかが見つかった。指数部分へ移行
					r.pos += len;
					//litaralType = TT_NumericLitaralType_Double;	// 指数部があるのでとりあえず Double 型とする
				}
				else {
					return 0;		// 10.5G 等、変なサフィックスとか、次の識別子との間にスペースが無いとか
				}
			}
			break;
		}
		case 2:		// 指数部分
		{
			if (charType.TestFlag(AlphaNumTypeFlags::Number)) {
				r.pos++;		// 普通の数値は OK
			}
			else {
				return 0;	// それ以外は失敗
			}
			break;
		}
		}
	}

	if (r.pos - buffer.pos > 0)
	{
		// ここまで来たら解析成功
		AddToken(TokenGroup::ArithmeticLiteral, buffer.pos, r.pos, litaralType);
	}
	return r.pos - buffer.pos;
}

//------------------------------------------------------------------------------
int CppLexer::IsHexLiteralStart(const Range& buffer)
{
	if (buffer.pos[0] == '0' && (buffer.pos[1] == 'x' || buffer.pos[1] == 'X'))
		return 2;
	return 0;
	// VB はたしか &H
}

//------------------------------------------------------------------------------
int CppLexer::IsIntegerSuffix(const Range& buffer, int* outLiteralType)
{
	/*
		u U
		l L
		ul Ul uL UL
		ll LL
		ull Ull uLL uLL
	*/
	if (buffer.pos[0] == 'u' || buffer.pos[0] == 'U')
	{
		if (buffer.pos[1] == 'l' || buffer.pos[1] == 'L')
		{
			if (buffer.pos[2] == 'l' || buffer.pos[2] == 'L')
			{
				*outLiteralType = TT_NumericLitaralType_UInt64;
				return 3;
			}
			else
			{
				*outLiteralType = TT_NumericLitaralType_UInt32;
				return 2;
			}
		}
		else
		{
			*outLiteralType = TT_NumericLitaralType_UInt32;
			return 1;
		}
	}

	if (buffer.pos[0] == 'l' || buffer.pos[0] == 'L')
	{
		if (buffer.pos[1] == 'l' || buffer.pos[1] == 'L')
		{
			*outLiteralType = TT_NumericLitaralType_Int64;
			return 2;
		}
		else
		{
			*outLiteralType = TT_NumericLitaralType_Int32;
			return 1;
		}
	}

	return 0;
}

//------------------------------------------------------------------------------
int CppLexer::IsRealSuffix(const Range& buffer, int* outLiteralType)
{
	if (buffer.pos[0] == 'f' || buffer.pos[0] == 'F')
	{
		*outLiteralType = TT_NumericLitaralType_Float;
		return 1;
	}
	if (buffer.pos[0] == 'd' || buffer.pos[0] == 'D')
	{
		*outLiteralType = TT_NumericLitaralType_Double;
		return 1;
	}
	return 0;
}

//------------------------------------------------------------------------------
int CppLexer::IsExponentStart(const Range& buffer)
{
	if (buffer.pos[0] == 'e' ||
		buffer.pos[0] == 'E')
	{
		if (buffer.pos[1] == '+' ||
			buffer.pos[1] == '-') {
			return 2;
		}
		return 1;
	}
	return 0;
}

//------------------------------------------------------------------------------
int CppLexer::ReadBlockComment(const Range& buffer)
{
	bool notFoundEndToken;
	int len = ReadEnclosingTokenHelper(buffer, IsBlockCommentStart, IsBlockCommentEnd, nullptr, &notFoundEndToken);
	if (len > 0)
	{
		if (notFoundEndToken)
		{
			getDiag()->Report(DiagnosticsCode::UnexpectedEOFInBlockComment);
			return 0;
		}
		AddToken(TokenGroup::Comment, buffer.pos, buffer.pos + len);
	}
	return len;
}

//------------------------------------------------------------------------------
int CppLexer::IsBlockCommentStart(const Range& buffer)
{
	if (buffer.pos[0] == '/' &&
		buffer.pos[1] == '*')
		return 2;
	return 0;
}

//------------------------------------------------------------------------------
int CppLexer::IsBlockCommentEnd(const Range& buffer)
{
	if (buffer.pos[0] == '*' &&
		buffer.pos[1] == '/')
		return 2;
	return 0;
}

//------------------------------------------------------------------------------
int CppLexer::ReadLineComment(const Range& buffer)
{
	// 行コメントの開始チェック
	int len = IsLineCommentStart(buffer);
	if (len == 0) {
		return 0;
	}

	Range r = buffer;
	while (r.pos < r.end)
	{
		// 行末 \ チェック
		len = IsEscapeNewLine(r);
		if (len > 0) {
			r.pos += len;
			continue;		// 継続
		}

		// 終了チェック (EOF は end 到達でループ抜けるので特に気にしなくて良い)
		len = IsNewLine(r);
		if (len > 0) {
			// 改行はコメントには含まない
			break;			// 終了
		}

		++r.pos;
	}

	AddToken(TokenGroup::Comment, buffer.pos, r.pos);
	return r.pos - buffer.pos;
}

//------------------------------------------------------------------------------
int CppLexer::IsLineCommentStart(const Range& buffer)
{
	if (buffer.pos[0] == '/' &&
		buffer.pos[1] == '/')
		return 2;
	return 0;
}

//------------------------------------------------------------------------------
int CppLexer::ReadOperator(const Range& buffer)
{
	int lnagTokenType = 0;
	int len = IsOperator(buffer, &lnagTokenType);
	if (len > 0) {
		AddToken(TokenGroup::Operator, buffer.pos, buffer.pos + len, lnagTokenType);
		return len;
	}
	return 0;
}

//------------------------------------------------------------------------------
int CppLexer::IsOperator(const Range& buffer, int* langTokenType)
{
	if (GetAlphaNumType(buffer.pos[0]).TestFlag(AlphaNumTypeFlags::OpChar))
	{
		struct WordData
		{
			const char*		word;
			int				length;
			int				type;
		};
		// ++ と + のように、1文字目が同じものは文字数の多い方が先に検索されるようにする
		static const WordData wordList[] =
		{
			{ LN_T(TChar, "##"),	2,	TT_CppOP_SharpSharp },
			{ LN_T(TChar, "#"),		1,	TT_CppOP_Sharp },
			{ LN_T(TChar, "->*"),	3,	TT_CppOP_ArrowAsterisk },
			{ LN_T(TChar, "->"),	2,	TT_CppOP_Arrow },
			{ LN_T(TChar, ","),		1,	TT_CppOP_Comma },
			{ LN_T(TChar, "++"),	2,	TT_CppOP_Increment },
			{ LN_T(TChar, "--"),	2,	TT_CppOP_Decrement },
			{ LN_T(TChar, "&&"),	2,	TT_CppOP_LogicalAnd },
			{ LN_T(TChar, "||"),	2,	TT_CppOP_LogicalOr },
			{ LN_T(TChar, "<="),	2,	TT_CppOP_LessThenEqual },
			{ LN_T(TChar, ">="),	2,	TT_CppOP_GreaterThenEqual },
			{ LN_T(TChar, "=="),	2,	TT_CppOP_CmpEqual },
			{ LN_T(TChar, "<<="),	3,	TT_CppOP_LeftShiftEqual },
			{ LN_T(TChar, ">>="),	3,	TT_CppOP_RightShiftEqual },
			{ LN_T(TChar, "+="),	2,	TT_CppOP_PlusEqual },
			{ LN_T(TChar, "-="),	2,	TT_CppOP_MinusEqual },
			{ LN_T(TChar, "*="),	2,	TT_CppOP_MulEqual },
			{ LN_T(TChar, "/="),	2,	TT_CppOP_DivEqual },
			{ LN_T(TChar, "%="),	2,	TT_CppOP_ModEqual },
			{ LN_T(TChar, "&="),	2,	TT_CppOP_AndEqual },
			{ LN_T(TChar, "|="),	2,	TT_CppOP_OrEqual },
			{ LN_T(TChar, "!="),	2,	TT_CppOP_NotEqual },
			{ LN_T(TChar, "="),		1,	TT_CppOP_Equal },
			{ LN_T(TChar, "<<"),	2,	TT_CppOP_LeftShift },
			{ LN_T(TChar, ">>"),	2,	TT_CppOP_RightShift },
			{ LN_T(TChar, "+"),		1,	TT_CppOP_Plus },
			{ LN_T(TChar, "-"),		1,	TT_CppOP_Minus },
			{ LN_T(TChar, "*"),		1,	TT_CppOP_Asterisk },
			{ LN_T(TChar, "/"),		1,	TT_CppOP_Slash },
			{ LN_T(TChar, "%"),		1,	TT_CppOP_Parseint },
			{ LN_T(TChar, "&"),		1,	TT_CppOP_Ampersand },
			{ LN_T(TChar, "|"),		1,	TT_CppOP_Pipe },
			{ LN_T(TChar, "~"),		1,	TT_CppOP_Tilde },
			{ LN_T(TChar, "^"),		1,	TT_CppOP_Caret },
			{ LN_T(TChar, "!"),		1,	TT_CppOP_Exclamation },
			{ LN_T(TChar, "..."),	3,	TT_CppOP_Ellipsis },
			{ LN_T(TChar, ".*"),	2,	TT_CppOP_DotAsterisk },
			{ LN_T(TChar, "."),		1,	TT_CppOP_Dot },
			{ LN_T(TChar, "::"),	2,	TT_CppOP_DoubleColon },
			{ LN_T(TChar, "?"),		1,	TT_CppOP_Question },
			{ LN_T(TChar, ":"),		1,	TT_CppOP_Colon },
			{ LN_T(TChar, ";"),		1,	TT_CppOP_Semicolon },
			{ LN_T(TChar, "{"),		1,	TT_CppOP_LeftBrace },
			{ LN_T(TChar, "}"),		1,	TT_CppOP_RightBrace },
			{ LN_T(TChar, "["),		1,	TT_CppOP_LeftBracket },
			{ LN_T(TChar, "]"),		1,	TT_CppOP_RightBracket },
			{ LN_T(TChar, "("),		1,	TT_CppOP_LeftParen },
			{ LN_T(TChar, ")"),		1,	TT_CppOP_RightParen },
			{ LN_T(TChar, "<"),		1,	TT_CppOP_LeftAngle },
			{ LN_T(TChar, ">"),		1,	TT_CppOP_RightAngle },
		};
		const int count = LN_ARRAY_SIZE_OF(wordList);
		for (int i = 0; i < count; ++i)
		{
			if (wordList[i].word[0] == buffer.pos[0] &&		// まずは先頭文字を調べて
				StringTraits::strncmp(wordList[i].word, buffer.pos, wordList[i].length) == 0)	// 先頭が一致したら残りを調べる
			{
				*langTokenType = (int)wordList[i].type;
				return wordList[i].length;
			}
		}
		return 0;
	}
	return 0;
}

//------------------------------------------------------------------------------
int CppLexer::ReadEscapeNewLine(const Range& buffer)
{
	int len = IsEscapeNewLine(buffer);
	if (len > 0) {
		AddToken(TokenGroup::SpaceSequence, buffer.pos, buffer.pos + len, TT_EscapeNewLine);
		return len;
	}
	return 0;
}

//------------------------------------------------------------------------------
int CppLexer::IsEscapeNewLine(const Range& buffer)
{
	if (buffer.pos[0] == '\\')
	{
		if (buffer.pos[1] == '\r' &&
			buffer.pos[2] == '\n')
		{
			return 3;
		}
		if (buffer.pos[1] == '\r' ||
			buffer.pos[1] == '\n')
		{
			return 2;
		}
	}
	return 0;
}

//------------------------------------------------------------------------------
int CppLexer::ReadPPHeaderName(const Range& buffer)
{
	// エスケープは無い
	bool notFoundEndToken;
	int len = ReadEnclosingTokenHelper(buffer, IsStringLiteralStartInIncludeDirective, IsStringLiteralEndIncludeDirective, nullptr, &notFoundEndToken);
	if (len > 0)
	{
		AddToken(TokenGroup::StringLiteral, buffer.pos, buffer.pos + len, TT_HeaderName);
	}
	return len;
}

//------------------------------------------------------------------------------
int CppLexer::IsStringLiteralStartInIncludeDirective(const Range& buffer)
{
	if (buffer.pos[0] == '"' || buffer.pos[0] == '<') {
		return 1;
	}
	return 0;
}

//------------------------------------------------------------------------------
int CppLexer::IsStringLiteralEndIncludeDirective(const Range& buffer)
{
	if (buffer.pos[0] == '"' || buffer.pos[0] == '>') {
		return 1;
	}
	return 0;
}

//------------------------------------------------------------------------------
// プリプロセッサトークン列(1行)の読み取り。
// 改行は含まない。
// #define AAA は 「#, define, space, AAA, NewLine」で出てくる。
// EscapeNewLine も同様。
//		#error AAA\
//		BBB
// は、「#, error, space, PPTokens, EscapeNewLine, PPTokens」
//------------------------------------------------------------------------------
int CppLexer::ReadPPTokens(const Range& buffer)
{
	Range r = buffer;
	while (r.pos < r.end)
	{
		// 改行まで読む
		int len = IsEscapeNewLine(r);
		if (len > 0) {
			break;		// 終了
		}
		len = IsNewLine(r);
		if (len > 0) {
			break;		// 終了
		}
		++r.pos;
	}

	if (r.pos - buffer.pos > 0)
	{
		AddToken(TokenGroup::TextTokens, buffer.pos, r.pos, TT_PPTokens);
	}
	return r.pos - buffer.pos;
}

} // namespace fl

