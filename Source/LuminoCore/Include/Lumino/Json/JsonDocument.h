
#pragma once
#include "../Base/Serialization.h"
#include "../IO/TextReader.h"
#include "Common.h"
#include "JsonValue.h"

LN_NAMESPACE_BEGIN
namespace tr {
class JsonWriter;
class JsonReader2;
class JsonElement2;
class JsonObject2;
class JsonDocument2;
namespace detail { class JsonElementCache; }

/**
	@brief	JSON データのルート要素です。
*/
class JsonDocument
	: public JsonValue
{
public:

	/*
		@brief	指定した JSON 形式文字列を解析し、ドキュメントを構築します。
	*/
	void Parse(const String& text);

	/*
		@brief	指定した JSON 形式文字列を解析し、ドキュメントを構築します。
	*/
	void Parse(const TCHAR* text, int len = -1);

	/*
		@brief	指定した TextReader から JSON 形式文字列を解析し、ドキュメントを構築します。
	*/
	void Parse(TextReader* textReader);

	//void Load();

private:
};






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
	static bool IsValueType(JsonToken type);
	static bool IsValueType(JsonValueType type);
	static JsonParseResult LoadElement(JsonDocument2* doc, JsonReader2* reader, JsonElement2** outElement);

};
} // namespace detail

/**
	@brief	
*/
class JsonElement2
	: public RefObject
	, public ISerializeElement
{
public:
	JsonValueType getType() const { return m_type; }

protected:
	JsonElement2(JsonDocument2* owner);
	virtual ~JsonElement2();
	virtual void OnSave(JsonWriter* writer) = 0;
	virtual JsonParseResult OnLoad(JsonReader2* reader) = 0;

	// ISerializeElement interface
	virtual void setValueInt32(const StringRef& name, int32_t value) override {}
	virtual void setValueString(const StringRef& name, const String& value) override {}
	virtual ISerializeElement* addObject(const StringRef& name) override { return nullptr; }
	virtual bool tryGetValueInt32(const StringRef& name, int32_t* outValue) override { return false; }
	virtual bool tryGetValueString(const StringRef& name, String* outValue) override { return false; }
	virtual bool tryGetObject(const StringRef& name, ISerializeElement** outValue) override { return false; }
	virtual bool tryGetArray(const StringRef& name, ISerializeElement** outValue) override { return false; }
	virtual int getSerializeElementCount() const override { return 0; }
	virtual ISerializeElement* getSerializeElement(int index) const override { return nullptr; }
	virtual const String& getSerializeElementName(int index) const override { return String::getEmpty(); }
	virtual SerializationValueType getSerializationValueType() const override;
	virtual bool getSerializeValueBool() const override { return false; }
	virtual int8_t getSerializeValueInt8() const override { return 0; }
	virtual int16_t getSerializeValueInt16() const override { return 0; }
	virtual int32_t getSerializeValueInt32() const override { return 0; }
	virtual int64_t getSerializeValueInt64() const override { return 0; }
	virtual uint8_t getSerializeValueUInt8() const override { return 0; }
	virtual uint16_t getSerializeValueUInt16() const override { return 0; }
	virtual uint32_t getSerializeValueUInt32() const override { return 0; }
	virtual uint64_t getSerializeValueUInt64() const override { return 0; }
	virtual float getSerializeValueFloat() const override { return 0; }
	virtual double getSerializeValueDouble() const override { return 0; }
	virtual String getSerializeValueString() const override { return String::getEmpty(); }
	virtual void addSerializeItemValue(SerializationValueType type, const void* value) override {}
	virtual ISerializeElement* addSerializeItemNewArray() override { return nullptr; }
	virtual ISerializeElement* addSerializeItemNewObject() override { return nullptr; }
	virtual ISerializeElement* findSerializeElement(const StringRef& name) const override { return nullptr; }
	virtual void addSerializeMemberValue(const StringRef& name, SerializationValueType type, const void* value) override {}
	virtual ISerializeElement* addSerializeMemberNewArray(const StringRef& name) override { return nullptr; }
	virtual ISerializeElement* addSerializeMemberNewObject(const StringRef& name) override { return nullptr; }

LN_INTERNAL_ACCESS:
	JsonDocument2* GetOwnerDocument() const { return m_ownerDoc; }
	void SetType(JsonValueType type) { m_type = type; }
	void save(JsonWriter* writer) { OnSave(writer); }
	JsonParseResult load(JsonReader2* reader) { return OnLoad(reader); }

private:
	JsonDocument2*	m_ownerDoc;
	JsonValueType	m_type;

	friend class JsonObject2;
	friend class detail::JsonElementCache;
};

/**
	@brief	
*/
class JsonValue2
	: public JsonElement2
{
public:
	void SetNull();
	void SetBool(bool value);
	void SetInt32(int32_t value);
	void SetInt64(int64_t value);
	void SetFloat(float value);
	void SetDouble(double value);
	void setString(const StringRef& value);

	bool isNull() const;
	bool GetBool() const;
	int32_t GetInt32() const;
	int64_t GetInt64() const;
	float GetFloat() const;
	double GetDouble() const;
	String getString() const;

protected:
	// ISerializeElement interface
	virtual SerializationElementType getSerializationElementType() const { return SerializationElementType::Value; }
	virtual bool getSerializeValueBool() const override { return GetBool(); }
	virtual int8_t getSerializeValueInt8() const override { return GetInt32(); }
	virtual int16_t getSerializeValueInt16() const override { return GetInt32(); }
	virtual int32_t getSerializeValueInt32() const override { return GetInt32(); }
	virtual int64_t getSerializeValueInt64() const override { return GetInt64(); }
	virtual uint8_t getSerializeValueUInt8() const override { return GetInt32(); }
	virtual uint16_t getSerializeValueUInt16() const override { return GetInt32(); }
	virtual uint32_t getSerializeValueUInt32() const override { return GetInt32(); }
	virtual uint64_t getSerializeValueUInt64() const override { return GetInt64();; }
	virtual float getSerializeValueFloat() const override { return GetFloat(); }
	virtual double getSerializeValueDouble() const override { return GetDouble(); }
	virtual String getSerializeValueString() const override { return getString(); }

private:
	JsonValue2(JsonDocument2* ownerDoc);
	virtual ~JsonValue2();
	void CheckRelease();

	virtual void OnSave(JsonWriter* writer) override;
	virtual JsonParseResult OnLoad(JsonReader2* reader) override;

	union
	{
		bool	m_bool;
		int32_t	m_int32;
		int64_t	m_int64;
		float	m_float;
		double	m_double;
		ln::detail::GenericStringCore<TCHAR>*	m_stringCore;
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

	void AddNull();
	void AddBool(bool value);
	void AddInt32(int32_t value);
	void AddInt64(int64_t value);
	void AddFloat(float value);
	void AddDouble(double value);
	void AddString(const StringRef& value);

	JsonArray2* AddArray();
	JsonObject2* addObject();

	int GetElementCount() const { return m_itemList.getCount(); }
	JsonElement2* GetElement(int index) const { return m_itemList.getAt(index); }

protected:
	// ISerializeElement interface
	virtual SerializationElementType getSerializationElementType() const override { return SerializationElementType::Array; }
	virtual int getSerializeElementCount() const override { return GetElementCount(); }
	virtual ISerializeElement* getSerializeElement(int index) const override { return GetElement(index); }
	virtual void addSerializeItemValue(SerializationValueType type, const void* value) override;
	virtual ISerializeElement* addSerializeItemNewArray() override;
	virtual ISerializeElement* addSerializeItemNewObject() override;

private:
	JsonArray2(JsonDocument2* ownerDoc);
	virtual ~JsonArray2();
	virtual void OnSave(JsonWriter* writer) override;
	virtual JsonParseResult OnLoad(JsonReader2* reader) override;

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

	void AddMemberNull(const StringRef& name);
	void AddMemberBool(const StringRef& name, bool value);
	void AddMemberInt32(const StringRef& name, int32_t value);
	void AddMemberInt64(const StringRef& name, int64_t value);
	void AddMemberFloat(const StringRef& name, float value);
	void AddMemberDouble(const StringRef& name, double value);
	void AddMemberString(const StringRef& name, const StringRef& value);

	JsonArray2* AddMemberArray(const StringRef& name);
	JsonObject2* AddMemberObject(const StringRef& name);

	JsonElement2* find(const StringRef& name) const;

protected:
	JsonObject2(JsonDocument2* ownerDoc);
	virtual ~JsonObject2();
	virtual void OnSave(JsonWriter* writer) override;
	virtual JsonParseResult OnLoad(JsonReader2* reader) override;

	// ISerializElement interface
	virtual SerializationElementType getSerializationElementType() const { return SerializationElementType::Object; }
	virtual void setValueInt32(const StringRef& name, int32_t value) override;
	virtual void setValueString(const StringRef& name, const String& value) override;
	virtual ISerializeElement* addObject(const StringRef& name) override;
	virtual bool tryGetValueInt32(const StringRef& name, int32_t* outValue) override;
	virtual bool tryGetValueString(const StringRef& name, String* outValue) override;
	virtual bool tryGetObject(const StringRef& name, ISerializeElement** outValue) override;
	virtual bool tryGetArray(const StringRef& name, ISerializeElement** outValue) override;
	virtual const String& getSerializeElementName(int index) const override;
	virtual ISerializeElement* findSerializeElement(const StringRef& name) const override;
	virtual void addSerializeMemberValue(const StringRef& name, SerializationValueType type, const void* value) override;
	virtual ISerializeElement* addSerializeMemberNewArray(const StringRef& name) override;
	virtual ISerializeElement* addSerializeMemberNewObject(const StringRef& name) override;

LN_INTERNAL_ACCESS:
	void Finalize() { m_memberList.clear(); }

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
	void Finalize();
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
	, public ISerializationeStore
{
public:
	JsonDocument2();
	virtual ~JsonDocument2();

	///*
	//	@brief	指定した JSON 形式文字列を解析し、ドキュメントを構築します。
	//*/
	void Parse(const String& text);

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
	virtual ISerializeElement* getRootObject() override;

LN_INTERNAL_ACCESS:
	template<class T>
	T* NewElement()
	{
		T* buf = static_cast<T*>(m_cache.alloc(sizeof(T)));
		new (buf)T(this);
		return buf;
	}

private:
	void ParseInternal(JsonReader2* reader);

	detail::JsonElementCache	m_cache;
};



class JsonSerializer
{
public:
	template<typename T>
	static String save(T& value)
	{
		tr::JsonDocument2 doc;
		tr::Archive ar(&doc, tr::ArchiveMode::save, true);
		ar.save(value);
		return doc.toString();
	}

	template<typename T>
	static T load(const StringRef& json)
	{
		tr::JsonDocument2 doc;
		doc.Parse(json);
		tr::Archive ar(&doc, tr::ArchiveMode::load, true);
		T t;
		ar.load(t);
		return t;
	}

	template<typename T>
	static RefPtr<T> LoadObject(const StringRef& json)
	{
		tr::JsonDocument2 doc;
		doc.Parse(json);
		tr::Archive ar(&doc, tr::ArchiveMode::load, true);
		auto t = NewObject<T>();
		ar.load(t);
		return t;
	}

	
};

} // namespace tr
LN_NAMESPACE_END
