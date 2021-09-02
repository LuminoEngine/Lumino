
#pragma once
#include <stack>
#include "../Math/Math.hpp"
#include "../Base/String.hpp"
#include "../Json/JsonDocument.hpp"
#include "Common.hpp"

namespace ln {
class Archive;

enum class ArchiveContainerType
{
    None,   // 最初の状態。ルートノードを open していない。
    Null,   // 便宜上、あらゆる型を表す null はコンテナとしても扱う
    Value,  // null と同じく、optional でも必要だった
	Object,
	Array,
};

enum class ArchiveNodeType
{
	Null,
	Bool,
	Int64,
	Double,
	String,
	Object,
	Array,
};

class ArchiveStore
{
public:
	ArchiveStore()
		: m_nextName()
		, m_nextIndex(0)
		//, m_readingArrayElementCount(0)
	{}

	// save のときは、次に write する値の名前。
	// load のときは、次に read する値の名前。
	void setNextName(const String& name) { m_nextName = name; }
	const String& getNextName() const { return m_nextName; }
	bool hasNextName() const { return !m_nextName.isEmpty(); }

	//-----------------------------------------------------------------------------
	// Save
	void writeObject() { onWriteObject(); m_nextName.clear(); /*m_nodeStack.push(NodeInfo{ ArchiveNodeType::Object });*/ }
	void writeArray() { onWriteArray(); m_nextName.clear(); /*m_nodeStack.push(NodeInfo{ ArchiveNodeType::Array });*/ }
	void writeObjectEnd() { onWriteObjectEnd(); m_nextName.clear(); /*m_nodeStack.pop();*/ }
	void writeArrayEnd() { onWriteArrayEnd(); m_nextName.clear(); /*m_nodeStack.pop();*/ }
	void writeValueNull() { onWriteValueNull(); m_nextName.clear(); }
	void writeValue(bool value) { onWriteValueBool(value); m_nextName.clear(); }
	void writeValue(int64_t value) { onWriteValueInt64(value); m_nextName.clear(); }
	void writeValue(double value) { onWriteValueDouble(value); m_nextName.clear(); }
	void writeValue(const String& value) { onWriteValueString(value); m_nextName.clear(); }

	//-----------------------------------------------------------------------------
	// Load
    virtual void setupLoad() {}
	ArchiveContainerType getOpendContainerType() const { return onGetContainerType(); }// { return m_nodeStack.top().m_readingNodeType; }
	int getContainerElementCount() const { return onReadContainerElementCount(); }
	//void setNextIndex(int index) { m_nextIndex = index; }	// setNextName() と同じように使う Array 版
	//int getNextIndex() const { return m_nextIndex; }
    virtual bool moveToNamedMember(const StringRef& name) = 0;
    virtual bool moveToIndexedMember(int index) = 0;
	virtual bool hasKey(const StringRef& name) const = 0;
	virtual const String& memberKey(int index) const = 0;	// open済みmapコンテナのキーを探す
	bool openContainer()
	{
		//int dummy;
		bool r = onOpenContainer(/*&dummy*/);//&m_readingArrayElementCount);
		//m_nodeStack.push(NodeInfo{});
		//m_nodeStack.top().m_readingNodeType = (m_readingArrayElementCount >= 0) ? ArchiveNodeType::Array : ArchiveNodeType::Object;
		postRead();
		return r;
	}		// Object Container をカレントにする
	void closeContainer() { onCloseContainer(); /*m_nodeStack.pop();*/ }
	bool readValue(bool* outValue) { bool r = onReadValueBool(outValue); postRead(); return r; }
	bool readValue(int64_t* outValue) { bool r = onReadValueInt64(outValue); postRead(); return r; }
	bool readValue(double* outValue) { bool r = onReadValueDouble(outValue); postRead(); return r; }
	bool readValue(String* outValue) { bool r = onReadValueString(outValue); postRead(); return r; }
	virtual ArchiveNodeType getReadingValueType() = 0;	// 次に readValue で読まれる(setNextXXXX されている)値の型を取得

protected:
	virtual ArchiveContainerType onGetContainerType() const = 0;

	virtual void onWriteObject() = 0;
	virtual void onWriteArray() = 0;
	virtual void onWriteObjectEnd() = 0;
	virtual void onWriteArrayEnd() = 0;
	virtual void onWriteValueNull() = 0;
	virtual void onWriteValueBool(bool value) = 0;
	virtual void onWriteValueInt64(int64_t value) = 0;
	virtual void onWriteValueDouble(double value) = 0;
	virtual void onWriteValueString(const String& value) = 0;

	virtual bool onOpenContainer(/*int* outElementCount*/) = 0;
	virtual bool onCloseContainer() = 0;
	virtual int onReadContainerElementCount() const = 0;
	virtual bool onReadValueBool(bool* outValue) = 0;
	virtual bool onReadValueInt64(int64_t* outValue) = 0;
	virtual bool onReadValueDouble(double* outValue) = 0;
	virtual bool onReadValueString(String* outValue) = 0;

private:
	void postRead()
	{
		m_nextName.clear();
		m_nextIndex = 0;
	}

	//struct NodeInfo
	//{
	//	ArchiveNodeType	m_readingNodeType;
	//};

	String	m_nextName;
	int		m_nextIndex;
	//int				m_readingArrayElementCount;
	//std::stack<NodeInfo>	m_nodeStack;
};

class JsonArchiveStore
	: public ArchiveStore
{
public:
	//JsonArchiveStore()
	//	: m_localDoc(Ref<JsonDocument>::makeRef())
	//{
	//	
	//}

	JsonArchiveStore(JsonDocument* doc/*, ArchiveMode mode*/)
		: m_localDoc(doc)
        , m_current(nullptr)
        , m_mode(ArchiveMode::Save)
	{
		// TODO: 今 JsonDocument はルート Array に対応していないのでこんな感じ。
		//m_nodeStack.push(doc);

        //if (mode == ArchiveMode::Load) {
        //    m_current = doc->rootElement();
        //}
	}

	~JsonArchiveStore()
	{
	}

protected:
	virtual ArchiveContainerType onGetContainerType() const override
	{
        JsonElement* currentNode = nullptr;
        if (m_mode == ArchiveMode::Save) {
            currentNode = m_nodeStack.top();
        }
        else {
            if (m_nodeStack.empty()) return ArchiveContainerType::None;
            currentNode = m_nodeStack.top();
            if (currentNode->type() == JsonElementType::Null) {
                return ArchiveContainerType::Null;
            }
            else if (currentNode->type() == JsonElementType::Object) {
                return ArchiveContainerType::Object;
            }
            else if (currentNode->type() == JsonElementType::Array) {
                return ArchiveContainerType::Array;
            }
            else {
                return ArchiveContainerType::Value;
            }
        }

		if (currentNode->type() == JsonElementType::Object)
			return ArchiveContainerType::Object;
		else if (currentNode->type() == JsonElementType::Array)
			return ArchiveContainerType::Array;
		else
			LN_UNREACHABLE();
		return ArchiveContainerType::Object;
	}

	virtual void onWriteObject() override
	{
		if (m_nodeStack.empty())
		{
			m_localDoc->setRootObject();
			m_nodeStack.push(m_localDoc->rootElement());
		}
		else if (m_nodeStack.top()->type() == JsonElementType::Object)
		{
			if (LN_REQUIRE(hasNextName())) return;
			JsonObject* v = static_cast<JsonObject*>(m_nodeStack.top())->addObject(getNextName());
			m_nodeStack.push(v);
		}
		else if (m_nodeStack.top()->type() == JsonElementType::Array)
		{
			JsonObject* v = static_cast<JsonArray*>(m_nodeStack.top())->addObject();
			m_nodeStack.push(v);
		}
		else
		{
			LN_UNREACHABLE();
		}
	}

	virtual void onWriteArray() override
	{
		if (m_nodeStack.empty())
		{
			m_localDoc->setRootArray();
			m_nodeStack.push(m_localDoc->rootElement());
		}
		else if (checkTopTypeForWrite(JsonElementType::Object))
		{
			if (LN_REQUIRE(hasNextName())) return;
			JsonArray* v = static_cast<JsonObject*>(m_nodeStack.top())->addArray(getNextName());
			m_nodeStack.push(v);
		}
		else if (checkTopTypeForWrite(JsonElementType::Array))
		{
			JsonArray* v = static_cast<JsonArray*>(m_nodeStack.top())->addArray();
			m_nodeStack.push(v);
		}
		else
		{
			LN_UNREACHABLE();
		}
	}

	virtual void onWriteObjectEnd() override
	{
		m_nodeStack.pop();
	}

	virtual void onWriteArrayEnd() override
	{
		m_nodeStack.pop();
	}

	virtual void onWriteValueNull() override
	{
		if (checkTopTypeForWrite(JsonElementType::Object)) {
			static_cast<JsonObject*>(m_nodeStack.top())->addNullValue(getNextName());
		}
	}

#define ON_WRITE_VALUE_FUNC(type, name) \
	virtual void onWriteValue##name(type value) override \
	{ \
		if (checkTopTypeForWrite(JsonElementType::Object)) \
		{ \
			if (LN_REQUIRE(hasNextName())) return; \
			static_cast<JsonObject*>(m_nodeStack.top())->add##name##Value(getNextName(), value); \
		} \
		else if (checkTopTypeForWrite(JsonElementType::Array)) \
		{ \
			static_cast<JsonArray*>(m_nodeStack.top())->add##name##Value(value); \
		} \
		else \
		{ \
			LN_UNREACHABLE(); \
		} \
	} \
	
	ON_WRITE_VALUE_FUNC(bool, Bool);
	ON_WRITE_VALUE_FUNC(int64_t, Int64);
	ON_WRITE_VALUE_FUNC(double, Double);
	ON_WRITE_VALUE_FUNC(const String&, String);

#undef ON_WRITE_VALUE_FUNC

    //-------------------------------------------------------------------------------
    // load

    virtual void setupLoad() override
    {
        m_current = m_localDoc->rootElement();
        m_mode = ArchiveMode::Load;
    }

    virtual bool moveToNamedMember(const StringRef& name) override
    {
        if (LN_REQUIRE(!m_nodeStack.empty())) return false;
        if (LN_REQUIRE(m_nodeStack.top()->type() == JsonElementType::Object)) return false;

        auto* newCurrent = static_cast<JsonObject*>(m_nodeStack.top())->find(name);
        if (LN_REQUIRE(newCurrent)) return false;

        m_current = newCurrent;
        return true;
    }

    virtual bool moveToIndexedMember(int index) override
    {
        if (LN_REQUIRE(!m_nodeStack.empty())) return false;
        if (LN_REQUIRE(m_nodeStack.top()->type() == JsonElementType::Array)) return false;

        auto* newCurrent = static_cast<JsonArray*>(m_nodeStack.top())->element(index);
        if (LN_REQUIRE(newCurrent)) return false;

        m_current = newCurrent;
        return true;
    }

	virtual bool hasKey(const StringRef& name) const override
	{
        if (LN_REQUIRE(!m_nodeStack.empty())) return false;
        if (LN_REQUIRE(m_nodeStack.top()->type() == JsonElementType::Object)) return false;

        return static_cast<JsonObject*>(m_nodeStack.top())->find(name) != nullptr;


		//if (checkTopType(JsonElementType::Object))
		//{
		//	return static_cast<JsonObject*>(current())->find(name) != nullptr;
		//}
		//else
		//{
		//	LN_NOTIMPLEMENTED();
		//	return false;
		//}
	}

	virtual const String& memberKey(int index) const override
	{
		if (checkCurrentContainerType(JsonElementType::Object))
		{
			return static_cast<JsonObject*>(currentContainer())->memberKey(index);
		}
		else
		{
			LN_NOTIMPLEMENTED();
			static const String empty;
			return empty;
		}
	}

	virtual bool onOpenContainer(/*int* outElementCount*/) override
	{
		//JsonElement* element = nullptr;
		//if (m_nodeStack.empty())
		//{
		//	element = m_localDoc->rootElement();
		//	if (!element) return false;
		//	m_nodeStack.push(element);
		//}
		//else if (checkTopType(JsonElementType::Object))
		//{
		//	if (LN_REQUIRE(hasNextName())) return false;
		//	element = static_cast<JsonObject*>(current())->find(getNextName());
		//	if (!element) return false;
		//	m_nodeStack.push(element);
		//}
		//else if (checkTopType(JsonElementType::Array))
		//{
		//	element = static_cast<JsonArray*>(current())->element(getNextIndex());
		//	m_nodeStack.push(element);
		//}
		//else
		//{
		//	LN_NOTIMPLEMENTED();
		//}

        //JsonElement* newCurrent = nullptr;
        //if (checkTopType(JsonElementType::Object)) {
        //    if (LN_REQUIRE(hasNextName())) return false;
        //    newCurrent = static_cast<JsonObject*>(current())->find(getNextName());
        //    if (!newCurrent) return false;
        //}
        //else if (checkTopType(JsonElementType::Array)) {
        //    newCurrent = static_cast<JsonArray*>(current())->element(getNextIndex());
        //}
        //else {

        //}

        if (LN_REQUIRE(m_current)) return false;

        m_nodeStack.push(m_current);
        m_current = nullptr;

		//if (m_current->type() == JsonElementType::Object)
		//{
		//	*outElementCount = -1;
		//}
		//else if (m_current->type() == JsonElementType::Array)
		//{
		//	*outElementCount = static_cast<JsonArray*>(m_current)->elementCount();
		//}
		//else
		//{
		//	LN_NOTIMPLEMENTED();
		//}

		return true;
	}

	virtual bool onCloseContainer() override
	{
        if (LN_REQUIRE(!m_nodeStack.empty())) return false;
        m_current = m_nodeStack.top();
		m_nodeStack.pop();
        return true;
	}

	virtual int onReadContainerElementCount() const override
	{
        if (LN_REQUIRE(!m_nodeStack.empty())) return false;
		if (m_nodeStack.top()->type() == JsonElementType::Object)
			return static_cast<JsonObject*>(m_nodeStack.top())->memberCount();
		else if (m_nodeStack.top()->type() == JsonElementType::Array)
			return static_cast<JsonArray*>(m_nodeStack.top())->elementCount();
		else
			LN_UNREACHABLE();
		return 0;
	}

	JsonValue* readValueHelper()
	{
        if (LN_REQUIRE(current()->type() != JsonElementType::Object)) return nullptr;
        if (LN_REQUIRE(current()->type() != JsonElementType::Array)) return nullptr;
        return static_cast<JsonValue*>(current());
		//if (checkTopType(JsonElementType::Object))
		//{
		//	if (LN_REQUIRE(hasNextName())) return nullptr;
		//	JsonElement* v = static_cast<JsonObject*>(current())->find(getNextName());
		//	return static_cast<JsonValue*>(v);
		//	//if (LN_ENSURE(v->type() == JsonElementType::internalName)) return;
		//	//*outValue = static_cast<type>(static_cast<JsonValue*>(v)->get##internalName());
		//}
		//else if (checkTopType(JsonElementType::Array))
		//{
		//	JsonElement* v = static_cast<JsonArray*>(current())->element(getNextIndex());
		//	//if (LN_ENSURE(v->type() == JsonElementType::internalName)) return;
		//	return static_cast<JsonValue*>(v);
		//	//*outValue = static_cast<type>(static_cast<JsonValue*>(v)->get##internalName());
		//}
		//else
		//{
		//	LN_NOTIMPLEMENTED();
		//	return nullptr;
		//}

	}

	virtual bool onReadValueBool(bool* outValue) override
	{
		JsonValue* v = readValueHelper();
		if (!v) return false;

		if (v->type() == JsonElementType::Bool)
			*outValue = v->boolValue();
		else
			LN_UNREACHABLE();

		return true;
	}

	virtual bool onReadValueInt64(int64_t* outValue) override
	{
		JsonValue* v = readValueHelper();
		if (!v) return false;

		if (v->type() == JsonElementType::Int32)
			*outValue = v->int32Value();
		else if (v->type() == JsonElementType::Int64)
			*outValue = v->int64Value();
		else
			LN_UNREACHABLE();

		return true;
	}

	virtual bool onReadValueDouble(double* outValue) override
	{
		JsonValue* v = readValueHelper();
		if (!v) return false;

		if (v->type() == JsonElementType::Float)
			*outValue = v->floatValue();
		else if (v->type() == JsonElementType::Double)
			*outValue = v->doubleValue();
        else if (v->type() == JsonElementType::String)
            if (String::compare(v->stringValue(), u"NaN", CaseSensitivity::CaseInsensitive) == 0) {
                *outValue = Math::NaN;
            }
            else if (String::compare(v->stringValue(), u"Inf", CaseSensitivity::CaseInsensitive) == 0) {
                *outValue = Math::Inf;
            }
            else {
                // TODO: Error
                LN_NOTIMPLEMENTED();
            }
		else
			LN_UNREACHABLE();

		return true;
	}

	virtual bool onReadValueString(String* outValue) override
	{
		JsonValue* v = readValueHelper();
		if (!v) return false;

		if (v->type() == JsonElementType::String)
			*outValue = v->stringValue();
		else
			LN_UNREACHABLE();

		return true;
	}

	virtual ArchiveNodeType getReadingValueType() override
	{
		//JsonValue* v = readValueHelper();
		//if (!v) return ArchiveNodeType::Null;

		switch (current()->type())
		{
		case JsonElementType::Null: return ArchiveNodeType::Null;
		case JsonElementType::Bool: return ArchiveNodeType::Bool;
		case JsonElementType::Int32: return ArchiveNodeType::Int64;
		case JsonElementType::Int64: return ArchiveNodeType::Int64;
		case JsonElementType::Float: return ArchiveNodeType::Double;
		case JsonElementType::Double: return ArchiveNodeType::Double;
		case JsonElementType::String: return ArchiveNodeType::String;
		case JsonElementType::Array: return ArchiveNodeType::Array;
		case JsonElementType::Object: return ArchiveNodeType::Object;
		default: return ArchiveNodeType::Null;
		}
	}

private:
	JsonElement* currentContainer() const { return m_nodeStack.top(); }
    JsonElement* current() const { return m_current; }
    bool checkTopTypeForWrite(JsonElementType t) const { return m_nodeStack.top()->type() == t; }
	bool checkCurrentContainerType(JsonElementType t) const { return m_nodeStack.top()->type() == t; }
	bool checkTopType(JsonElementType t) const { return current()->type() == t; }

	std::stack<JsonElement*>	m_nodeStack;	// Value 型は積まれない。コンテナのみ。
    JsonElement* m_current;
	Ref<JsonDocument>			m_localDoc;
    ArchiveMode m_mode;
};

} // namespace ln
