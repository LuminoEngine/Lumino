#pragma once
#include "Token.hpp"

namespace ln {
class DiagnosticsManager;

class TextDiagnostics
	: public RefObject
{
public:
	TextDiagnostics(DiagnosticsManager* baseDiag);
	void reportError(const StringRef& message) {}

private:
	Ref<DiagnosticsManager> m_baseDiag;
};

struct LexedToken
{
	TokenGroup group;
	int nativeTokenType;
	size_t length;
};

/** ASCII コードの文字種別 */
enum class AlphaNumTypeFlags
{
	MBC = 0x0000,		/**< マルチバイト文字に配置されるコード */
	Control = 0x0001,		/**< 制御文字 */
	OpChar = 0x0002,		/**< オペレーションコード */
	Number = 0x0004,		/**< 数字 */
	Alphabet = 0x0008,		/**< 通常文字 */
	HexAlpha = 0x0100,		/**< 16進数字としても使える文字 */
};
LN_FLAGS_OPERATORS(AlphaNumTypeFlags);

class AbstractLexer
	: public RefObject
{
public:
	struct Range
	{
		const char* pos;
		const char* end;

		Range()
		{
		}

		Range(const char* begin, const char* end_)
		{
			pos = begin;
			end = end_;
		}
	};

	void setDiag(TextDiagnostics* diag);
	TextDiagnostics* diag() const { return m_diag; }

	Ref<List<Token>> makeTokenList(const char* code, size_t length);

	LexedToken lex(const char* begin, const char* end);
	//const LexedToken& token() const { m_lastToken; }

	static const char* getTokenString(const Token& token, const char* code, size_t codeLength);

	static Flags<AlphaNumTypeFlags> GetAlphaNumType(int ch);
	static int IsNewLine(const Range& buffer);

protected:
	virtual int readToken(const Range& range) = 0;
	int ReadNewLine(const Range& buffer);
	int ReadMBSSequence(const Range& buffer);

	void AddToken(TokenGroup group, const char* bufBegin, const char* bufEnd, int tokenType = 0);

private:
	Ref<TextDiagnostics> m_diag;
	LexedToken m_lastResult;
};

} // namespace ln
