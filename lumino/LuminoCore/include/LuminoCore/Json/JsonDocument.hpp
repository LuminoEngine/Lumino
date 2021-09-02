// Copyright (c) 2018+ lriki. Distributed under the MIT license.
#pragma once
#include "Common.hpp"
#include "../Base/String.hpp"

namespace ln {
class JsonWriter;
class JsonReader;
class JsonElement;
class JsonArray;
class JsonObject;
class JsonDocument;
namespace detail {
class JsonElementCache;
class JsonHelper;
class LinearAllocator;
class LinearAllocatorPageManager;
}

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
    JsonDocument* ownerDocument() const { return m_ownerDoc; }

protected:
    JsonElement(JsonDocument* owner);
    virtual ~JsonElement();
    virtual void onSave(JsonWriter* writer) = 0;
    virtual JsonParseResult onLoad(JsonReader* reader) = 0;
    void setType(JsonElementType type) { m_type = type; }

private:
    void save(JsonWriter* writer) { onSave(writer); }
    JsonParseResult load(JsonReader* reader) { return onLoad(reader); }

    JsonDocument* m_ownerDoc;
    JsonElementType m_type;

    friend class JsonObject;
    friend class JsonArray;
    friend class JsonDocument;
    friend class detail::JsonElementCache;
    friend class detail::JsonHelper;
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
    JsonValue(JsonDocument* ownerDoc);
    virtual ~JsonValue();
    void checkRelease();

    virtual void onSave(JsonWriter* writer) override;
    virtual JsonParseResult onLoad(JsonReader* reader) override;

    union
    {
        bool m_bool;
        int32_t m_int32;
        int64_t m_int64;
        float m_float;
        double m_double;
        String* m_string;
    };

    friend class JsonDocument;
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
    JsonArray(JsonDocument* ownerDoc);
    virtual ~JsonArray();
    virtual void onSave(JsonWriter* writer) override;
    virtual JsonParseResult onLoad(JsonReader* reader) override;

    List<JsonElement*> m_itemList;

    friend class JsonDocument;
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
    JsonObject(JsonDocument* ownerDoc);
    virtual ~JsonObject();
    virtual void onSave(JsonWriter* writer) override;
    virtual JsonParseResult onLoad(JsonReader* reader) override;

private:
    void clear() { m_memberList.clear(); }
    JsonValue* getValue(const StringRef& name);

    struct Member
    {
        String name;
        JsonElement* value;
    };

    List<Member> m_memberList;

    friend class JsonDocument;
};

namespace detail {

class JsonHelper
{
public:
    static bool isValueType(JsonNode type);
    static bool isValueType(JsonElementType type);
    static JsonParseResult loadElement(JsonDocument* doc, JsonReader* reader, JsonElement** outElement);
};

class JsonElementCache
{
public:
    void initialize();
    void finalize();
    JsonElement* alloc(size_t size);

private:
    //static const size_t BufferSize = 2048;

    //struct BufferInfo
    //{
    //    ByteBuffer buffer;
    //    size_t used;
    //};
    //List<BufferInfo> m_buffers;
    List<JsonElement*> m_elements;
	Ref<LinearAllocatorPageManager> m_linearAllocatorPageManager;
	Ref<LinearAllocator> m_linearAllocator;
};

} // namespace detail

/** JSON ドキュメントのルート要素です。 */
class JsonDocument
    : public RefObject
{
public:
    JsonDocument();
    virtual ~JsonDocument();

    /** 指定した JSON 形式文字列を解析し、ドキュメントを構築します。 */
    void parse(const String& text);

    /** ルート要素を取得します。 */
    JsonElement* rootElement() const { return m_rootElement; }

    /** ルート要素として新しい JsonArray を設定します。 */
    void setRootArray();

    /** ルート要素として新しい JsonObject を設定します。 */
    void setRootObject();

    /** JSON 文字列としてファイルに保存します。 */
    void save(const StringRef& filePath, JsonFormatting formatting = JsonFormatting::None);

    /** JSON ファイルを読み込み、ドキュメントを構築します。 */
    void load(const StringRef& filePath);

    /** JSON 文字列を生成します。 */
    String toString(JsonFormatting formatting = JsonFormatting::None);

private:
    template<class T>
    T* newElement()
    {
        T* buf = static_cast<T*>(m_cache.alloc(sizeof(T)));
        new (buf) T(this);
        return buf;
    }

    void parseInternal(JsonReader* reader);

    detail::JsonElementCache m_cache;
    JsonElement* m_rootElement;

    friend class JsonObject;
    friend class JsonArray;
    friend class detail::JsonElementCache;
    friend class detail::JsonHelper;
};

} // namespace ln
