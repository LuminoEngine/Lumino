
#pragma once
#include "../Base/Stack.h"
#include "../IO/TextWriter.h"
#include "Common.h"

LN_NAMESPACE_BEGIN
namespace tr {

/**
	@brief	
*/
class JsonWriter
{
public:
	JsonWriter(TextWriter* textWriter);
	virtual ~JsonWriter();

public:
	void SetFormatting(JsonFormatting formatting);
	void WriteStartObject();
	void WriteEndObject();
	void WriteStartArray();
	void WriteEndArray();
	void WritePropertyName(const TCHAR* str, int length = -1);
	void WriteNull();
	void WriteBool(bool value);
	void WriteInt32(int32_t value);
	void WriteInt64(int64_t value);
	void WriteFloat(float value);
	void WriteDouble(double value);
	void WriteString(const TCHAR* str, int length = -1);
	bool IsComplete() const;

protected:
	enum PrefixType
	{
		PrefixType_Array = 0,
		PrefixType_Object,
		PrefixType_Key,
	};

	void OnPrefix(PrefixType type, int valueCount);
	void OnStartObject();
	void OnEndObject();
	void OnStartArray();
	void OnEndArray();
	void OnKey(const TCHAR* str, int length);
	void OnNull();
	void OnBool(bool value);
	void OnInt32(int32_t value);
	void OnInt64(int64_t value);
	void OnFloat(float value);
	void OnDouble(double value);
	void OnString(const TCHAR* str, int length);
	//void OnIndent(int level);

private:
	void AutoComplete(JsonToken token);

private:
	struct Level
	{
		Level(bool inArray) : valueCount(0), inArray(inArray), justSawKey(false), justSawContainerEnd(false){}
		int		valueCount;				// 現在のネストレベルに含まれている値の数。=配列の要素数またはオブジェクトのメンバ数
		bool	inArray;				// true なら配列、false ならオブジェクト
		bool	justSawKey;
		bool	justSawContainerEnd;	// for indent
	};

	JsonFormatting	m_formatting;
	TextWriter*		m_textWriter;
	Stack<Level>	m_levelStack;
	bool			m_isComplete;
};

} // namespace tr
LN_NAMESPACE_END
