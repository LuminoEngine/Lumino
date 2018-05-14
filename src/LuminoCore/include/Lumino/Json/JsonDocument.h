
#pragma once
//#include "../Base/Serialization.h"
#include "../IO/TextReader.hpp"
#include "Common.hpp"

namespace ln {
class JsonWriter;
class JsonReader;
namespace tr {
class JsonElement2;
class JsonObject2;
class JsonDocument2;
namespace detail { class JsonElementCache; }

//// JSON データのルート要素です。
//class JsonDocument
//	: public JsonValue
//{
//public:
//
//	/*
//		@brief	指定した JSON 形式文字列を解析し、ドキュメントを構築します。
//	*/
//	void parse(const String& text);
//
//	/*
//		@brief	指定した JSON 形式文字列を解析し、ドキュメントを構築します。
//	*/
//	void parse(const Char* text, int len = -1);
//
//	/*
//		@brief	指定した TextReader から JSON 形式文字列を解析し、ドキュメントを構築します。
//	*/
//	void parse(TextReader* textReader);
//
//	//void Load();
//
//private:
//};
//
//




/** Json の値の型を示します。*/
enum class JsonValueType
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

namespace detail {
class JsonHelper
{
public:
	static bool isValueType(JsonNode type);
	static bool isValueType(JsonValueType type);
	static JsonParseResult loadElement(JsonDocument2* doc, JsonReader* reader, JsonElement2** outElement);

};
} // namespace detail

/**
	@brief	
*/
class JsonElement2
	: public RefObject
	//, public ISerializeElement
{
public:
	JsonValueType getType() const { return m_type; }

protected:
	JsonElement2(JsonDocument2* owner);
	virtual ~JsonElement2();
	virtual void onSave(JsonWriter* writer) = 0;
	virtual JsonParseResult onLoad(JsonReader* reader) = 0;

	// ISerializeElement interface
	//virtual void setValueString(const StringRef& name, const String& value) override {}
	//virtual ISerializeElement* addObject(const StringRef& name) override { return nullptr; }
	//virtual bool tryGetValueInt32(const StringRef& name, int32_t* outValue) override { return false; }
	//virtual bool tryGetValueString(const StringRef& name, String* outValue) override { return false; }
	//virtual bool tryGetObject(const StringRef& name, ISerializeElement** outValue) override { return false; }
	//virtual bool tryGetArray(const StringRef& name, ISerializeElement** outValue) override { return false; }
	//virtual int getSerializeElementCount() const override { return 0; }
	//virtual ISerializeElement* getSerializeElement(int index) const override { return nullptr; }
	//virtual const String& getSerializeElementName(int index) const override { return String::getEmpty(); }
	//virtual SerializationValueType getSerializationValueType() const override;
	//virtual bool getSerializeValueBool() const override { return false; }
	//virtual int8_t getSerializeValueInt8() const override { return 0; }
	//virtual int16_t getSerializeValueInt16() const override { return 0; }
	//virtual int32_t getSerializeValueInt32() const override { return 0; }
	//virtual int64_t getSerializeValueInt64() const override { return 0; }
	//virtual uint8_t getSerializeValueUInt8() const override { return 0; }
	//virtual uint16_t getSerializeValueUInt16() const override { return 0; }
	//virtual uint32_t getSerializeValueUInt32() const override { return 0; }
	//virtual uint64_t getSerializeValueUInt64() const override { return 0; }
	//virtual float getSerializeValueFloat() const override { return 0; }
	//virtual double getSerializeValueDouble() const override { return 0; }
	//virtual String getSerializeValueString() const override { return String::getEmpty(); }
	//virtual void addSerializeItemValue(SerializationValueType type, const void* value) override {}
	//virtual ISerializeElement* addSerializeItemNewArray() override { return nullptr; }
	//virtual ISerializeElement* addSerializeItemNewObject() override { return nullptr; }
	//virtual ISerializeElement* findSerializeElement(const StringRef& name) const override { return nullptr; }
	//virtual void addSerializeMemberValue(const StringRef& name, SerializationValueType type, const void* value) override {}
	//virtual ISerializeElement* addSerializeMemberNewArray(const StringRef& name) override { return nullptr; }
	//virtual ISerializeElement* addSerializeMemberNewObject(const StringRef& name) override { return nullptr; }

LN_INTERNAL_ACCESS:
	JsonDocument2* getOwnerDocument() const { return m_ownerDoc; }
	void setType(JsonValueType type) { m_type = type; }
	void save(JsonWriter* writer) { onSave(writer); }
	JsonParseResult load(JsonReader* reader) { return onLoad(reader); }

private:
	JsonDocument2*	m_ownerDoc;
	JsonValueType	m_type;

	friend class JsonObject2;
	friend class detail::JsonElementCache;
};

//
class JsonValue2
	: public JsonElement2
{
public:
	void setNull();
	void setBool(bool value);
	void setInt32(int32_t value);
	void setInt64(int64_t value);
	void setFloat(float value);
	void setDouble(double value);
	void setString(const StringRef& value);

	bool isNull() const;
	bool getBool() const;
	int32_t getInt32() const;
	int64_t getInt64() const;
	float getFloat() const;
	double getDouble() const;
	String getString() const;

protected:
	//// ISerializeElement interface
	//virtual SerializationElementType getSerializationElementType() const override { return SerializationElementType::Value; }
	//virtual bool getSerializeValueBool() const override { return getBool(); }
	//virtual int8_t getSerializeValueInt8() const override { return getInt32(); }
	//virtual int16_t getSerializeValueInt16() const override { return getInt32(); }
	//virtual int32_t getSerializeValueInt32() const override { return getInt32(); }
	//virtual int64_t getSerializeValueInt64() const override { return getInt64(); }
	//virtual uint8_t getSerializeValueUInt8() const override { return getInt32(); }
	//virtual uint16_t getSerializeValueUInt16() const override { return getInt32(); }
	//virtual uint32_t getSerializeValueUInt32() const override { return getInt32(); }
	//virtual uint64_t getSerializeValueUInt64() const override { return getInt64();; }
	//virtual float getSerializeValueFloat() const override { return getFloat(); }
	//virtual double getSerializeValueDouble() const override { return getDouble(); }
	//virtual String getSerializeValueString() const override { return getString(); }

private:
	JsonValue2(JsonDocument2* ownerDoc);
	virtual ~JsonValue2();
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

/**
	@brief	
*/
class JsonArray2
	: public JsonElement2
{
public:

	void addNull();
	void addBool(bool value);
	void addInt32(int32_t value);
	void addInt64(int64_t value);
	void addFloat(float value);
	void addDouble(double value);
	void addString(const StringRef& value);

	JsonArray2* addArray();
	JsonObject2* addObject();

	int getElementCount() const { return m_itemList.size(); }
	JsonElement2* getElement(int index) const { return m_itemList.at(index); }

protected:
	//// ISerializeElement interface
	//virtual SerializationElementType getSerializationElementType() const override { return SerializationElementType::Array; }
	//virtual int getSerializeElementCount() const override { return getElementCount(); }
	//virtual ISerializeElement* getSerializeElement(int index) const override { return getElement(index); }
	//virtual void addSerializeItemValue(SerializationValueType type, const void* value) override;
	//virtual ISerializeElement* addSerializeItemNewArray() override;
	//virtual ISerializeElement* addSerializeItemNewObject() override;

private:
	JsonArray2(JsonDocument2* ownerDoc);
	virtual ~JsonArray2();
	virtual void onSave(JsonWriter* writer) override;
	virtual JsonParseResult onLoad(JsonReader* reader) override;

	List<JsonElement2*>	m_itemList;

	friend class JsonDocument2;
};

/**
	@brief	
*/
class JsonObject2
	: public JsonElement2
{
public:

	void addMemberNull(const StringRef& name);
	void addMemberBool(const StringRef& name, bool value);
	void addMemberInt32(const StringRef& name, int32_t value);
	void addMemberInt64(const StringRef& name, int64_t value);
	void addMemberFloat(const StringRef& name, float value);
	void addMemberDouble(const StringRef& name, double value);
	void addMemberString(const StringRef& name, const StringRef& value);

	JsonArray2* addMemberArray(const StringRef& name);
	JsonObject2* addMemberObject(const StringRef& name);

	JsonElement2* find(const StringRef& name) const;

	int size() const { return m_memberList.size(); }
	const String& getKey(int index) const { return m_memberList[index].name; }

protected:
	JsonObject2(JsonDocument2* ownerDoc);
	virtual ~JsonObject2();
	virtual void onSave(JsonWriter* writer) override;
	virtual JsonParseResult onLoad(JsonReader* reader) override;

	//// ISerializElement interface
	//virtual SerializationElementType getSerializationElementType() const override { return SerializationElementType::Object; }
	//virtual void setValueString(const StringRef& name, const String& value) override;
	//virtual ISerializeElement* addObject(const StringRef& name) override;
	//virtual bool tryGetValueInt32(const StringRef& name, int32_t* outValue) override;
	//virtual bool tryGetValueString(const StringRef& name, String* outValue) override;
	//virtual bool tryGetObject(const StringRef& name, ISerializeElement** outValue) override;
	//virtual bool tryGetArray(const StringRef& name, ISerializeElement** outValue) override;
	//virtual const String& getSerializeElementName(int index) const override;
	//virtual ISerializeElement* findSerializeElement(const StringRef& name) const override;
	//virtual void addSerializeMemberValue(const StringRef& name, SerializationValueType type, const void* value) override;
	//virtual ISerializeElement* addSerializeMemberNewArray(const StringRef& name) override;
	//virtual ISerializeElement* addSerializeMemberNewObject(const StringRef& name) override;

LN_INTERNAL_ACCESS:
	void clear() { m_memberList.clear(); }

private:
	JsonValue2* getValue(const StringRef& name);

	struct Member
	{
		String			name;
		JsonElement2*	value;
	};

	List<Member>	m_memberList;

	friend class JsonDocument2;
};

namespace detail
{

class JsonElementCache
{
public:
	void initialize();
	void finalize();
	JsonElement2* alloc(size_t size);

private:
	static const size_t BufferSize = 2048;

	struct BufferInfo
	{
		ByteBuffer	buffer;
		size_t		used;
	};
	List<BufferInfo>	m_buffers;
	List<JsonElement2*>	m_elements;
};

} // namespace detail


/**
	@brief	JSON データのルート要素です。
*/
class JsonDocument2
	: public JsonObject2
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
