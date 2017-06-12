
#include "../Internal.h"
#include <Lumino/Base/String.h>
#include <Lumino/IO/StringReader.h>
#include <Lumino/Json/JsonReader.h>

LN_NAMESPACE_BEGIN
namespace tr {

//==============================================================================
// JsonReader
//==============================================================================
//------------------------------------------------------------------------------
JsonReader::JsonReader(JsonHandler* handler)
	: m_error()
	, m_handler(handler)
	, m_reader(NULL)
	, m_tmpStream()
	, m_currentCharCount(0)
{
}

//------------------------------------------------------------------------------
JsonReader::~JsonReader()
{
}

//------------------------------------------------------------------------------
void JsonReader::Parse(const String& text)
{
	StringReader textReader(text);
	Parse(&textReader);
}

//------------------------------------------------------------------------------
void JsonReader::Parse(const TCHAR* text, int len)
{
	StringReader textReader(String(text, len));
	Parse(&textReader);
}

//------------------------------------------------------------------------------
void JsonReader::Parse(TextReader* textReader)
{
	if (LN_CHECK_ARG(textReader != nullptr)) return;

	m_reader = textReader;

	// 一時バッファ。もし足りなければ拡張される
	m_tmpStream.initialize(512);

	// バッファ先頭の空白を読み飛ばす
	if (!SkipWhitespace())
	{
		// Error: バッファが空だった
		m_error.SetError(JsonParseError::DocumentEmpty, m_currentCharCount);
		return;
	}

	// ルート要素の解析
	if (!ParseValue()) {
		return;
	}

	// バッファ終端の空白を読み飛ばす
	if (SkipWhitespace())
	{
		// Error: 複数のルート要素が見つかった
		m_error.SetError(JsonParseError::DocumentRootNotSingular, m_currentCharCount);
		return;
	}
}

//------------------------------------------------------------------------------
bool JsonReader::SkipWhitespace()
{
	while (m_reader->Peek() == ' ' || m_reader->Peek() == '\n' || m_reader->Peek() == '\r' || m_reader->Peek() == '\t') {
		m_reader->Read();
	}
	return !m_reader->IsEOF();
}

//------------------------------------------------------------------------------
bool JsonReader::ParseValue()
{
	switch (m_reader->Peek())
	{
		case 'n': return ParseNull();			// null かもしれない
		case 't': return ParseTrue();			// true かもしれない
		case 'f': return ParseFalse();			// false かもしれない
		case '"': return ParseString(false);	// 文字列かもしれない
		case '[': return ParseArray();			// 配列かもしれない
		case '{': return ParseObject();			// オブジェクトかもしれない
		default: return ParseNumber();			// 数値かもしれない
	}
}

//------------------------------------------------------------------------------
bool JsonReader::ParseNull()
{
	m_reader->Read();	// skip 'n'
	if (m_reader->Read() == 'u' &&
		m_reader->Read() == 'l' &&
		m_reader->Read() == 'l')
	{
		if (!m_handler->OnNull())
		{
			// 中断
			m_error.SetError(JsonParseError::Termination, m_currentCharCount);
			return false;
		}
	}
	else
	{
		// Error: "null" ではなかった
		m_error.SetError(JsonParseError::ValueInvalid, m_currentCharCount);
		return false;
	}
	return true;
}

//------------------------------------------------------------------------------
bool JsonReader::ParseTrue()
{
	m_reader->Read();	// skip 't'
	if (m_reader->Read() == 'r' &&
		m_reader->Read() == 'u' &&
		m_reader->Read() == 'e')
	{
		if (!m_handler->OnBool(true))
		{
			// 中断
			m_error.SetError(JsonParseError::Termination, m_currentCharCount);
			return false;
		}
	}
	else
	{
		// Error: "true" ではなかった
		m_error.SetError(JsonParseError::ValueInvalid, m_currentCharCount);
		return false;
	}
	return true;
}

//------------------------------------------------------------------------------
bool JsonReader::ParseFalse()
{
	m_reader->Read();	// skip 'f'
	if (m_reader->Read() == 'a' &&
		m_reader->Read() == 'l' &&
		m_reader->Read() == 's' &&
		m_reader->Read() == 'e')
	{
		if (!m_handler->OnBool(false))
		{
			// 中断
			m_error.SetError(JsonParseError::Termination, m_currentCharCount);
			return false;
		}
	}
	else
	{
		// Error: "false" ではなかった
		m_error.SetError(JsonParseError::ValueInvalid, m_currentCharCount);
		return false;
	}
	return true;
}

//------------------------------------------------------------------------------
bool JsonReader::ParseNumber()
{
	// 数値への変換には strtod を使用する。そのため、まずは数値扱いできる文字を全て読み取る
	m_tmpStream.Seek(0, SeekOrigin_Begin);
	int len = 0;
	TCHAR ch;
	while (true)
	{
		ch = m_reader->Peek();	// 読むだけ。ポインタは進めない
		if (ch == '.' ||
			('0' <= ch && ch <= '9') ||
			(ch == 'e' || ch == 'E') ||
			(ch == '+' || ch == '-'))
		{
			m_tmpStream.Write(&ch, sizeof(TCHAR));
			++len;
			m_reader->Read();	// ここで1つ進める
		}
		else {
			break;				// 一致しなければポインタは進めない
		}
	}
	if (len == 0)
	{
		// Error: 数値っぽい文字が見つからなかった
		m_error.SetError(JsonParseError::NumberInvalid, m_currentCharCount);
		return false;
	}
	ch = '\0';
	m_tmpStream.Write(&ch, sizeof(TCHAR));	// 終端 \0

	// double へ変換する
	TCHAR* str = (TCHAR*)m_tmpStream.GetBuffer();
	const TCHAR* endptr = NULL;
	NumberConversionResult result;
	double value = StringTraits::ToDouble(str, len, &endptr, &result);
	if ((endptr - str) != len)	// 正常に変換できていれば、読み取った文字数が全て消費されるはず
	{
		// Error: 構文が正しくない
		m_error.SetError(JsonParseError::NumberInvalid, m_currentCharCount);
		return false;
	}
	if (result == NumberConversionResult::Overflow) {
		// Error: オーバーフローが発生した
		m_error.SetError(JsonParseError::NumberOverflow, m_currentCharCount);
		return false;
	}

	// Handler に通知する
	if (!m_handler->OnDouble(value))
	{
		// 中断
		m_error.SetError(JsonParseError::Termination, m_currentCharCount);
		return false;
	}
	return true;
}


//------------------------------------------------------------------------------
bool JsonReader::ParseString(bool isKey)
{
	// http://json.org/json-ja.html
	static const TCHAR escapeTable[256] =
	{
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
		0, 0, '\"', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '/',
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '\\', 0, 0, 0,
		0, 0, '\b', 0, 0, 0, '\f', 0, 0, 0, 0, 0, 0, 0, '\n', 0,
		0, 0, '\r', 0, '\t', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	};

	// 1つずつ読んだ文字を格納していく一時バッファ。シーク位置を先頭に戻しておく
	m_tmpStream.Seek(0, SeekOrigin_Begin);

	m_reader->Read();	// skip '"'
	while (true)
	{
		TCHAR c = m_reader->Peek();

		// エスケープシーケンス
		if (c == '\\')
		{
			m_reader->Read();	// skip '\'
			TCHAR esc = m_reader->Read();
			// 基本的なエスケープ
			if (unsigned(esc) < 256 && escapeTable[(unsigned char)esc])
			{
				m_tmpStream.Write(&escapeTable[(unsigned char)esc], sizeof(TCHAR));
			}
			// Unicode エスケープ
			else if (esc == 'u')
			{
				// 未実装
				m_error.SetError(JsonParseError::StringEscapeInvalid, m_currentCharCount);
				return false;
			}
			else
			{
				// Error: 無効なエスケープ
				m_error.SetError(JsonParseError::StringEscapeInvalid, m_currentCharCount);
				return false;
			}
		}
		// 文字列終端
		else if (c == '"')
		{
			m_reader->Read();	// skip '"'
			break;
		}
		// 文字列の途中でバッファが切れた
		else if (m_reader->IsEOF() || c == '\0')
		{
			// Error: " が一致しなかった
			m_error.SetError(JsonParseError::StringMissQuotationMark, m_currentCharCount);
			return false;
		}
		// 0x20 未満の制御文字は使えない
		else if ((unsigned)c < 0x20) {
			// RFC 4627: unescaped = %x20-21 / %x23-5B / %x5D-10FFFF
			m_error.SetError(JsonParseError::StringEscapeInvalid, m_currentCharCount);
			return false;
		}
		// 普通の文字
		else
		{
			m_tmpStream.Write(&c, sizeof(TCHAR));
			m_reader->Read();
		}
	}

	// Handler に通知
	bool cont = false;
	if (isKey) {
		cont = m_handler->OnKey((TCHAR*)m_tmpStream.GetBuffer(), ((int)m_tmpStream.GetPosition()) / sizeof(TCHAR));
	}
	else {
		cont = m_handler->OnString((TCHAR*)m_tmpStream.GetBuffer(), ((int)m_tmpStream.GetPosition()) / sizeof(TCHAR));
	}
	if (!cont)
	{
		// 中断
		m_error.SetError(JsonParseError::Termination, m_currentCharCount);
		return false;
	}
	return true;
}

//------------------------------------------------------------------------------
bool JsonReader::ParseArray()
{
	m_reader->Read();  // skip '['

	// Handler に Array の開始を通知
	if (!m_handler->OnStartArray())
	{
		// 中断
		m_error.SetError(JsonParseError::Termination, m_currentCharCount);
		return false;
	}

	SkipWhitespace();
	if (m_reader->Peek() == ']')
	{
		// 空配列だった。Array の終了を通知する
		m_reader->Read(); 	// ']' の次を指しておく
		if (!m_handler->OnEndArray(0))
		{
			// 中断
			m_error.SetError(JsonParseError::Termination, m_currentCharCount);
			return false;
		}
		return true;
	}

	SkipWhitespace();
	size_t elementCount = 0;
	while (true)
	{
		if (!ParseValue()) {
			return false;	// エラーは処理済み
		}

		++elementCount;
		SkipWhitespace();

		switch (m_reader->Read())
		{
		case ',':
			SkipWhitespace();
			break;
		case ']':
			// 配列の終端
			if (!m_handler->OnEndArray(elementCount))
			{
				// 中断
				m_error.SetError(JsonParseError::Termination, m_currentCharCount);
				return false;
			}
			SkipWhitespace();
			return true;
		default:
			m_error.SetError(JsonParseError::ArrayMissCommaOrSquareBracket, m_currentCharCount);
			return false;
		}
	}

	// ここに来ることはない
}

//------------------------------------------------------------------------------
bool JsonReader::ParseObject()
{
	m_reader->Read();  // Skip '{'

	// オブジェクト定義の開始
	if (!m_handler->OnStartObject())
	{
		// 中断
		m_error.SetError(JsonParseError::Termination, m_currentCharCount);
		return false;
	}

	SkipWhitespace();
	if (m_reader->Peek() == '}')
	{
		// メンバが1つも無かった
		m_reader->Read();	// '}' の次を指しておく
		if (!m_handler->OnEndObject(0))
		{
			// 中断
			m_error.SetError(JsonParseError::Termination, m_currentCharCount);
			return false;
		}
		return true;
	}

	int memberCount = 0;
	while (true)
	{
		// 最初はメンバ名
		if (m_reader->Peek() != '"')
		{
			// Error: メンバ名の開始が見つからなかった
			m_error.SetError(JsonParseError::ObjectMissKeyStart, m_currentCharCount);
			return false;
		}
		if (!ParseString(true)) return false;
		SkipWhitespace();

		// 続いて ':'
		if (m_reader->Read() != ':')
		{
			// Error: ':' が見つからなかった
			m_error.SetError(JsonParseError::ObjectMissColon, m_currentCharCount);
			return false;
		}
		SkipWhitespace();

		// 最後に値
		if (!ParseValue()) return false;
		SkipWhitespace();

		++memberCount;

		switch (m_reader->Read())
		{
		case ',':
			SkipWhitespace();
			break;
		case '}':
			// オブジェクト定義終端
			if (!m_handler->OnEndObject(memberCount))
			{
				// 中断
				m_error.SetError(JsonParseError::Termination, m_currentCharCount);
				return false;
			}
			SkipWhitespace();
			return true;
		default:
			m_error.SetError(JsonParseError::ObjectMissCommaOrCurlyBracket, m_currentCharCount);
			return false;
		}
	}

	// ここには来ないはず
}


//==============================================================================
// JsonReader
//==============================================================================

//------------------------------------------------------------------------------
JsonReader2::JsonReader2()
	: m_reader()
{
}

//------------------------------------------------------------------------------
JsonReader2::JsonReader2(const String& text)
	: JsonReader2()
{
	RefPtr<StringReader> r(LN_NEW StringReader(text), false);
	m_reader.attach(LN_NEW detail::PositioningTextReader(r));
}

//------------------------------------------------------------------------------
JsonReader2::JsonReader2(TextReader* textReader)
	: JsonReader2()
{
	m_reader.attach(LN_NEW detail::PositioningTextReader(textReader));
}

//------------------------------------------------------------------------------
JsonReader2::~JsonReader2()
{
}

//------------------------------------------------------------------------------
bool JsonReader2::Read()
{
	return TryRead();
	// TODO: 例外
}

//------------------------------------------------------------------------------
bool JsonReader2::TryRead()
{
	m_textCache.clear();

	bool skip;
	do
	{
		skip = false;
		switch (m_currentState.state)
		{
			case State::Start:
			case State::Property:
			case State::ArrayStart:
			case State::Array:
				return ParseValue();
			case State::ObjectStart:
			case State::Object:
				return ParseObject();
				break;
			case State::PostValue:
			{
				bool r = ParsePostValue(&skip);
				if (!skip) return r;
			}
		}

	} while (skip);
	return false;
}

//------------------------------------------------------------------------------
JsonToken JsonReader2::GetTokenType() const
{
	return m_currentToken.type;
}

//------------------------------------------------------------------------------
const String& JsonReader2::GetValue() const
{
	return m_value;
}

//------------------------------------------------------------------------------
const String& JsonReader2::GetPropertyName() const
{
	return m_currentState.propertyName;
}

//------------------------------------------------------------------------------
bool JsonReader2::GetBoolValue() const
{
	if (LN_CHECK_STATE(m_currentToken.type == JsonToken::Boolean)) return false;
	return m_valueData.m_bool;
}

//------------------------------------------------------------------------------
int32_t JsonReader2::GetInt32Value() const
{
	if (LN_CHECK_STATE(m_currentToken.type == JsonToken::Int32)) return 0;
	return m_valueData.m_int32;
}

//------------------------------------------------------------------------------
int64_t JsonReader2::GetInt64Value() const
{
	if (LN_CHECK_STATE(m_currentToken.type == JsonToken::Int64)) return 0;
	return m_valueData.m_int64;
}

//------------------------------------------------------------------------------
float JsonReader2::GetFloatValue() const
{
	if (LN_CHECK_STATE(m_currentToken.type == JsonToken::Float)) return 0;
	return m_valueData.m_float;
}

//------------------------------------------------------------------------------
double JsonReader2::GetDoubleValue() const
{
	if (LN_CHECK_STATE(m_currentToken.type == JsonToken::Double)) return 0;
	return m_valueData.m_double;
}

//------------------------------------------------------------------------------
const JsonError2& JsonReader2::GetError() const
{
	return m_error;
}

void JsonReader2::ReadAsStartObject()
{
	if (!Read() || GetTokenType() != JsonToken::StartObject)
	{
		LN_THROW(0, InvalidFormatException);
	}
}

void JsonReader2::ReadAsEndObject()
{
	if (!Read() || GetTokenType() != JsonToken::EndObject)
	{
		LN_THROW(0, InvalidFormatException);
	}
}

void JsonReader2::ReadAsStartArray()
{
	if (!Read() || GetTokenType() != JsonToken::StartArray)
	{
		LN_THROW(0, InvalidFormatException);
	}
}

void JsonReader2::ReadAsEndArray()
{
	if (!Read() || GetTokenType() != JsonToken::EndArray)
	{
		LN_THROW(0, InvalidFormatException);
	}
}

bool JsonReader2::ReadAsBool()
{
	if (!Read() || GetTokenType() != JsonToken::Boolean)
	{
		LN_THROW(0, InvalidFormatException);
	}
	return GetValue()[0] == 't';
}

const String& JsonReader2::ReadAsPropertyName()
{
	if (!Read() || GetTokenType() != JsonToken::PropertyName)
	{
		LN_THROW(0, InvalidFormatException);
	}
	return GetValue();
}

const String& JsonReader2::ReadAsString()
{
	if (!Read() || GetTokenType() != JsonToken::String)
	{
		LN_THROW(0, InvalidFormatException);
	}
	return GetValue();
}

//------------------------------------------------------------------------------
bool JsonReader2::SkipWhitespace()
{
	while (m_reader->Peek() == ' ' || m_reader->Peek() == '\n' || m_reader->Peek() == '\r' || m_reader->Peek() == '\t') {
		m_reader->Read();
	}
	return !m_reader->IsEOF();	// スキップした後に EOF にたどり着いたら false
}

//------------------------------------------------------------------------------
bool JsonReader2::ParseValue()
{
	if (!SkipWhitespace()) return false;

	//while (true)
	{
		//if (m_reader->IsEOF())
		//{
		//	return false;
		//}

		TCHAR ch = m_reader->Peek();
		switch (ch)
		{
			//case ' ':
			//case '\t':
			//	m_reader->Read();	// 空白は消費するだけ
			//	break;
			case '{':
				SetToken(JsonToken::StartObject);
				m_reader->Read();
				return true;
			case '[':
				SetToken(JsonToken::StartArray);
				m_reader->Read();
				return true;
			case ']':	// 空配列
				SetToken(JsonToken::EndArray);
				m_reader->Read();
				return true;
			case '"':
				return ParseString(false);

			case 'n':
				return ParseNull();
			case 't':
				return ParseTrue();
			case 'f':
				return ParseFalse();
				//case 't': return ParseTrue();			// true かもしれない
				//case 'f': return ParseFalse();			// false かもしれない
				//case '"': return ParseString(false);	// 文字列かもしれない
				//case '[': return ParseArray();			// 配列かもしれない
				//case '{': return ParseObject();			// オブジェクトかもしれない
				//default: return ParseNumber();			// 数値かもしれない

			default:
				if (isdigit(ch) || ch == '-' || ch == '.')
				{
					return ParseNumber();
				}
				return false;	// TODO

			
		}
	}
}

//------------------------------------------------------------------------------
bool JsonReader2::ParseNull()
{
	m_reader->Read();	// skip 'n'
	if (m_reader->Read() == 'u' &&
		m_reader->Read() == 'l' &&
		m_reader->Read() == 'l')
	{
		return SetToken(JsonToken::Null);
	}
	else
	{
		// Error: "null" ではなかった
		SetError(JsonParseError2::ValueInvalid);
		return false;
	}
}

//------------------------------------------------------------------------------
bool JsonReader2::ParseTrue()
{
	m_reader->Read();	// skip 't'
	if (m_reader->Read() == 'r' &&
		m_reader->Read() == 'u' &&
		m_reader->Read() == 'e')
	{
		m_valueData.m_bool = true;
		return SetToken(JsonToken::Boolean, _T("true"), 4);
	}
	else
	{
		// Error: "true" ではなかった
		SetError(JsonParseError2::ValueInvalid);
		return false;
	}
}

//------------------------------------------------------------------------------
bool JsonReader2::ParseFalse()
{
	m_reader->Read();	// skip 'f'
	if (m_reader->Read() == 'a' &&
		m_reader->Read() == 'l' &&
		m_reader->Read() == 's' &&
		m_reader->Read() == 'e')
	{
		m_valueData.m_bool = false;
		return SetToken(JsonToken::Boolean, _T("false"), 5);
	}
	else
	{
		// Error: "false" ではなかった
		SetError(JsonParseError2::ValueInvalid);
		return false;
	}
}

//------------------------------------------------------------------------------
bool JsonReader2::ParseNumber()
{
	// 数値として使える文字を m_textCache に入れていく
	int len = 0;
	bool isDecimal = false;
	TCHAR ch;
	while (true)
	{
		ch = m_reader->Peek();	// 読むだけ。ポインタは進めない
		if (ch == '.' ||
			('0' <= ch && ch <= '9') ||
			(ch == 'e' || ch == 'E') ||
			(ch == '+' || ch == '-'))
		{
			m_textCache.Add(ch);
			++len;
			m_reader->Read();	// ここで1つ進める

			// 小数か？
			if (!isDecimal) isDecimal = (ch == '.');
		}
		else {
			break;				// 一致しなければポインタは進めない
		}
	}
	if (len == 0)
	{
		// Error: 数値っぽい文字が見つからなかった
		SetError(JsonParseError2::UnexpectedToken);
		return false;
	}

	if (isDecimal)
	{
		const TCHAR* str = &m_textCache[0];
		const TCHAR* endptr = nullptr;
		NumberConversionResult result;
		double value = StringTraits::ToDouble(str, len, &endptr, &result);

		if ((endptr - str) != len)	// 正常に変換できていれば、読み取った文字数が全て消費されるはず
		{
			// Error: 構文が正しくない
			SetError(JsonParseError2::InvalidNumber);
			return false;
		}
		if (result == NumberConversionResult::Overflow)
		{
			// Error: オーバーフローが発生した
			SetError(JsonParseError2::NumberOverflow);
			return false;
		}

		if (-FLT_MAX <= value && value <= FLT_MAX)
		{
			m_valueData.m_float = (float)value;
			return SetToken(JsonToken::Float, str, len);
		}
		else
		{
			m_valueData.m_double = value;
			return SetToken(JsonToken::Double, str, len);
		}
	}
	else
	{
		const TCHAR* str = &m_textCache[0];
		const TCHAR* endptr = nullptr;
		NumberConversionResult result;
		int64_t value = StringTraits::ToInt64(str, len, 0, &endptr, &result);

		if ((endptr - str) != len)	// 正常に変換できていれば、読み取った文字数が全て消費されるはず
		{
			// Error: 構文が正しくない
			SetError(JsonParseError2::InvalidNumber);
			return false;
		}
		if (result == NumberConversionResult::Overflow)
		{
			// Error: オーバーフローが発生した
			SetError(JsonParseError2::NumberOverflow);
			return false;
		}

		if (INT32_MIN <= value && value <= INT32_MAX)
		{
			m_valueData.m_int32 = (int32_t)value;
			return SetToken(JsonToken::Int32, str, len);
		}
		else
		{
			m_valueData.m_int64 = value;
			return SetToken(JsonToken::Int64, str, len);
		}
	}

#if 0

	// 数値への変換には strtod を使用する。そのため、まずは数値扱いできる文字を全て読み取る
	m_tmpStream.Seek(0, SeekOrigin_Begin);
	int len = 0;
	TCHAR ch;
	while (true)
	{
		ch = m_reader->Peek();	// 読むだけ。ポインタは進めない
		if (ch == '.' ||
			('0' <= ch && ch <= '9') ||
			(ch == 'e' || ch == 'E') ||
			(ch == '+' || ch == '-'))
		{
			m_tmpStream.Write(&ch, sizeof(TCHAR));
			++len;
			m_reader->Read();	// ここで1つ進める
		}
		else {
			break;				// 一致しなければポインタは進めない
		}
	}
	if (len == 0)
	{
		// Error: 数値っぽい文字が見つからなかった
		m_error.SetError(JsonParseError::NumberInvalid, m_currentCharCount);
		return false;
	}
	ch = '\0';
	m_tmpStream.Write(&ch, sizeof(TCHAR));	// 終端 \0

	// double へ変換する
	TCHAR* str = (TCHAR*)m_tmpStream.GetBuffer();
	const TCHAR* endptr = NULL;
	NumberConversionResult result;
	double value = StringTraits::ToDouble(str, len, &endptr, &result);
	if ((endptr - str) != len)	// 正常に変換できていれば、読み取った文字数が全て消費されるはず
	{
		// Error: 構文が正しくない
		m_error.SetError(JsonParseError::NumberInvalid, m_currentCharCount);
		return false;
	}
	if (result == NumberConversionResult::Overflow) {
		// Error: オーバーフローが発生した
		m_error.SetError(JsonParseError::NumberOverflow, m_currentCharCount);
		return false;
	}

	// Handler に通知する
	if (!m_handler->OnDouble(value))
	{
		// 中断
		m_error.SetError(JsonParseError::Termination, m_currentCharCount);
		return false;
	}
	return true;
#endif
}

//------------------------------------------------------------------------------
bool JsonReader2::ParseObject()
{
	if (!SkipWhitespace()) return false;

	switch (m_reader->Peek())
	{
	case '}':
		SetToken(JsonToken::EndObject);
		m_reader->Read();
		return true;
	default:
		return TryParsePropertyName();
	}
}

//------------------------------------------------------------------------------
bool JsonReader2::TryParsePropertyName()
{
	/*
		::member = string name-separator value
		パース時点ではプロパティ名の空文字を許している。
	*/
	if (m_reader->Peek() == '"')
	{
		if (!ParseString(true))
		{
			// Error: " から始まる文字列ではなかった
			// Invalid property identifier character:
			return false;
		}

		if (!SkipWhitespace()) {
			// Error: EOF した
			return false;
		}

		if (m_reader->Peek() == ':') {
			// Invalid character after parsing property name. Expected ':'
		}
		m_reader->Read();

		if (!m_textCache.IsEmpty()) {
			SetToken(JsonToken::PropertyName, &m_textCache[0], m_textCache.GetCount());
		}
		else {
			SetToken(JsonToken::PropertyName);
		}

		return true;
	}
	else
	{
		return false;
	}

}

//------------------------------------------------------------------------------
bool JsonReader2::ParseString(bool isKey)
{
	/*
		 string = quotation-mark *char quotation-mark

         char = unescaped /
                escape (
                    %x22 /          ; "    quotation mark  U+0022
                    %x5C /          ; \    reverse solidus U+005C
                    %x2F /          ; /    solidus         U+002F
                    %x62 /          ; b    backspace       U+0008
                    %x66 /          ; f    form feed       U+000C
                    %x6E /          ; n    line feed       U+000A
                    %x72 /          ; r    carriage return U+000D
                    %x74 /          ; t    tab             U+0009
                    %x75 4HEXDIG )  ; uXXXX                U+XXXX

         escape = %x5C              ; \

         quotation-mark = %x22      ; "

         unescaped = %x20-21 / %x23-5B / %x5D-10FFFF
	*/
	// http://json.org/json-ja.html
	static const TCHAR escapeTable[256] =
	{
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
		0, 0, '\"', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '/',
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '\\', 0, 0, 0,
		0, 0, '\b', 0, 0, 0, '\f', 0, 0, 0, 0, 0, 0, 0, '\n', 0,
		0, 0, '\r', 0, '\t', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	};

	m_reader->Read();	// skip '"'
	while (true)
	{
		TCHAR c = m_reader->Peek();

		// エスケープシーケンス
		if (c == '\\')
		{
			m_reader->Read();	// skip '\'
			TCHAR esc = m_reader->Read();
			// 基本的なエスケープ
			if (unsigned(esc) < 256 && escapeTable[(unsigned char)esc])
			{
				m_textCache.Add(escapeTable[(unsigned char)esc]);
			}
			// Unicode エスケープ
			else if (esc == 'u')
			{
				// TODO: 未実装
				LN_THROW(0, NotImplementedException);
				return false;
			}
			else
			{
				// 無効なエスケープ
				SetError(JsonParseError2::InvalidStringEscape);
				return false;
			}
		}
		// 文字列終端
		else if (c == '"')
		{
			m_reader->Read();	// skip '"'
			break;
		}
		// 文字列の途中でバッファが切れた
		else if (m_reader->IsEOF() || c == '\0')
		{
			// " が一致しなかった
			SetError(JsonParseError2::UnterminatedString);
			return false;
		}
		// 0x20 未満の制御文字は使えない
		else if ((unsigned)c < 0x20)
		{
			SetError(JsonParseError2::InvalidStringChar);
			return false;
		}
		// 普通の文字
		else
		{
			m_textCache.Add(c);
			m_reader->Read();
		}
	}

	if (isKey)
	{
	}
	else
	{
		if (!m_textCache.IsEmpty()) {
			SetToken(JsonToken::String, &m_textCache[0], m_textCache.GetCount());
		}
		else {
			SetToken(JsonToken::String);
		}
	}

	//// Handler に通知
	//bool cont = false;
	//if (isKey) {
	//	cont = m_handler->OnKey((TCHAR*)m_tmpStream.GetBuffer(), ((int)m_tmpStream.GetPosition()) / sizeof(TCHAR));
	//}
	//else {
	//	cont = m_handler->OnString((TCHAR*)m_tmpStream.GetBuffer(), ((int)m_tmpStream.GetPosition()) / sizeof(TCHAR));
	//}
	//if (!cont)
	//{
	//	// 中断
	//	m_error.SetError(JsonParseError::Termination, m_currentCharCount);
	//	return false;
	//}
	return true;
}

//------------------------------------------------------------------------------
bool JsonReader2::ParsePostValue(bool* outSkip)
{
	*outSkip = false;
	if (!SkipWhitespace()) return false;

	switch (m_reader->Peek())
	{
	case '}':
		SetToken(JsonToken::EndObject);
		m_reader->Read();
		return true;
	case ']':
		SetToken(JsonToken::EndArray);
		m_reader->Read();
		return true;
	case ',':
		*outSkip = true;	// 今回の , は Read() で返さず、次のトークンを読みに行く
		if (m_currentState.containerType == ContainerType::Object) {
			m_currentState.state = State::Object;
		}
		else if (m_currentState.containerType == ContainerType::Array) {
			m_currentState.state = State::Array;
		}
		m_reader->Read();
		return true;
	default:
		return false;	// Error
	}
}

//------------------------------------------------------------------------------
// 現在位置の状態を newToken にする
//------------------------------------------------------------------------------
bool JsonReader2::SetToken(JsonToken newToken, const TCHAR* value, int valueLen)
{
	m_currentToken.type = newToken;
	//m_currentToken.valuePos = valuePos;
	//m_currentToken.valueLen = valueLen;
	if (value != nullptr && valueLen > 0)
	{
		m_value = String(value, valueLen/*&m_textCache[0], m_textCache.GetCount()*/);
	}
	else
	{
		m_value = String::GetEmpty();
	}

	switch (m_currentToken.type)
	{
		//case State::Start:
		//	break;
		case JsonToken::StartObject:
			m_currentState.state = State::ObjectStart;
			PushState();
			m_currentState.containerType = ContainerType::Object;
			break;
		case JsonToken::EndObject:
			if (m_currentState.state == State::ObjectStart ||
				m_currentState.state == State::PostValue)
			{
				PopState();
				m_currentState.state = State::PostValue;
			}
			else
			{
				SetError(JsonParseError2::InvalidObjectClosing);
				return false;
			}
			break;
		case JsonToken::StartArray:
			m_currentState.state = State::ArrayStart;
			PushState();
			m_currentState.containerType = ContainerType::Array;
			break;
		case JsonToken::EndArray:
			// ↓ EndObject と同じ処理だから関数化してもいいかも。Json.NET では ValidateEnd()
			if (m_currentState.state == State::ArrayStart ||
				m_currentState.state == State::PostValue)
			{
				PopState();
				m_currentState.state = State::PostValue;
			}
			else
			{
				SetError(JsonParseError2::ArrayInvalidClosing);
				return false;
			}
			break;
		case JsonToken::PropertyName:
			m_currentState.state = State::Property;	// : まで読んでいる。次は値がほしい
			m_currentState.propertyName = m_value;
			break;
		case JsonToken::Int32:
		case JsonToken::Int64:
		case JsonToken::Float:
		case JsonToken::Double:
		case JsonToken::Null:
		case JsonToken::Boolean:
		case JsonToken::String:
			m_currentState.state = State::PostValue;
			break;
		default:
			SetError(JsonParseError2::UnexpectedToken);
			return false;
	}
	return true;
}

//------------------------------------------------------------------------------
void JsonReader2::PushState(/*ContainerType containerType*/)
{
	m_stateStack.Push(m_currentState);
}

//------------------------------------------------------------------------------
void JsonReader2::PopState()
{
	m_stateStack.Pop(&m_currentState);
}

//------------------------------------------------------------------------------
void JsonReader2::SetError(JsonParseError2 code, const String& message)
{
	m_error.code = code;
	m_error.message = message;
	m_error.line = m_reader->GetLineNumber();
	m_error.column = m_reader->GetColumnNumber();
}

} // namespace tr
LN_NAMESPACE_END
