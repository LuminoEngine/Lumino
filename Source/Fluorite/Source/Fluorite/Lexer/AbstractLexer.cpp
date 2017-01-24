
#include "../Internal.h"
#include <Lumino/Text/UnicodeUtils.h>
#include <Fluorite/AnalyzerContext.h>
#include "AbstractLexer.h"

namespace fl {
	
static int g_alphaNumTypeTable[256] =
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

//==============================================================================
// AbstractLexer
//==============================================================================

//------------------------------------------------------------------------------
AbstractLexer::AbstractLexer()
	: m_inputBuffer(nullptr)
	//, m_tokenList(nullptr)
	, m_diag(nullptr)
	, m_currentLineNumber(0)
	, m_currentColumn(0)
{
}

//------------------------------------------------------------------------------
AbstractLexer::~AbstractLexer()
{
}

//------------------------------------------------------------------------------
ResultState AbstractLexer::Tokenize(InputFile* file)
{
	assert(file != nullptr);
	m_inputFile = file;
	return Tokenize(file->GetCodeBuffer(), file->GetTokenListInternal(), file->GetDiag());
}

//------------------------------------------------------------------------------
ResultState AbstractLexer::Tokenize(const ByteBuffer* buffer, TokenList* outTokenList, DiagnosticsItemSet* diag)
{
	assert(buffer != nullptr);
	assert(outTokenList != nullptr);
	assert(diag != nullptr);

	m_inputBuffer = buffer;
	//m_tokenList = outTokenList;
	m_diag = diag;
	m_currentLineNumber = 1;
	m_currentColumn = 1;

	//// 最悪のパターンで容量確保
	//m_tokenList->Clear();
	//m_tokenList->Reserve(buffer->GetSize());

	OnStart();

	// 解析メイン
	Range r;
	r.pos = (const char*)buffer->GetConstData();
	r.end = r.pos + (buffer->GetSize() / sizeof(char));
	while (r.pos < r.end)
	{
		int len = ReadToken(r);
		if (len == 0) {
			break;
		}

		Token* lastToken = GetLastToken();
		PollingToken(lastToken);

		// トークンの開始位置をセット
		lastToken->SetFirstLineNumber(m_currentLineNumber);
		lastToken->SetFirstColumn(m_currentColumn);

		// 行番号、列番号の計算。トークン内に改行があるかどうか、1文字ずつ確認する。
		// また、次の解析開始位置まで r.pos を進める。
		const char* tokenEnd = r.pos + len;
		bool isNLLast = false;
		while (r.pos < tokenEnd)
		{
			int nlLen = IsNewLine(r);
			if (nlLen > 0)
			{
				isNLLast = true;
				r.pos += nlLen;

				lastToken->SetLastLineNumber(m_currentLineNumber);
				lastToken->SetLastColumn(m_currentColumn + nlLen - 1);

				m_currentLineNumber++;
				m_currentColumn = 1;
			}
			else
			{
				r.pos++;
				m_currentColumn++;
				isNLLast = false;
			}
		}
		if (!isNLLast)
		{
			lastToken->SetLastLineNumber(m_currentLineNumber);
			lastToken->SetLastColumn(m_currentColumn - 1);
		}
	}

	// 最後に EOF を入れておく
	AddToken(TokenGroup::Eof, 0, 0);

	return ResultState::Success;
}

//------------------------------------------------------------------------------
void AbstractLexer::PollingToken(Token* newToken)
{
}

//------------------------------------------------------------------------------
void AbstractLexer::AddToken(TokenGroup group, const char* bufBegin, const char* bufEnd, int tokenType)
{
	Token* token = m_inputFile->CreateToken();

	const char* begin = (const char*)m_inputBuffer->GetConstData();
	*token = Token(m_inputFile, group, bufBegin - begin, bufEnd - begin, tokenType);
}

//------------------------------------------------------------------------------
Token* AbstractLexer::GetLastToken()
{
	return m_inputFile->GetTokenListInternal()->GetLast();
}

//------------------------------------------------------------------------------
bool AbstractLexer::EqualsString(Token* token, const char* str, int length) const
{
	// TODO: 普通に Token::EqualXXXX使っていいと思う
	if (token->GetLength() != length) return false;
	const char* begin = (const char*)m_inputBuffer->GetConstData();
	return StringTraits::StrNCmp(begin + token->GetBeginLoc(), str, length) == 0;	// TODO: Case
}

//------------------------------------------------------------------------------
AlphaNumTypeFlags AbstractLexer::GetAlphaNumType(int ch)
{
	uint8_t uc = (uint8_t)ch;
	if (uc > 255) {
		return AlphaNumTypeFlags::MBC;	// wchar_t 型の多バイトコード
	}
	return (AlphaNumTypeFlags::enum_type)g_alphaNumTypeTable[uc];
}

//------------------------------------------------------------------------------
int AbstractLexer::ReadNewLine(const Range& buffer)
{
	int len = IsNewLine(buffer);
	if (len > 0) {
		AddToken(TokenGroup::NewLine, buffer.pos, buffer.pos + len);
		return len;
	}
	return 0;
}

//------------------------------------------------------------------------------
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

//------------------------------------------------------------------------------
int AbstractLexer::ReadMBSSequence(const Range& buffer)
{
	Range r = buffer;
	while (r.pos < r.end)
	{
		int extraCount = 0;
		UnicodeUtils::CheckUTF8TrailingBytes((const UnicodeUtils::UTF8*)r.pos, (const UnicodeUtils::UTF8*)r.end, false, &extraCount);

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

//------------------------------------------------------------------------------
void AbstractLexer::OnStart()
{
}

} // namespace fl
