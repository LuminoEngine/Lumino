
#pragma once
#include <stack>
#include "../IO/TextReader.hpp"
#include "Common.hpp"
#include "JsonDiagnostics.h"

namespace ln {
namespace tr {




namespace detail
{

class PositioningTextReader
	: public TextReader
{
public:
	PositioningTextReader(TextReader* innter);
	virtual int peek() override;
	virtual int read() override;
	virtual bool readLine(String* line) override;
	virtual String readToEnd() override;
	virtual bool isEOF() override;
	int getPosition() const;
	int getLineNumber() const;
	int getColumnNumber() const;

private:
	void advancePosition(Char ch);

	Ref<TextReader>	m_innter;
	int					m_pos;
	int					m_line;
	int					m_column;
	bool				m_lastCR;
};

} // namespace detail


class JsonReader2
{
public:
	JsonReader2(const String& text);
	JsonReader2(TextReader* textReader);
	~JsonReader2();

public:
	
	/**
		次のノード読み取ります。　

		正常に読み取られた場合は true を返します。続いて getValue() などで値を取得できます。
		それ以上読み取るノードが存在しない場合は false を返します。
	*/
	bool read();


	/**
		@brief		現在のノードの種類を取得します。
	*/
	JsonToken getTokenType() const;

	/**
		@brief		現在のノードの値 (文字列形式) を取得します。
		@details	値が無い場合は空文字列を返します。
	*/
	const String& getValue() const;

	const String& getPropertyName() const;

	/** 現在のノードが Bool である場合、パース結果の bool 値を取得できます。*/
	bool getBoolValue() const;
	/** 現在のノードが Int32 である場合、パース結果の int32_t 値を取得できます。*/
	int32_t getInt32Value() const;
	/** 現在のノードが Int64 である場合、パース結果の int64_t 値を取得できます。*/
	int64_t getInt64Value() const;
	/** 現在のノードが Float である場合、パース結果の float 値を取得できます。*/
	float getFloatValue() const;
	/** 現在のノードが Double である場合、パース結果の double 値を取得できます。*/
	double getDoubleValue() const;


	const JsonError2& getError() const;


	void readAsStartObject();
	void readAsEndObject();
	void readAsStartArray();
	void readAsEndArray();
	const String& readAsPropertyName();
	bool readAsBool();
	const String& readAsString();

private:
	enum class ResultState
	{
		Success = 0,
		Error = 1,
	};

	enum class State
	{
		Start,
		ObjectStart,	// { の次の状態。次は } でも良い。
		Object,			// , の次の状態。次は } ではダメ。
		ArrayStart,
		Array,
		Property,
		PostValue,
	};

	enum class ContainerType
	{
		None,
		Object,
		Array,
	};

	struct Token
	{
		JsonToken	type;
		//int			valuePos;	// m_textCache 上のインデックス
		//int			valueLen;	// m_textCache 上のインデックス
	};

	struct ParserState
	{
		State				state = State::Start;
		String				propertyName;
		ContainerType		containerType = ContainerType::None;	// 現在解析中のブロックの種類
	};

	union 
	{
		bool	m_bool;
		int32_t	m_int32;
		int64_t	m_int64;
		float	m_float;
		double	m_double;

	} m_valueData;

	//Ref<TextReader>		m_reader;
	Ref<detail::PositioningTextReader>	m_reader;
	//State					m_currentState;
	ParserState				m_currentState;
	Token					m_currentToken;
	String					m_value;
	List<Char>				m_textCache;
	std::stack<ParserState>		m_stateStack;
	JsonError2				m_error;

	JsonReader2();
	bool skipWhitespace();
	bool parseValue();
	bool parseNull();
	bool parseTrue();
	bool parseFalse();
	bool parseNumber();
	bool parseObject();
	bool tryParsePropertyName();
	bool parseString(bool isKey);
	bool ParsePostValue(bool* outSkip);

	bool setToken(JsonToken newToken, const Char* value = nullptr, int valueLen = 0);
	void pushState(/*ContainerType containerType*/);
	void popState();
	void setError(JsonParseError2 code, const String& message = String::getEmpty());
};

} // namespace tr
} // namespace ln
