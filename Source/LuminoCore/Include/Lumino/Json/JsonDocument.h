
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
	JsonValueType GetType() const { return m_type; }

protected:
	JsonElement2(JsonDocument2* owner);
	virtual ~JsonElement2();
	virtual void OnSave(JsonWriter* writer) = 0;
	virtual JsonParseResult OnLoad(JsonReader2* reader) = 0;

	// ISerializeElement interface
	virtual void SetValueInt32(const StringRef& name, int32_t value) override {}
	virtual void SetValueString(const StringRef& name, const String& value) override {}
	virtual ISerializeElement* AddObject(const StringRef& name) override { return nullptr; }
	virtual bool TryGetValueInt32(const StringRef& name, int32_t* outValue) override { return false; }
	virtual bool TryGetValueString(const StringRef& name, String* outValue) override { return false; }
	virtual bool TryGetObject(const StringRef& name, ISerializeElement** outValue) override { return false; }
	virtual bool TryGetArray(const StringRef& name, ISerializeElement** outValue) override { return false; }
	virtual int GetSerializeElementCount() const override { return 0; }
	virtual ISerializeElement* GetSerializeElement(int index) const override { return nullptr; }
	virtual const String& GetSerializeElementName(int index) const { return String::GetEmpty(); }
	virtual SerializationValueType GetSerializationValueType() const override { return SerializationValueType::Null; }
	virtual bool GetSerializeValueBool() const override { return false; }
	virtual int8_t GetSerializeValueInt8() const override { return 0; }
	virtual int16_t GetSerializeValueInt16() const override { return 0; }
	virtual int32_t GetSerializeValueInt32() const override { return 0; }
	virtual int64_t GetSerializeValueInt64() const override { return 0; }
	virtual uint8_t GetSerializeValueUInt8() const override { return 0; }
	virtual uint16_t GetSerializeValueUInt16() const override { return 0; }
	virtual uint32_t GetSerializeValueUInt32() const override { return 0; }
	virtual uint64_t GetSerializeValueUInt64() const override { return 0; }
	virtual float GetSerializeValueFloat() const override { return 0; }
	virtual double GetSerializeValueDouble() const override { return 0; }
	virtual String GetSerializeValueString() const override { return String::GetEmpty(); }
	virtual void AddSerializeItemValue(SerializationValueType type, const void* value) override {}
	virtual ISerializeElement* AddSerializeItemNewArray() override { return nullptr; }
	virtual ISerializeElement* AddSerializeItemNewObject() override { return nullptr; }
	virtual ISerializeElement* FindSerializeElement(const StringRef& name) const override { return nullptr; }
	virtual void AddSerializeMemberValue(const StringRef& name, SerializationValueType type, const void* value) override {}
	virtual ISerializeElement* AddSerializeMemberNewArray(const StringRef& name) override { return nullptr; }
	virtual ISerializeElement* AddSerializeMemberNewObject(const StringRef& name) override { return nullptr; }

LN_INTERNAL_ACCESS:
	JsonDocument2* GetOwnerDocument() const { return m_ownerDoc; }
	void SetType(JsonValueType type) { m_type = type; }
	void Save(JsonWriter* writer) { OnSave(writer); }
	JsonParseResult Load(JsonReader2* reader) { return OnLoad(reader); }

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
	void SetString(const StringRef& value);

	bool IsNull() const;
	bool GetBool() const;
	int32_t GetInt32() const;
	int64_t GetInt64() const;
	float GetFloat() const;
	double GetDouble() const;
	String GetString() const;

protected:
	// ISerializeElement interface
	virtual SerializationElementType GetSerializationElementType() const { return SerializationElementType::Value; }
	virtual SerializationValueType GetSerializationValueType() const override;
	virtual bool GetSerializeValueBool() const override { return GetBool(); }
	virtual int8_t GetSerializeValueInt8() const override { return GetInt32(); }
	virtual int16_t GetSerializeValueInt16() const override { return GetInt32(); }
	virtual int32_t GetSerializeValueInt32() const override { return GetInt32(); }
	virtual int64_t GetSerializeValueInt64() const override { return GetInt64(); }
	virtual uint8_t GetSerializeValueUInt8() const override { return GetInt32(); }
	virtual uint16_t GetSerializeValueUInt16() const override { return GetInt32(); }
	virtual uint32_t GetSerializeValueUInt32() const override { return GetInt32(); }
	virtual uint64_t GetSerializeValueUInt64() const override { return GetInt64();; }
	virtual float GetSerializeValueFloat() const override { return GetFloat(); }
	virtual double GetSerializeValueDouble() const override { return GetDouble(); }
	virtual String GetSerializeValueString() const override { return GetString(); }

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
	JsonObject2* AddObject();

	int GetElementCount() const { return m_itemList.GetCount(); }
	JsonElement2* GetElement(int index) const { return m_itemList.GetAt(index); }

protected:
	// ISerializeElement interface
	virtual SerializationElementType GetSerializationElementType() const override { return SerializationElementType::Array; }
	virtual int GetSerializeElementCount() const override { return GetElementCount(); }
	virtual ISerializeElement* GetSerializeElement(int index) const override { return GetElement(index); }
	virtual void AddSerializeItemValue(SerializationValueType type, const void* value) override;
	virtual ISerializeElement* AddSerializeItemNewArray() override;
	virtual ISerializeElement* AddSerializeItemNewObject() override;

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

	JsonElement2* Find(const StringRef& name) const;

protected:
	JsonObject2(JsonDocument2* ownerDoc);
	virtual ~JsonObject2();
	virtual void OnSave(JsonWriter* writer) override;
	virtual JsonParseResult OnLoad(JsonReader2* reader) override;

	// ISerializElement interface
	virtual SerializationElementType GetSerializationElementType() const { return SerializationElementType::Object; }
	virtual void SetValueInt32(const StringRef& name, int32_t value) override;
	virtual void SetValueString(const StringRef& name, const String& value) override;
	virtual ISerializeElement* AddObject(const StringRef& name) override;
	virtual bool TryGetValueInt32(const StringRef& name, int32_t* outValue) override;
	virtual bool TryGetValueString(const StringRef& name, String* outValue) override;
	virtual bool TryGetObject(const StringRef& name, ISerializeElement** outValue) override;
	virtual bool TryGetArray(const StringRef& name, ISerializeElement** outValue) override;
	virtual const String& GetSerializeElementName(int index) const override;
	virtual ISerializeElement* FindSerializeElement(const StringRef& name) const override;
	virtual void AddSerializeMemberValue(const StringRef& name, SerializationValueType type, const void* value) override;
	virtual ISerializeElement* AddSerializeMemberNewArray(const StringRef& name) override;
	virtual ISerializeElement* AddSerializeMemberNewObject(const StringRef& name) override;

LN_INTERNAL_ACCESS:
	void Finalize() { m_memberList.Clear(); }

private:
	JsonValue2* GetValue(const StringRef& name);

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
	void Initialize();
	void Finalize();
	JsonElement2* Alloc(size_t size);

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

	void Save(const StringRef& filePath, JsonFormatting formatting = JsonFormatting::None);

	void Load(const StringRef& filePath);


	String ToString(JsonFormatting formatting = JsonFormatting::None);

protected:
	virtual ISerializeElement* GetRootObject() override;

LN_INTERNAL_ACCESS:
	template<class T>
	T* NewElement()
	{
		T* buf = static_cast<T*>(m_cache.Alloc(sizeof(T)));
		new (buf)T(this);
		return buf;
	}

private:
	void ParseInternal(JsonReader2* reader);

	detail::JsonElementCache	m_cache;
};




} // namespace tr
LN_NAMESPACE_END
