
#pragma once
#include <LuminoCore.h>

LN_NAMESPACE_BEGIN
namespace parser
{
typedef char TokenChar;
typedef GenericString<TokenChar> TokenString;
typedef GenericStringRef<TokenChar> TokenStringRef;
typedef GenericPathName<TokenChar> TokenPathName;

class Context;
class CompileOptions;
class UnitFile;
class IncludeFile;
class CompileUnitFile;
class TokenBuffer;
class TokenList;
typedef RefPtr<TokenList>	TokenListPtr;
typedef RefPtr<UnitFile>	UnitFilePtr;
typedef RefPtr<IncludeFile>	IncludeFilePtr;

class DiagnosticsItemSet;

class MacroDefine;

class Scope;
class ReferenceTracker;

typedef size_t DataLocation;

/** ASCII コードの文字種別 */
LN_ENUM_FLAGS(AlphaNumTypeFlags)
{
	MBC			= 0x0000,		/**< マルチバイト文字に配置されるコード */
	Control		= 0x0001,		/**< 制御文字 */
	OpChar		= 0x0002,		/**< オペレーションコード */
	Number		= 0x0004,		/**< 数字 */
	Alphabet	= 0x0008,		/**< 通常文字 */
	HexAlpha	= 0x0100,		/**< 16進数字としても使える文字 */
};
LN_ENUM_FLAGS_DECLARE(AlphaNumTypeFlags);

/** 基本的なトークン種別 (言語間共通) */
LN_ENUM(CommonTokenType)
{
	Unknown = 0,			/**< 共通種別としては分類されない */
	SpaceSequence,			/**< 空白並び (Cの EscapeNewLine も含む) */
	NewLine,				/**< 改行 ("\r" "\n" "\r\n" のいずれか。並びではない。"\r\n\r\n" は2つの NewLine トークンとなる) */
	Identifier,				/**< 識別子 */
	Keyword,				/**< キーワード */
	Operator,				/**< 演算子 */
	ArithmeticLiteral,		/**< 算術型リテラル (整数、小数。C言語の char も算術型) */
	StringLiteral,			/**< 文字列 */
	Comment,				/**< コメント */
	MbsSequence,			/**< マルチバイト文字並び */
	TextTokens,				/**< プリプロセッサなどで使われるメッセージ文字列 */
	Eof,					/**< バッファ EOF */
};
LN_ENUM_DECLARE(CommonTokenType);


enum class ResultState
{
	Success = 0,
	Error = 1,
};

} // namespace Parser
LN_NAMESPACE_END


