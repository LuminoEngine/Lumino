
/*
literal:
	integer-literal
	character-literal
	floating-literal
	string-literal
	boolean-literal
	pointer-literal
	user-defined-literal
*/
#pragma once
#include "../Lexer.h"

LN_NAMESPACE_BEGIN
namespace parser
{
	
/// (bison に種別として伝えるため、細かく定数分けする)[2015/12/16] する予定だったけどたぶんしない
enum CppTokenType
{
	TT_Cpp_Unknown = 0,

	/* 1～255 は ASCII 文字とする (bison で文字リテラル '*' 等が使えるようになる) */

	/* 以下は全て bison 定義ファイル内で %token としても定義する */

	TT_CppKW_asm = 256,
	TT_CppKW_auto,
	TT_CppKW_bool,
	TT_CppKW_break,
	TT_CppKW_case,
	TT_CppKW_catch,
	TT_CppKW_char,
	TT_CppKW_class,
	TT_CppKW_const,
	TT_CppKW_const_cast,
	TT_CppKW_continue,
	TT_CppKW_default,
	TT_CppKW_delete,
	TT_CppKW_do,
	TT_CppKW_double,
	TT_CppKW_dynamic_cast,
	TT_CppKW_else,
	TT_CppKW_enum,
	TT_CppKW_explicit,
	TT_CppKW_export,
	TT_CppKW_extern,
	TT_CppKW_false,
	TT_CppKW_float,
	TT_CppKW_for,
	TT_CppKW_friend,
	TT_CppKW_goto,
	TT_CppKW_if,
	TT_CppKW_inline,
	TT_CppKW_int,
	TT_CppKW_long,
	TT_CppKW_mutable,
	TT_CppKW_namespace,
	TT_CppKW_new,
	TT_CppKW_operator,
	TT_CppKW_private,
	TT_CppKW_protected,
	TT_CppKW_public,
	TT_CppKW_register,
	TT_CppKW_reinterpret_cast,
	TT_CppKW_return,
	TT_CppKW_short,
	TT_CppKW_signed,
	TT_CppKW_sizeof,
	TT_CppKW_static,
	TT_CppKW_static_cast,
	TT_CppKW_struct,
	TT_CppKW_switch,
	TT_CppKW_template,
	TT_CppKW_this,
	TT_CppKW_throw,
	TT_CppKW_true,
	TT_CppKW_try,
	TT_CppKW_typedef,
	TT_CppKW_typeid,
	TT_CppKW_typename,
	TT_CppKW_union,
	TT_CppKW_unsigned,
	TT_CppKW_using,
	TT_CppKW_virtual,
	TT_CppKW_void,
	TT_CppKW_volatile,
	TT_CppKW_wchar_t,
	TT_CppKW_while,

	TT_CppOP_SeparatorBegin,	///< 以降は演算子
	// operator
	// 解析時は基本的に文字数の大きい方からトークン分割しないと、例えば + と ++ を間違えることがある。
	// 例）x+++++y	→	x ++ ++ + y,
	TT_CppOP_SharpSharp,		// ## (concat)
	TT_CppOP_Sharp,				// # (Prepro/stringize)
	TT_CppOP_ArrowAsterisk,		// ->*
	TT_CppOP_Arrow,				// ->
	TT_CppOP_Comma,				// ,
	TT_CppOP_Increment,			// ++
	TT_CppOP_Decrement,			// --
	TT_CppOP_LogicalAnd,		// &&
	TT_CppOP_LogicalOr,			// ||
	TT_CppOP_LessThenEqual,		// <=
	TT_CppOP_GreaterThenEqual,	// >=
	TT_CppOP_CmpEqual,			// ==
	TT_CppOP_LeftShiftEqual,	// <<=
	TT_CppOP_RightShiftEqual,	// >>=
	TT_CppOP_PlusEqual,			// +=
	TT_CppOP_MinusEqual,		// -=
	TT_CppOP_MulEqual,			// *=
	TT_CppOP_DivEqual,			// /=
	TT_CppOP_ModEqual,			// %=
	TT_CppOP_AndEqual,			// &=
	TT_CppOP_OrEqual,			// |=
	TT_CppOP_NotEqual,			// !=
	TT_CppOP_Equal,				// =
	TT_CppOP_LeftShift,			// <<
	TT_CppOP_RightShift,		// >>
	TT_CppOP_Plus,				// +
	TT_CppOP_Minus,				// -
	TT_CppOP_Asterisk,			// *
	TT_CppOP_Slash,				// /
	TT_CppOP_Parseint,			// %
	TT_CppOP_Ampersand,			// &
	TT_CppOP_Pipe,				// |
	TT_CppOP_Tilde,				// ~
	TT_CppOP_Caret,				// ^
	TT_CppOP_Exclamation,		// !
	TT_CppOP_Ellipsis,			// ...
	TT_CppOP_DotAsterisk,		// .*
	TT_CppOP_Dot,				// .
	TT_CppOP_DoubleColon,		// ::
	TT_CppOP_Question,			// ?
	TT_CppOP_Colon,				// :
	TT_CppOP_Semicolon,			// ;
	TT_CppOP_LeftBrace,			// {
	TT_CppOP_RightBrace,		// }
	TT_CppOP_LeftBracket,		// [
	TT_CppOP_RightBracket,		// ]
	TT_CppOP_LeftParen,			// (
	TT_CppOP_RightParen,		// )
	TT_CppOP_LeftAngle,			// <
	TT_CppOP_RightAngle,		// >

	TT_CppOP_SeparatorEnd,			///< 以降は演算子

	//<: :> <% %> %: %:%: ?= 
	//and and_eq bitand bitor compl not not_eq
	//or or_eq xor xor_eq

	// new delete はキーワードとして扱う

	/* ちなみに C++11 の演算子は以下の通り。トリグラフを含まなければ C++ と同じ
	{ } [ ] # ## ( )
	<: :> <% %> %: %:%: ; : ...
	new delete ? :: . .*
	+ - * / % ^ & | ~
	! = < > += -= *= /= %=
	^= &= |= << >> >>= <<= == !=
	<= >= && || ++ -- , ->* ->
	and and_eq bitand bitor compl not not_eq
	or or_eq xor xor_eq
	*/


	TT_NumericLitaralType_Char,
	TT_NumericLitaralType_WideChar,

	TT_NumericLitaralType_Int32,
	TT_NumericLitaralType_UInt32,
	TT_NumericLitaralType_Int64,
	TT_NumericLitaralType_UInt64,
	TT_NumericLitaralType_Float,	/**< 32bit */
	TT_NumericLitaralType_Double,	/**< 64bit */

	TT_NumericLitaralType_AsciiString,
	TT_NumericLitaralType_WideString,

	TT_HeaderName,


	TT_EscapeNewLine,
	TT_PPTokens,
};


/**
	@brief
	@note	
		[2016/2/22 2]
		やっぱり #include は特別扱いしたほうが色々と都合がいい。
		インクルードファイル名のトークンは header-name として取り出す。
	
		[2016/2/22]
		#include や #line 等は、Lexer では pp-tokens として解析する。
		構文的にはこう。
			:: # include pp-tokens new-line
			:: # line pp-tokens new-line

		で、Preprocessor クラスで、pp-tokens をトークン分割する。
		ちなみに pp-tokens の構文は次の通り。
			pp-tokens:
				preprocessing-token
				pp-tokens preprocessing-token
			preprocessing-token:
				header-name
				identifier
				pp-number
				character-literal
				user-defined-character-literal
				string-literal
				user-defined-string-literal
				preprocessing-op-or-punc
				each non-white-space character that cannot be one of the above
		構文的には #include <a.h> <b.h> とか書ける点に注意。

		× #include は特別扱いする。<> で囲まれたファイルパスは文字列リテラル扱いする。
*/
class CppLexer
	: public Lexer
{
public:
	typedef int(TokenCheckCallback)(const Range& buffer);

	CppLexer();
	virtual ~CppLexer();

	virtual int ReadToken(const Range& buffer, TokenList* list) override;
	virtual void PollingToken(const Token& newToken) override;
	virtual void OnStart();


	static int IsSpaceChar(const Range& r);
	int ReadSpaceSequence(const Range& buffer, ReadResult* outResult);


	static int IsKeyword(const Range& buffer, int* langTokenType);
	int ReadKeyword(const Range& buffer, ReadResult* outResult);

	static int ReadEnclosingTokenHelper(const Range& buffer, TokenCheckCallback start, TokenCheckCallback end, const TokenChar* chars, bool* outNotFoundEndToken);

	int ReadCharLiteral(const Range& buffer, ReadResult* outResult);
	static int IsCharLiteralStart(const Range& buffer);
	static int IsCharLiteralEnd(const Range& buffer);
	//static int IsAnyChar(const Range& buffer, const TokenChar* chars, int count);

	int ReadStringLiteral(const Range& buffer, ReadResult* outResult);
	static int IsStringLiteralStart(const Range& buffer);
	static int IsStringLiteralEnd(const Range& buffer);

	int ReadIdentifier(const Range& buffer, ReadResult* outResult);
	static int IsIdentifierStart(const Range& buffer);
	static int IsIdentifierLetter(const Range& buffer);

	int ReadNumericLiteral(const Range& buffer, ReadResult* outResult);
	static int IsHexLiteralStart(const Range& buffer);
	static int IsIntegerSuffix(const Range& buffer, int* outLiteralType);
	static int IsRealSuffix(const Range& buffer, int* outLiteralType);
	static int IsExponentStart(const Range& buffer);

	int ReadBlockComment(const Range& buffer, ReadResult* outResult);
	static int IsBlockCommentStart(const Range& buffer);
	static int IsBlockCommentEnd(const Range& buffer);

	int ReadLineComment(const Range& buffer, ReadResult* outResult);
	static int IsLineCommentStart(const Range& buffer);

	int ReadOperator(const Range& buffer, ReadResult* outResult);
	static int IsOperator(const Range& buffer, int* langTokenType);

	int ReadEscapeNewLine(const Range& buffer, ReadResult* outResult);
	static int IsEscapeNewLine(const Range& buffer);

	int ReadPPHeaderName(const Range& buffer, ReadResult* outResult);
	static int IsStringLiteralStartInIncludeDirective(const Range& buffer);
	static int IsStringLiteralEndIncludeDirective(const Range& buffer);

	int ReadPPTokens(const Range& buffer, ReadResult* outResult);

private:
	// pp-tokens として取り出すプリプロセッサディレクティブ
	enum class PPDirectiveSeq
	{
		Idle = 0,		// 何もしていない
		LineHead,		// 行頭である。または初期状態
		FoundSharp,		// "#" を見つけた

		//FoundInclude,	// "include" を見つけた
		ReadingPPHeaderName,
		ReadingPPTokens,
	};

	PPDirectiveSeq	m_seqPPDirective;
};

} // namespace Parser
LN_NAMESPACE_END

