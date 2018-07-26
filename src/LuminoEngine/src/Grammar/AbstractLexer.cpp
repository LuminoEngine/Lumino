
#include "Internal.hpp"
#include "../../LuminoCore/src/Text/UnicodeUtils.hpp"
#include <Lumino/Engine/Diagnostics.hpp>
#include "AbstractLexer.hpp"

namespace ln {

TextDiagnostics::TextDiagnostics(DiagnosticsManager* baseDiag)
	: m_baseDiag(baseDiag)
{
}

void AbstractLexer::setDiag(TextDiagnostics* diag)
{
	m_diag = diag;
}

Ref<List<Token>> AbstractLexer::makeTokenList(const char* code, size_t length)
{
	auto tokenList = makeRef<List<Token>>();

	Range r;
	r.pos = code;
	r.end = code + length;
	while (r.pos < r.end)
	{
		int len = readToken(r);
		if (len == 0) {
			break;
		}

		tokenList->add(Token(m_lastResult.group, m_lastResult.nativeTokenType, r.pos - code, m_lastResult.length));

		r.pos += len;
	}

	return tokenList;
}

//LexedToken AbstractLexer::lex(const char* begin, const char* end)
//{
//	return readToken(r);
//}

const char* AbstractLexer::getTokenString(const Token& token, const char* code, size_t codeLength)
{
	const char* head = code + token.location();
	if (LN_REQUIRE(head < code + codeLength)) return nullptr;
	return head;
}

Flags<AlphaNumTypeFlags> AbstractLexer::GetAlphaNumType(int ch)
{
	static int s_alphaNumTypeTable[256] =
	{
		0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001,
		0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001,
		0x0001, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002,
		0x0004, 0x0004, 0x0004, 0x0004, 0x0004, 0x0004, 0x0004, 0x0004, 0x0004, 0x0004, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002,
		0x0002, 0x0108, 0x0108, 0x0108, 0x0108, 0x0108, 0x0108, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008,
		0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002,
		0x0002, 0x0108, 0x0108, 0x0108, 0x0108, 0x0108, 0x0108, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008,
		0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0002, 0x0002, 0x0002, 0x0002, 0x0001,
		0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
		0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
		0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
		0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
		0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
		0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
		0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
		0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	};
	uint8_t uc = (uint8_t)ch;
	if (uc > 255) {
		return AlphaNumTypeFlags::MBC;	// wchar_t 型の多バイトコード
	}
	return (AlphaNumTypeFlags)s_alphaNumTypeTable[uc];
}

int AbstractLexer::IsNewLine(const Range& buffer)
{
	if (buffer.pos[0] == '\r' &&
		buffer.pos[1] == '\n')
	{
		return 2;
	}
	if (buffer.pos[0] == '\r' ||
		buffer.pos[0] == '\n')
	{
		return 1;
	}
	return 0;
}

int AbstractLexer::ReadNewLine(const Range& buffer)
{
	int len = IsNewLine(buffer);
	if (len > 0) {
		AddToken(TokenGroup::NewLine, buffer.pos, buffer.pos + len);
		return len;
	}
	return 0;
}

int AbstractLexer::ReadMBSSequence(const Range& buffer)
{
	Range r = buffer;
	while (r.pos < r.end)
	{
		int extraCount = 0;
		UnicodeUtils::checkUTF8TrailingBytes((const UTF8*)r.pos, (const UTF8*)r.end, false, &extraCount);

		// 追加バイトが無い = シングルバイト文字だった場合は終了
		if (extraCount == 0) {
			break;
		}

		// 今回調べた文字と、追加文字分進める
		r.pos += (1 + extraCount);
	};

	int len = r.pos - buffer.pos;
	if (len > 0) {
		AddToken(TokenGroup::MbsSequence, buffer.pos, r.pos);
	}
	return len;
}

void AbstractLexer::AddToken(TokenGroup group, const char* bufBegin, const char* bufEnd, int tokenType)
{
	m_lastResult.group = group;
	m_lastResult.length = bufEnd - bufBegin;
	m_lastResult.nativeTokenType = tokenType;
}

} // namespace ln
