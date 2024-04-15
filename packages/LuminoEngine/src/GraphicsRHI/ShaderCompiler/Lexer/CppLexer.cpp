
#include "Internal.hpp"
#include "CppLexer.hpp"

namespace ln {

int CppLexer::readToken(const Range& buffer)
{
	int len;

	// 空白並び
	len = ReadSpaceSequence(buffer);
	if (len > 0) { return len; }
	// プリプロセッサトークン列
	//len = ReadPPTokens(buffer);
	//if (len > 0) { return len; }
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


	/*
	* 文字列リテラルは識別子の前に解析する。これは、L 等のプレフィックス対応のため。
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

int CppLexer::IsSpaceChar(const Range& r)
{
	if (r.pos[0] == ' ' || r.pos[0] == '\t' || r.pos[0] == '\f' || r.pos[0] == '\v')
	{
		return 1;
	}
	return 0;
}

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
		{ ("asm"),					3,	TT_CppKW_asm },
		{ ("auto"),					4,	TT_CppKW_auto },
		{ ("bool"),					4,	TT_CppKW_bool },
		{ ("break"),					5,	TT_CppKW_break },
		{ ("case"),					4,	TT_CppKW_case },
		{ ("catch"),					5,	TT_CppKW_catch },
		{ ("char"),					4,	TT_CppKW_char },
		{ ("class"),					5,	TT_CppKW_class },
		{ ("const"),					5,	TT_CppKW_const },
		{ ("const_cast"),			10,	TT_CppKW_const_cast },
		{ ("continue"),				8,	TT_CppKW_continue },
		{ ("default"),				7,	TT_CppKW_default },
		{ ("delete"),				6,	TT_CppKW_delete },
		{ ("do"),					2,	TT_CppKW_do },
		{ ("double"),				6,	TT_CppKW_double },
		{ ("dynamic_cast"),			12,	TT_CppKW_dynamic_cast },
		{ ("else"),					4,	TT_CppKW_else },
		{ ("enum"),					4,	TT_CppKW_enum },
		{ ("explicit"),				8,	TT_CppKW_explicit },
		{ ("export"),				6,	TT_CppKW_export },
		{ ("extern"),				6,	TT_CppKW_extern },
		{ ("false"),					5,	TT_CppKW_false },
		{ ("float"),					5,	TT_CppKW_float },
		{ ("for"),					3,	TT_CppKW_for },
		{ ("friend"),				6,	TT_CppKW_friend },
		{ ("goto"),					4,	TT_CppKW_goto },
		{ ("if"),					2,	TT_CppKW_if },
		{ ("inline"),				6,	TT_CppKW_inline },
		{ ("int"),					3,	TT_CppKW_int },
		{ ("long"),					4,	TT_CppKW_long },
		{ ("mutable"),				7,	TT_CppKW_mutable },
		{ ("namespace"),				9,	TT_CppKW_namespace },
		{ ("new"),					3,	TT_CppKW_new },
		{ ("operator"),				8,	TT_CppKW_operator },
		{ ("private"),				7,	TT_CppKW_private },
		{ ("protected"),				9,	TT_CppKW_protected },
		{ ("public"),				6,	TT_CppKW_public },
		{ ("register"),				8,	TT_CppKW_register },
		{ ("reinterpret_cast"),		16,	TT_CppKW_reinterpret_cast },
		{ ("return"),				6,	TT_CppKW_return },
		{ ("short"),					5,	TT_CppKW_short },
		{ ("signed"),				6,	TT_CppKW_signed },
		{ ("sizeof"),				6,	TT_CppKW_sizeof },
		{ ("static"),				6,	TT_CppKW_static },
		{ ("static_cast"),			11,	TT_CppKW_static_cast },
		{ ("struct"),				6,	TT_CppKW_struct },
		{ ("switch"),				6,	TT_CppKW_switch },
		{ ("template"),				8,	TT_CppKW_template },
		{ ("this"),					4,	TT_CppKW_this },
		{ ("throw"),					5,	TT_CppKW_throw },
		{ ("true"),					4,	TT_CppKW_true },
		{ ("try"),					3,	TT_CppKW_try },
		{ ("typedef"),				7,	TT_CppKW_typedef },
		{ ("typeid"),				6,	TT_CppKW_typeid },
		{ ("typename"),				8,	TT_CppKW_typename },
		{ ("union"),					5,	TT_CppKW_union },
		{ ("unsigned"),				8,	TT_CppKW_unsigned },
		{ ("using"),					5,	TT_CppKW_using },
		{ ("virtual"),				7,	TT_CppKW_virtual },
		{ ("void"),					4,	TT_CppKW_void },
		{ ("volatile"),				8,	TT_CppKW_volatile },
		{ ("wchar_t"),				7,	TT_CppKW_wchar_t },
		{ ("while"),					5,	TT_CppKW_while },
	};

	const int len = LN_ARRAY_SIZE_OF(wordList);
	int keyLen = 0;
	int type = 0;
	for (int i = 0; i < len; ++i)
	{
		if (wordList[i].word[0] == buffer.pos[0] &&		// まずは先頭文字を調べて
			strncmp(wordList[i].word, buffer.pos, wordList[i].length) == 0)	// 先頭が一致したら残りを調べる
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
		Flags<AlphaNumTypeFlags> charType = GetAlphaNumType(r.pos[0]);

		// マルチバイトコードまたは制御文字または . 以外の演算子であれば終了
		if (charType.hasFlag(AlphaNumTypeFlags::MBC) ||
			charType.hasFlag(AlphaNumTypeFlags::Control) ||
			(charType.hasFlag(AlphaNumTypeFlags::OpChar) && r.pos[0] != '.')) {
			break;	// 終了
		}

		// サフィックスのチェック (サフィックスは一番最後だが、いろいろなところに書ける。1f 1.0f .1f 1.0-e2f)
		if (charType.hasFlag(AlphaNumTypeFlags::Alphabet) /*|| charType.TestFlag(AlphaNumTypeFlags::HexAlpha)*/)
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
			if (bHexMode && charType.hasFlag(AlphaNumTypeFlags::HexAlpha)) {
				r.pos++;		// 16 進数なら HexAlpha を許可して継続
			}
			else if (charType.hasFlag(AlphaNumTypeFlags::Number)) {
				r.pos++;		// 普通の数値も OK
			}
			else if (charType.hasFlag(AlphaNumTypeFlags::OpChar) && r.pos[0] == '.') {
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
			if (bHexMode && charType.hasFlag(AlphaNumTypeFlags::HexAlpha)) {
				r.pos++;		// 16 進数なら HexAlpha を許可して継続
			}
			else if (charType.hasFlag(AlphaNumTypeFlags::Number)) {
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
			if (charType.hasFlag(AlphaNumTypeFlags::Number)) {
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
			diag()->reportError(_T("DiagnosticsCode::UnexpectedEOFInBlockComment"));
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
	if (GetAlphaNumType(buffer.pos[0]).hasFlag(AlphaNumTypeFlags::OpChar))
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
			{ ("##"),	2,	TT_CppOP_SharpSharp },
		{ ("#"),		1,	TT_CppOP_Sharp },
		{ ("->*"),	3,	TT_CppOP_ArrowAsterisk },
		{ ("->"),	2,	TT_CppOP_Arrow },
		{ (","),		1,	TT_CppOP_Comma },
		{ ("++"),	2,	TT_CppOP_Increment },
		{ ("--"),	2,	TT_CppOP_Decrement },
		{ ("&&"),	2,	TT_CppOP_LogicalAnd },
		{ ("||"),	2,	TT_CppOP_LogicalOr },
		{ ("<="),	2,	TT_CppOP_LessThenEqual },
		{ (">="),	2,	TT_CppOP_GreaterThenEqual },
		{ ("=="),	2,	TT_CppOP_CmpEqual },
		{ ("<<="),	3,	TT_CppOP_LeftShiftEqual },
		{ (">>="),	3,	TT_CppOP_RightShiftEqual },
		{ ("+="),	2,	TT_CppOP_PlusEqual },
		{ ("-="),	2,	TT_CppOP_MinusEqual },
		{ ("*="),	2,	TT_CppOP_MulEqual },
		{ ("/="),	2,	TT_CppOP_DivEqual },
		{ ("%="),	2,	TT_CppOP_ModEqual },
		{ ("&="),	2,	TT_CppOP_AndEqual },
		{ ("|="),	2,	TT_CppOP_OrEqual },
		{ ("!="),	2,	TT_CppOP_NotEqual },
		{ ("="),		1,	TT_CppOP_Equal },
		{ ("<<"),	2,	TT_CppOP_LeftShift },
		{ (">>"),	2,	TT_CppOP_RightShift },
		{ ("+"),		1,	TT_CppOP_Plus },
		{ ("-"),		1,	TT_CppOP_Minus },
		{ ("*"),		1,	TT_CppOP_Asterisk },
		{ ("/"),		1,	TT_CppOP_Slash },
		{ ("%"),		1,	TT_CppOP_Parseint },
		{ ("&"),		1,	TT_CppOP_Ampersand },
		{ ("|"),		1,	TT_CppOP_Pipe },
		{ ("~"),		1,	TT_CppOP_Tilde },
		{ ("^"),		1,	TT_CppOP_Caret },
		{ ("!"),		1,	TT_CppOP_Exclamation },
		{ ("..."),	3,	TT_CppOP_Ellipsis },
		{ (".*"),	2,	TT_CppOP_DotAsterisk },
		{ ("."),		1,	TT_CppOP_Dot },
		{ ("::"),	2,	TT_CppOP_DoubleColon },
		{ ("?"),		1,	TT_CppOP_Question },
		{ (":"),		1,	TT_CppOP_Colon },
		{ (";"),		1,	TT_CppOP_Semicolon },
		{ ("{"),		1,	TT_CppOP_LeftBrace },
		{ ("}"),		1,	TT_CppOP_RightBrace },
		{ ("["),		1,	TT_CppOP_LeftBracket },
		{ ("]"),		1,	TT_CppOP_RightBracket },
		{ ("("),		1,	TT_CppOP_LeftParen },
		{ (")"),		1,	TT_CppOP_RightParen },
		{ ("<"),		1,	TT_CppOP_LeftAngle },
		{ (">"),		1,	TT_CppOP_RightAngle },
		};
		const int count = LN_ARRAY_SIZE_OF(wordList);
		for (int i = 0; i < count; ++i)
		{
			if (wordList[i].word[0] == buffer.pos[0] &&		// まずは先頭文字を調べて
				strncmp(wordList[i].word, buffer.pos, wordList[i].length) == 0)	// 先頭が一致したら残りを調べる
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

} // namespace ln
