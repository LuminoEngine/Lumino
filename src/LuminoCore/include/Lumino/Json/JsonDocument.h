
#pragma once
#include "Common.hpp"
#include "../Base/String.hpp"

namespace ln {
class JsonWriter;
class JsonReader;
namespace tr {
class JsonElement;
class JsonObject;
class JsonDocument2;
namespace detail { class JsonElementCache; }

/** Json の値の型を示します。*/
enum class JsonElementType
{
	Null,
	Bool,
	Int32,
	Int64,
	Float,
	Double,
	String,
	Array,
	Object,
};

/** JSON ドキュメントの構成要素のベースクラスです。 */
class JsonElement
	: public RefObject
{
public:
	JsonElementType type() const { return m_type; }

protected:
	JsonElement(JsonDocument2* owner);
	virtual ~JsonElement();
	virtual void onSave(JsonWriter* writer) = 0;
	virtual JsonParseResult onLoad(JsonReader* reader) = 0;

LN_INTERNAL_ACCESS:
	JsonDocument2* getOwnerDocument() const { return m_ownerDoc; }
	void setType(JsonElementType type) { m_type = type; }
	void save(JsonWriter* writer) { onSave(writer); }
	JsonParseResult load(JsonReader* reader) { return onLoad(reader); }

private:
	JsonDocument2*	m_ownerDoc;
	JsonElementType	m_type;

	friend class JsonObject;
	friend class detail::JsonElementCache;
};

/** JSON のプリミティブな値を表します。 */
class JsonValue
	: public JsonElement
{
public:
	void setNullValue();
	void setBoolValue(bool value);
	void setInt32Value(int32_t value);
	void setInt64Value(int64_t value);
	void setFloatValue(float value);
	void setDoubleValue(double value);
	void setStringValue(const StringRef& value);

	bool isNull() const;
	bool boolValue() const;
	int32_t int32Value() const;
	int64_t int64Value() const;
	float floatValue() const;
	double doubleValue() const;
	const String& stringValue() const;

private:
	JsonValue(JsonDocument2* ownerDoc);
	virtual ~JsonValue();
	void checkRelease();

	virtual void onSave(JsonWriter* writer) override;
	virtual JsonParseResult onLoad(JsonReader* reader) override;

	union
	{
		bool	m_bool;
		int32_t	m_int32;
		int64_t	m_int64;
		float	m_float;
		double	m_double;
		String*	m_string;
	};

	friend class JsonDocument2;
};

/** JSON の配列を表します。 */
class JsonArray
	: public JsonElement
{
public:
	void addNullValue();
	void addBoolValue(bool value);
	void addInt32Value(int32_t value);
	void addInt64Value(int64_t value);
	void addFloatValue(float value);
	void addDoubleValue(double value);
	void addStringValue(const StringRef& value);

	JsonArray* addArray();
	JsonObject* addObject();

	int elementCount() const { return m_itemList.size(); }
	JsonElement* element(int index) const { return m_itemList.at(index); }

private:
	JsonArray(JsonDocument2* ownerDoc);
	virtual ~JsonArray();
	virtual void onSave(JsonWriter* writer) override;
	virtual JsonParseResult onLoad(JsonReader* reader) override;

	List<JsonElement*>	m_itemList;

	friend class JsonDocument2;
};

/** JSON のオブジェクトを表します。 */
class JsonObject
	: public JsonElement
{
public:
	void addNullValue(const StringRef& name);
	void addBoolValue(const StringRef& name, bool value);
	void addInt32Value(const StringRef& name, int32_t value);
	void addInt64Value(const StringRef& name, int64_t value);
	void addFloatValue(const StringRef& name, float value);
	void addDoubleValue(const StringRef& name, double value);
	void addStringValue(const StringRef& name, const StringRef& value);

	JsonArray* addArray(const StringRef& name);
	JsonObject* addObject(const StringRef& name);

	JsonElement* find(const StringRef& name) const;

	int memberCount() const { return m_memberList.size(); }
	const String& memberKey(int index) const { return m_memberList[index].name; }

protected:
	JsonObject(JsonDocument2* ownerDoc);
	virtual ~JsonObject();
	virtual void onSave(JsonWriter* writer) override;
	virtual JsonParseResult onLoad(JsonReader* reader) override;

LN_INTERNAL_ACCESS:
	void clear() { m_memberList.clear(); }

private:
	JsonValue* getValue(const StringRef& name);

	struct Member
	{
		String			name;
		JsonElement*	value;
	};

	List<Member>	m_memberList;

	friend class JsonDocument2;
};

namespace detail {

class JsonHelper
{
public:
    static bool isValueType(JsonNode type);
    static bool isValueType(JsonElementType type);
    static JsonParseResult loadElement(JsonDocument2* doc, JsonReader* reader, JsonElement** outElement);
};

class JsonElementCache
{
public:
	void initialize();
	void finalize();
	JsonElement* alloc(size_t size);

private:
	static const size_t BufferSize = 2048;

	struct BufferInfo
	{
		ByteBuffer	buffer;
		size_t		used;
	};
	List<BufferInfo>	m_buffers;
	List<JsonElement*>	m_elements;
};

} // namespace detail


/**
	@brief	JSON データのルート要素です。
*/
class JsonDocument2
	: public JsonObject
	//, public ISerializationeStore
{
public:
	JsonDocument2();
	virtual ~JsonDocument2();

	///*
	//	@brief	指定した JSON 形式文字列を解析し、ドキュメントを構築します。
	//*/
	void parse(const String& text);

	///*
	//	@brief	指定した JSON 形式文字列を解析し、ドキュメントを構築します。
	//*/
	//void Parse(const TCHAR* text, int len = -1);

	///*
	//	@brief	指定した TextReader から JSON 形式文字列を解析し、ドキュメントを構築します。
	//*/
	//void Parse(TextReader* textReader);

	//void Load();

	void save(const StringRef& filePath, JsonFormatting formatting = JsonFormatting::None);

	void load(const StringRef& filePath);


	String toString(JsonFormatting formatting = JsonFormatting::None);

protected:
	//virtual ISerializeElement* getRootObject() override;

LN_INTERNAL_ACCESS:
	template<class T>
	T* newElement()
	{
		T* buf = static_cast<T*>(m_cache.alloc(sizeof(T)));
		new (buf)T(this);
		return buf;
	}

private:
	void parseInternal(JsonReader* reader);

	detail::JsonElementCache	m_cache;
};

} // namespace tr
} // namespace ln
