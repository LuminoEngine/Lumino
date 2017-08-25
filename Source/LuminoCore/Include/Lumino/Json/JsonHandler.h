
#pragma once
#include "../Base/Stack.h"
#include "../IO/BinaryReader.h"
#include "../IO/BinaryWriter.h"
#include "../IO/MemoryStream.h"

LN_NAMESPACE_BEGIN
namespace tr {
class JsonDocument;
class JsonValue;
class JsonMember;

/**
	@brief	SAX スタイルの JSON パーサから通知を受け取るインターフェイスです。
*/
class JsonHandler
{
public:
	virtual ~JsonHandler();

public:
	virtual bool onNull() = 0;
	virtual bool onBool(bool value) = 0;
	virtual bool onDouble(double value) = 0;
	virtual bool onString(const Char* str, int len) = 0;
	virtual bool onStartArray() = 0;
	virtual bool onEndArray(int elementCount) = 0;
	virtual bool onStartObject() = 0;
	virtual bool onKey(const Char* str, int len) = 0;
	virtual bool onEndObject(int memberCount) = 0;
};

/**
	@brief	JsonDocument を構築するための JsonHandler の実装です。
*/
class JsonDOMHandler
	: public JsonHandler
{
public:
	JsonDOMHandler(JsonDocument* document);
	virtual ~JsonDOMHandler();

public:
	virtual bool onNull();
	virtual bool onBool(bool value);
	virtual bool onDouble(double value);
	virtual bool onString(const Char* str, int len);
	virtual bool onStartArray();
	virtual bool onEndArray(int elementCount);
	virtual bool onStartObject();
	virtual bool onKey(const Char* str, int len);
	virtual bool onEndObject(int memberCount);

	void build();	// Parse が終わった後に呼び出す

private:
	void buildValue(BinaryReader* reader, JsonValue* v);
	void buildMember(BinaryReader* reader, JsonMember* m);

private:
	JsonDocument*	m_document;
	MemoryStream	m_valueRawData;
	BinaryWriter	m_writer;
	Stack<size_t>	m_startIndexStack;
};

} // namespace tr
LN_NAMESPACE_END
