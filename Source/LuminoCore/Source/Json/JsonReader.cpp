
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
void JsonReader::parse(const String& text)
{
	StringReader textReader(text);
	parse(&textReader);
}

//------------------------------------------------------------------------------
void JsonReader::parse(const TCHAR* text, int len)
{
	StringReader textReader(String(text, len));
	parse(&textReader);
}

//------------------------------------------------------------------------------
void JsonReader::parse(TextReader* textReader)
{
	if (LN_CHECK_ARG(textReader != nullptr)) return;

	m_reader = textReader;

	// 一時バッファ。もし足りなければ拡張される
	m_tmpStream.initialize(512);

	// バッファ先頭の空白を読み飛ばす
	if (!skipWhitespace())
	{
		// Error: バッファが空だった
		m_error.setError(JsonParseError::DocumentEmpty, m_currentCharCount);
		return;
	}

	// ルート要素の解析
	if (!parseValue()) {
		return;
	}

	// バッファ終端の空白を読み飛ばす
	if (skipWhitespace())
	{
		// Error: 複数のルート要素が見つかった
		m_error.setError(JsonParseError::DocumentRootNotSingular, m_currentCharCount);
		return;
	}
}

//------------------------------------------------------------------------------
bool JsonReader::skipWhitespace()
{
	while (m_reader->peek() == ' ' || m_reader->peek() == '\n' || m_reader->peek() == '\r' || m_reader->peek() == '\t') {
		m_reader->read();
	}
	return !m_reader->isEOF();
}

//------------------------------------------------------------------------------
bool JsonReader::parseValue()
{
	switch (m_reader->peek())
	{
		case 'n': return parseNull();			// null かもしれない
		case 't': return parseTrue();			// true かもしれない
		case 'f': return parseFalse();			// false かもしれない
		case '"': return parseString(false);	// 文字列かもしれない
		case '[': return parseArray();			// 配列かもしれない
		case '{': return parseObject();			// オブジェクトかもしれない
		default: return parseNumber();			// 数値かもしれない
	}
}

//------------------------------------------------------------------------------
bool JsonReader::parseNull()
{
	m_reader->read();	// skip 'n'
	if (m_reader->read() == 'u' &&
		m_reader->read() == 'l' &&
		m_reader->read() == 'l')
	{
		if (!m_handler->onNull())
		{
			// 中断
			m_error.setError(JsonParseError::Termination, m_currentCharCount);
			return false;
		}
	}
	else
	{
		// Error: "null" ではなかった
		m_error.setError(JsonParseError::ValueInvalid, m_currentCharCount);
		return false;
	}
	return true;
}

//------------------------------------------------------------------------------
bool JsonReader::parseTrue()
{
	m_reader->read();	// skip 't'
	if (m_reader->read() == 'r' &&
		m_reader->read() == 'u' &&
		m_reader->read() == 'e')
	{
		if (!m_handler->onBool(true))
		{
			// 中断
			m_error.setError(JsonParseError::Termination, m_currentCharCount);
			return false;
		}
	}
	else
	{
		// Error: "true" ではなかった
		m_error.setError(JsonParseError::ValueInvalid, m_currentCharCount);
		return false;
	}
	return true;
}

//------------------------------------------------------------------------------
bool JsonReader::parseFalse()
{
	m_reader->read();	// skip 'f'
	if (m_reader->read() == 'a' &&
		m_reader->read() == 'l' &&
		m_reader->read() == 's' &&
		m_reader->read() == 'e')
	{
		if (!m_handler->onBool(false))
		{
			// 中断
			m_error.setError(JsonParseError::Termination, m_currentCharCount);
			return false;
		}
	}
	else
	{
		// Error: "false" ではなかった
		m_error.setError(JsonParseError::ValueInvalid, m_currentCharCount);
		return false;
	}
	return true;
}

//------------------------------------------------------------------------------
bool JsonReader::parseNumber()
{
	// 数値への変換には strtod を使用する。そのため、まずは数値扱いできる文字を全て読み取る
	m_tmpStream.seek(0, SeekOrigin_Begin);
	int len = 0;
	TCHAR ch;
	while (true)
	{
		ch = m_reader->peek();	// 読むだけ。ポインタは進めない
		if (ch == '.' ||
			('0' <= ch && ch <= '9') ||
			(ch == 'e' || ch == 'E') ||
			(ch == '+' || ch == '-'))
		{
			m_tmpStream.write(&ch, sizeof(TCHAR));
			++len;
			m_reader->read();	// ここで1つ進める
		}
		else {
			break;				// 一致しなければポインタは進めない
		}
	}
	if (len == 0)
	{
		// Error: 数値っぽい文字が見つからなかった
		m_error.setError(JsonParseError::NumberInvalid, m_currentCharCount);
		return false;
	}
	ch = '\0';
	m_tmpStream.write(&ch, sizeof(TCHAR));	// 終端 \0

	// double へ変換する
	TCHAR* str = (TCHAR*)m_tmpStream.getBuffer();
	const TCHAR* endptr = NULL;
	NumberConversionResult result;
	double value = StringTraits::toDouble(str, len, &endptr, &result);
	if ((endptr - str) != len)	// 正常に変換できていれば、読み取った文字数が全て消費されるはず
	{
		// Error: 構文が正しくない
		m_error.setError(JsonParseError::NumberInvalid, m_currentCharCount);
		return false;
	}
	if (result == NumberConversionResult::Overflow) {
		// Error: オーバーフローが発生した
		m_error.setError(JsonParseError::NumberOverflow, m_currentCharCount);
		return false;
	}

	// Handler に通知する
	if (!m_handler->onDouble(value))
	{
		// 中断
		m_error.setError(JsonParseError::Termination, m_currentCharCount);
		return false;
	}
	return true;
}


//------------------------------------------------------------------------------
bool JsonReader::parseString(bool isKey)
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
	m_tmpStream.seek(0, SeekOrigin_Begin);

	m_reader->read();	// skip '"'
	while (true)
	{
		TCHAR c = m_reader->peek();

		// エスケープシーケンス
		if (c == '\\')
		{
			m_reader->read();	// skip '\'
			TCHAR esc = m_reader->read();
			// 基本的なエスケープ
			if (unsigned(esc) < 256 && escapeTable[(unsigned char)esc])
			{
				m_tmpStream.write(&escapeTable[(unsigned char)esc], sizeof(TCHAR));
			}
			// Unicode エスケープ
			else if (esc == 'u')
			{
				// 未実装
				m_error.setError(JsonParseError::StringEscapeInvalid, m_currentCharCount);
				return false;
			}
			else
			{
				// Error: 無効なエスケープ
				m_error.setError(JsonParseError::StringEscapeInvalid, m_currentCharCount);
				return false;
			}
		}
		// 文字列終端
		else if (c == '"')
		{
			m_reader->read();	// skip '"'
			break;
		}
		// 文字列の途中でバッファが切れた
		else if (m_reader->isEOF() || c == '\0')
		{
			// Error: " が一致しなかった
			m_error.setError(JsonParseError::StringMissQuotationMark, m_currentCharCount);
			return false;
		}
		// 0x20 未満の制御文字は使えない
		else if ((unsigned)c < 0x20) {
			// RFC 4627: unescaped = %x20-21 / %x23-5B / %x5D-10FFFF
			m_error.setError(JsonParseError::StringEscapeInvalid, m_currentCharCount);
			return false;
		}
		// 普通の文字
		else
		{
			m_tmpStream.write(&c, sizeof(TCHAR));
			m_reader->read();
		}
	}

	// Handler に通知
	bool cont = false;
	if (isKey) {
		cont = m_handler->onKey((TCHAR*)m_tmpStream.getBuffer(), ((int)m_tmpStream.getPosition()) / sizeof(TCHAR));
	}
	else {
		cont = m_handler->onString((TCHAR*)m_tmpStream.getBuffer(), ((int)m_tmpStream.getPosition()) / sizeof(TCHAR));
	}
	if (!cont)
	{
		// 中断
		m_error.setError(JsonParseError::Termination, m_currentCharCount);
		return false;
	}
	return true;
}

//------------------------------------------------------------------------------
bool JsonReader::parseArray()
{
	m_reader->read();  // skip '['

	// Handler に Array の開始を通知
	if (!m_handler->onStartArray())
	{
		// 中断
		m_error.setError(JsonParseError::Termination, m_currentCharCount);
		return false;
	}

	skipWhitespace();
	if (m_reader->peek() == ']')
	{
		// 空配列だった。Array の終了を通知する
		m_reader->read(); 	// ']' の次を指しておく
		if (!m_handler->onEndArray(0))
		{
			// 中断
			m_error.setError(JsonParseError::Termination, m_currentCharCount);
			return false;
		}
		return true;
	}

	skipWhitespace();
	size_t elementCount = 0;
	while (true)
	{
		if (!parseValue()) {
			return false;	// エラーは処理済み
		}

		++elementCount;
		skipWhitespace();

		switch (m_reader->read())
		{
		case ',':
			skipWhitespace();
			break;
		case ']':
			// 配列の終端
			if (!m_handler->onEndArray(elementCount))
			{
				// 中断
				m_error.setError(JsonParseError::Termination, m_currentCharCount);
				return false;
			}
			skipWhitespace();
			return true;
		default:
			m_error.setError(JsonParseError::ArrayMissCommaOrSquareBracket, m_currentCharCount);
			return false;
		}
	}

	// ここに来ることはない
}

//------------------------------------------------------------------------------
bool JsonReader::parseObject()
{
	m_reader->read();  // Skip '{'

	// オブジェクト定義の開始
	if (!m_handler->onStartObject())
	{
		// 中断
		m_error.setError(JsonParseError::Termination, m_currentCharCount);
		return false;
	}

	skipWhitespace();
	if (m_reader->peek() == '}')
	{
		// メンバが1つも無かった
		m_reader->read();	// '}' の次を指しておく
		if (!m_handler->onEndObject(0))
		{
			// 中断
			m_error.setError(JsonParseError::Termination, m_currentCharCount);
			return false;
		}
		return true;
	}

	int memberCount = 0;
	while (true)
	{
		// 最初はメンバ名
		if (m_reader->peek() != '"')
		{
			// Error: メンバ名の開始が見つからなかった
			m_error.setError(JsonParseError::ObjectMissKeyStart, m_currentCharCount);
			return false;
		}
		if (!parseString(true)) return false;
		skipWhitespace();

		// 続いて ':'
		if (m_reader->read() != ':')
		{
			// Error: ':' が見つからなかった
			m_error.setError(JsonParseError::ObjectMissColon, m_currentCharCount);
			return false;
		}
		skipWhitespace();

		// 最後に値
		if (!parseValue()) return false;
		skipWhitespace();

		++memberCount;

		switch (m_reader->read())
		{
		case ',':
			skipWhitespace();
			break;
		case '}':
			// オブジェクト定義終端
			if (!m_handler->onEndObject(memberCount))
			{
				// 中断
				m_error.setError(JsonParseError::Termination, m_currentCharCount);
				return false;
			}
			skipWhitespace();
			return true;
		default:
			m_error.setError(JsonParseError::ObjectMissCommaOrCurlyBracket, m_currentCharCount);
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
bool JsonReader2::read()
{
	return tryRead();
	// TODO: 例外
}

//------------------------------------------------------------------------------
bool JsonReader2::tryRead()
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
				return parseValue();
			case State::ObjectStart:
			case State::Object:
				return parseObject();
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
JsonToken JsonReader2::getTokenType() const
{
	return m_currentToken.type;
}

//------------------------------------------------------------------------------
const String& JsonReader2::getValue() const
{
	return m_value;
}

//------------------------------------------------------------------------------
const String& JsonReader2::getPropertyName() const
{
	return m_currentState.propertyName;
}

//------------------------------------------------------------------------------
bool JsonReader2::getBoolValue() const
{
	if (LN_CHECK_STATE(m_currentToken.type == JsonToken::Boolean)) return false;
	return m_valueData.m_bool;
}

//------------------------------------------------------------------------------
int32_t JsonReader2::getInt32Value() const
{
	if (LN_CHECK_STATE(m_currentToken.type == JsonToken::Int32)) return 0;
	return m_valueData.m_int32;
}

//------------------------------------------------------------------------------
int64_t JsonReader2::getInt64Value() const
{
	if (LN_CHECK_STATE(m_currentToken.type == JsonToken::Int64)) return 0;
	return m_valueData.m_int64;
}

//------------------------------------------------------------------------------
float JsonReader2::getFloatValue() const
{
	if (LN_CHECK_STATE(m_currentToken.type == JsonToken::Float)) return 0;
	return m_valueData.m_float;
}

//------------------------------------------------------------------------------
double JsonReader2::getDoubleValue() const
{
	if (LN_CHECK_STATE(m_currentToken.type == JsonToken::Double)) return 0;
	return m_valueData.m_double;
}

//------------------------------------------------------------------------------
const JsonError2& JsonReader2::getError() const
{
	return m_error;
}

void JsonReader2::readAsStartObject()
{
	if (!read() || getTokenType() != JsonToken::StartObject)
	{
		LN_THROW(0, InvalidFormatException);
	}
}

void JsonReader2::readAsEndObject()
{
	if (!read() || getTokenType() != JsonToken::EndObject)
	{
		LN_THROW(0, InvalidFormatException);
	}
}

void JsonReader2::readAsStartArray()
{
	if (!read() || getTokenType() != JsonToken::StartArray)
	{
		LN_THROW(0, InvalidFormatException);
	}
}

void JsonReader2::readAsEndArray()
{
	if (!read() || getTokenType() != JsonToken::EndArray)
	{
		LN_THROW(0, InvalidFormatException);
	}
}

bool JsonReader2::readAsBool()
{
	if (!read() || getTokenType() != JsonToken::Boolean)
	{
		LN_THROW(0, InvalidFormatException);
	}
	return getValue()[0] == 't';
}

const String& JsonReader2::readAsPropertyName()
{
	if (!read() || getTokenType() != JsonToken::PropertyName)
	{
		LN_THROW(0, InvalidFormatException);
	}
	return getValue();
}

const String& JsonReader2::readAsString()
{
	if (!read() || getTokenType() != JsonToken::String)
	{
		LN_THROW(0, InvalidFormatException);
	}
	return getValue();
}

//------------------------------------------------------------------------------
bool JsonReader2::skipWhitespace()
{
	while (m_reader->peek() == ' ' || m_reader->peek() == '\n' || m_reader->peek() == '\r' || m_reader->peek() == '\t') {
		m_reader->read();
	}
	return !m_reader->isEOF();	// スキップした後に EOF にたどり着いたら false
}

//------------------------------------------------------------------------------
bool JsonReader2::parseValue()
{
	if (!skipWhitespace()) return false;

	//while (true)
	{
		//if (m_reader->IsEOF())
		//{
		//	return false;
		//}

		TCHAR ch = m_reader->peek();
		switch (ch)
		{
			//case ' ':
			//case '\t':
			//	m_reader->Read();	// 空白は消費するだけ
			//	break;
			case '{':
				setToken(JsonToken::StartObject);
				m_reader->read();
				return true;
			case '[':
				setToken(JsonToken::StartArray);
				m_reader->read();
				return true;
			case ']':	// 空配列
				setToken(JsonToken::EndArray);
				m_reader->read();
				return true;
			case '"':
				return parseString(false);

			case 'n':
				return parseNull();
			case 't':
				return parseTrue();
			case 'f':
				return parseFalse();
				//case 't': return ParseTrue();			// true かもしれない
				//case 'f': return ParseFalse();			// false かもしれない
				//case '"': return ParseString(false);	// 文字列かもしれない
				//case '[': return ParseArray();			// 配列かもしれない
				//case '{': return ParseObject();			// オブジェクトかもしれない
				//default: return ParseNumber();			// 数値かもしれない

			default:
				if (isdigit(ch) || ch == '-' || ch == '.')
				{
					return parseNumber();
				}
				return false;	// TODO

			
		}
	}
}

//------------------------------------------------------------------------------
bool JsonReader2::parseNull()
{
	m_reader->read();	// skip 'n'
	if (m_reader->read() == 'u' &&
		m_reader->read() == 'l' &&
		m_reader->read() == 'l')
	{
		return setToken(JsonToken::Null);
	}
	else
	{
		// Error: "null" ではなかった
		setError(JsonParseError2::ValueInvalid);
		return false;
	}
}

//------------------------------------------------------------------------------
bool JsonReader2::parseTrue()
{
	m_reader->read();	// skip 't'
	if (m_reader->read() == 'r' &&
		m_reader->read() == 'u' &&
		m_reader->read() == 'e')
	{
		m_valueData.m_bool = true;
		return setToken(JsonToken::Boolean, _T("true"), 4);
	}
	else
	{
		// Error: "true" ではなかった
		setError(JsonParseError2::ValueInvalid);
		return false;
	}
}

//------------------------------------------------------------------------------
bool JsonReader2::parseFalse()
{
	m_reader->read();	// skip 'f'
	if (m_reader->read() == 'a' &&
		m_reader->read() == 'l' &&
		m_reader->read() == 's' &&
		m_reader->read() == 'e')
	{
		m_valueData.m_bool = false;
		return setToken(JsonToken::Boolean, _T("false"), 5);
	}
	else
	{
		// Error: "false" ではなかった
		setError(JsonParseError2::ValueInvalid);
		return false;
	}
}

//------------------------------------------------------------------------------
bool JsonReader2::parseNumber()
{
	// 数値として使える文字を m_textCache に入れていく
	int len = 0;
	bool isDecimal = false;
	TCHAR ch;
	while (true)
	{
		ch = m_reader->peek();	// 読むだけ。ポインタは進めない
		if (ch == '.' ||
			('0' <= ch && ch <= '9') ||
			(ch == 'e' || ch == 'E') ||
			(ch == '+' || ch == '-'))
		{
			m_textCache.add(ch);
			++len;
			m_reader->read();	// ここで1つ進める

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
		setError(JsonParseError2::UnexpectedToken);
		return false;
	}

	if (isDecimal)
	{
		const TCHAR* str = &m_textCache[0];
		const TCHAR* endptr = nullptr;
		NumberConversionResult result;
		double value = StringTraits::toDouble(str, len, &endptr, &result);

		if ((endptr - str) != len)	// 正常に変換できていれば、読み取った文字数が全て消費されるはず
		{
			// Error: 構文が正しくない
			setError(JsonParseError2::InvalidNumber);
			return false;
		}
		if (result == NumberConversionResult::Overflow)
		{
			// Error: オーバーフローが発生した
			setError(JsonParseError2::NumberOverflow);
			return false;
		}

		if (-FLT_MAX <= value && value <= FLT_MAX)
		{
			m_valueData.m_float = (float)value;
			return setToken(JsonToken::Float, str, len);
		}
		else
		{
			m_valueData.m_double = value;
			return setToken(JsonToken::Double, str, len);
		}
	}
	else
	{
		const TCHAR* str = &m_textCache[0];
		const TCHAR* endptr = nullptr;
		NumberConversionResult result;
		int64_t value = StringTraits::toInt64(str, len, 0, &endptr, &result);

		if ((endptr - str) != len)	// 正常に変換できていれば、読み取った文字数が全て消費されるはず
		{
			// Error: 構文が正しくない
			setError(JsonParseError2::InvalidNumber);
			return false;
		}
		if (result == NumberConversionResult::Overflow)
		{
			// Error: オーバーフローが発生した
			setError(JsonParseError2::NumberOverflow);
			return false;
		}

		if (INT32_MIN <= value && value <= INT32_MAX)
		{
			m_valueData.m_int32 = (int32_t)value;
			return setToken(JsonToken::Int32, str, len);
		}
		else
		{
			m_valueData.m_int64 = value;
			return setToken(JsonToken::Int64, str, len);
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
bool JsonReader2::parseObject()
{
	if (!skipWhitespace()) return false;

	switch (m_reader->peek())
	{
	case '}':
		setToken(JsonToken::EndObject);
		m_reader->read();
		return true;
	default:
		return tryParsePropertyName();
	}
}

//------------------------------------------------------------------------------
bool JsonReader2::tryParsePropertyName()
{
	/*
		::member = string name-separator value
		パース時点ではプロパティ名の空文字を許している。
	*/
	if (m_reader->peek() == '"')
	{
		if (!parseString(true))
		{
			// Error: " から始まる文字列ではなかった
			// Invalid property identifier character:
			return false;
		}

		if (!skipWhitespace()) {
			// Error: EOF した
			return false;
		}

		if (m_reader->peek() == ':') {
			// Invalid character after parsing property name. Expected ':'
		}
		m_reader->read();

		if (!m_textCache.isEmpty()) {
			setToken(JsonToken::PropertyName, &m_textCache[0], m_textCache.getCount());
		}
		else {
			setToken(JsonToken::PropertyName);
		}

		return true;
	}
	else
	{
		return false;
	}

}

//------------------------------------------------------------------------------
bool JsonReader2::parseString(bool isKey)
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

	m_reader->read();	// skip '"'
	while (true)
	{
		TCHAR c = m_reader->peek();

		// エスケープシーケンス
		if (c == '\\')
		{
			m_reader->read();	// skip '\'
			TCHAR esc = m_reader->read();
			// 基本的なエスケープ
			if (unsigned(esc) < 256 && escapeTable[(unsigned char)esc])
			{
				m_textCache.add(escapeTable[(unsigned char)esc]);
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
				setError(JsonParseError2::InvalidStringEscape);
				return false;
			}
		}
		// 文字列終端
		else if (c == '"')
		{
			m_reader->read();	// skip '"'
			break;
		}
		// 文字列の途中でバッファが切れた
		else if (m_reader->isEOF() || c == '\0')
		{
			// " が一致しなかった
			setError(JsonParseError2::UnterminatedString);
			return false;
		}
		// 0x20 未満の制御文字は使えない
		else if ((unsigned)c < 0x20)
		{
			setError(JsonParseError2::InvalidStringChar);
			return false;
		}
		// 普通の文字
		else
		{
			m_textCache.add(c);
			m_reader->read();
		}
	}

	if (isKey)
	{
	}
	else
	{
		if (!m_textCache.isEmpty()) {
			setToken(JsonToken::String, &m_textCache[0], m_textCache.getCount());
		}
		else {
			setToken(JsonToken::String);
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
	if (!skipWhitespace()) return false;

	switch (m_reader->peek())
	{
	case '}':
		setToken(JsonToken::EndObject);
		m_reader->read();
		return true;
	case ']':
		setToken(JsonToken::EndArray);
		m_reader->read();
		return true;
	case ',':
		*outSkip = true;	// 今回の , は Read() で返さず、次のトークンを読みに行く
		if (m_currentState.containerType == ContainerType::Object) {
			m_currentState.state = State::Object;
		}
		else if (m_currentState.containerType == ContainerType::Array) {
			m_currentState.state = State::Array;
		}
		m_reader->read();
		return true;
	default:
		return false;	// Error
	}
}

//------------------------------------------------------------------------------
// 現在位置の状態を newToken にする
//------------------------------------------------------------------------------
bool JsonReader2::setToken(JsonToken newToken, const TCHAR* value, int valueLen)
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
		m_value = String::getEmpty();
	}

	switch (m_currentToken.type)
	{
		//case State::Start:
		//	break;
		case JsonToken::StartObject:
			m_currentState.state = State::ObjectStart;
			pushState();
			m_currentState.containerType = ContainerType::Object;
			break;
		case JsonToken::EndObject:
			if (m_currentState.state == State::ObjectStart ||
				m_currentState.state == State::PostValue)
			{
				popState();
				m_currentState.state = State::PostValue;
			}
			else
			{
				setError(JsonParseError2::InvalidObjectClosing);
				return false;
			}
			break;
		case JsonToken::StartArray:
			m_currentState.state = State::ArrayStart;
			pushState();
			m_currentState.containerType = ContainerType::Array;
			break;
		case JsonToken::EndArray:
			// ↓ EndObject と同じ処理だから関数化してもいいかも。Json.NET では ValidateEnd()
			if (m_currentState.state == State::ArrayStart ||
				m_currentState.state == State::PostValue)
			{
				popState();
				m_currentState.state = State::PostValue;
			}
			else
			{
				setError(JsonParseError2::ArrayInvalidClosing);
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
			setError(JsonParseError2::UnexpectedToken);
			return false;
	}
	return true;
}

//------------------------------------------------------------------------------
void JsonReader2::pushState(/*ContainerType containerType*/)
{
	m_stateStack.push(m_currentState);
}

//------------------------------------------------------------------------------
void JsonReader2::popState()
{
	m_stateStack.pop(&m_currentState);
}

//------------------------------------------------------------------------------
void JsonReader2::setError(JsonParseError2 code, const String& message)
{
	m_error.code = code;
	m_error.message = message;
	m_error.line = m_reader->getLineNumber();
	m_error.column = m_reader->getColumnNumber();
}

} // namespace tr
LN_NAMESPACE_END
