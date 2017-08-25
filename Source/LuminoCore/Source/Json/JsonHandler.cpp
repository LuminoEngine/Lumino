
#include "../Internal.h"
#include <Lumino/Json/JsonValue.h>
#include <Lumino/Json/JsonDocument.h>
#include <Lumino/Json/JsonHandler.h>

LN_NAMESPACE_BEGIN
namespace tr {

//==============================================================================
// JsonHandler
//==============================================================================

//------------------------------------------------------------------------------
JsonHandler::~JsonHandler()
{
}

//==============================================================================
// JsonDOMHandler
//==============================================================================
/*
	On～() で正直に親の JsonValue へ追加して… なんてことやってると、
	かなりの回数のメモリ再配置が発生する。

	JsonValue は基本的に値型として使われる。ArrayList.Add() なんてすれば
	JsonValue が丸ごとコピー、つまり子 Value も全部コピーすることになる。
	(JsonValue を RefObject にする手もあるが、それはそれで頻繁な new が必要になる)

	これを避けるため、DOM の構築は2ステップに分かれている。
	step1:
		On～() で得られた値をバイナリデータにしてメモリに一時保存する。
		1度全て解析することで配列の要素数など、必要なメモリサイズが全てわかる。

	step2:
		一時保存したメモリからツリー構造を構築する。
		step1 で求めた要素数などを使用することで、メモリ確保回数を最小限に抑える。
*/

//------------------------------------------------------------------------------
JsonDOMHandler::JsonDOMHandler(JsonDocument* document)
	: m_document(document)
	, m_valueRawData()
	, m_writer(&m_valueRawData)
{
	if (LN_CHECK_ARG(document != nullptr)) return;
	m_valueRawData.initialize(2048);
}

//------------------------------------------------------------------------------
JsonDOMHandler::~JsonDOMHandler()
{
}

//------------------------------------------------------------------------------
bool JsonDOMHandler::onNull()
{
	m_writer.writeUInt8(JsonType::Null);
	return true;
}

//------------------------------------------------------------------------------
bool JsonDOMHandler::onBool(bool value)
{
	m_writer.writeUInt8(JsonType::Bool);
	m_writer.writeUInt8(value ? 1 : 0);
	return true;
}

//------------------------------------------------------------------------------
bool JsonDOMHandler::onDouble(double value)
{
	m_writer.writeUInt8(JsonType::Double);
	m_writer.writeDouble(value);
	return true;
}

//------------------------------------------------------------------------------
bool JsonDOMHandler::onString(const Char* str, int len)
{
	m_writer.writeUInt8(JsonType::String);
	m_writer.writeInt32(len);
	m_writer.write(str, sizeof(Char) * len);
	return true;
}

//------------------------------------------------------------------------------
bool JsonDOMHandler::onStartArray()
{
	m_writer.writeUInt8(JsonType::Array);
	m_startIndexStack.push((size_t)m_writer.getPosition());	// 現在位置を Array の開始点として覚えておく
	m_writer.writeUInt32(0);						// 要素数 (ダミー)
	return true;
}

//------------------------------------------------------------------------------
bool JsonDOMHandler::onEndArray(int elementCount)
{
	size_t index;
	m_startIndexStack.pop(&index);

	size_t curPos = (size_t)m_writer.getPosition();
	m_writer.seek(index, SeekOrigin_Begin);
	m_writer.writeUInt32(elementCount);				// ダミーとして確保していた場所に要素数を書く
	m_writer.seek(curPos, SeekOrigin_Begin);
	return true;
}

//------------------------------------------------------------------------------
bool JsonDOMHandler::onStartObject()
{
	m_writer.writeUInt8(JsonType::Object);
	m_startIndexStack.push((size_t)m_writer.getPosition());	// 現在位置を Object の開始点として覚えておく
	m_writer.writeUInt32(0);						// 要素数 (ダミー)
	return true;
}

//------------------------------------------------------------------------------
bool JsonDOMHandler::onKey(const Char* str, int len)
{
	m_writer.writeUInt8(0x80 | JsonType::String);		// キーであることを示すために最上位 bit を立てておく
	m_writer.writeInt32(len);
	m_writer.write(str, sizeof(Char) * len);
	return true;
}

//------------------------------------------------------------------------------
bool JsonDOMHandler::onEndObject(int memberCount)
{
	size_t index;
	m_startIndexStack.pop(&index);

	size_t curPos = (size_t)m_writer.getPosition();
	m_writer.seek(index, SeekOrigin_Begin);
	m_writer.writeUInt32(memberCount);				// ダミーとして確保していた場所に要素数を書く
	m_writer.seek(curPos, SeekOrigin_Begin);
	return true;
}

//------------------------------------------------------------------------------
void JsonDOMHandler::build()
{
	m_valueRawData.seek(0, SeekOrigin_Begin);
	BinaryReader reader(&m_valueRawData);
	buildValue(&reader, m_document);
}

//------------------------------------------------------------------------------
void JsonDOMHandler::buildValue(BinaryReader* reader, JsonValue* v)
{
	uint8_t type = reader->readUInt8();
	switch (type)
	{
	case JsonType::Null:
		v->setNull();
		break;
	case JsonType::Bool:
		v->setBool(reader->readUInt8() != 0);
		break;
	case JsonType::Double:
		v->setDouble(reader->readDouble());
		break;
	case JsonType::String:
	{
		int len = reader->readInt32();
		v->setString(String((Char*)m_valueRawData.getBuffer((size_t)m_valueRawData.getPosition()), len));	// 生メモリから文字列を生成
		m_valueRawData.seek(len * sizeof(Char), SeekOrigin_Current);								// 直接メモリを読んだので自分で進める
		break;
	}
	case JsonType::Array:
		v->setArray();
		v->resizeValueList(reader->readUInt32());	// 次の uint32 が要素数
		for (int i = 0; i < v->getItemCount(); i++)
		{
			buildValue(reader, &v->getAtValue(i));
		}
		break;
	case JsonType::Object:
	{
		v->setObject();
		v->resizeMemberList(reader->readUInt32());	// 次の uint32 が要素数
		for (int i = 0; i < v->getMemberCount(); i++)
		{
			buildMember(reader, &v->getAtMember(i));
		}
		break;
	}
	default:
		LN_THROW(0, InvalidOperationException);	// fail safe.
		break;
	}
}

//------------------------------------------------------------------------------
void JsonDOMHandler::buildMember(BinaryReader* reader, JsonMember* m)
{
	//LN_ASSERT(reader->ReadUInt8() == (0x80 | JsonType::String));	// 種別は必ず Key であるはず

	// キー文字列
	uint8_t type = reader->readUInt8();
	assert(type == (0x80 | JsonType::String));	// 種別は必ず Key であるはず

	int len = reader->readInt32();
	m->Name = String((Char*)m_valueRawData.getBuffer((size_t)m_valueRawData.getPosition()), len);	// 生メモリから文字列を生成
	m_valueRawData.seek(len * sizeof(Char), SeekOrigin_Current);							// 直接メモリを読んだので自分で進める

	// 値
	buildValue(reader, &m->Value);
}

} // namespace tr
LN_NAMESPACE_END
