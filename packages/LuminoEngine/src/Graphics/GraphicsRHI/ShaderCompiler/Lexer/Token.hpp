#pragma once

namespace ln {

using SourceLocation = uint32_t;

/** 基本的なトークン種別 (言語間共通) */
enum class TokenGroup
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

class Token
{
public:
	Token(TokenGroup group, uint32_t kind, SourceLocation location, size_t length)
		: m_group(group)
		, m_kind(kind)
		, m_location(location)
		, m_length(length)
	{}

	TokenGroup group() const { return m_group; }
	uint32_t kind() const { return m_kind; }
	SourceLocation location() const { return m_location; }
	size_t length() const { return m_length; }

private:
	TokenGroup m_group;
	uint32_t m_kind;
	SourceLocation m_location;
	size_t m_length;
};

} // namespace ln
