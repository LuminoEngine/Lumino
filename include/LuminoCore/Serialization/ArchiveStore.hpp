
#pragma once
#include <stack>
#include "../Base/String.hpp"
#include "../Json/JsonDocument.hpp"

namespace ln {
class Archive;

enum class ArchiveContainerType
{
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
	ArchiveContainerType getContainerType() const { return onGetContainerType(); }// { return m_nodeStack.top().m_readingNodeType; }
	int getContainerElementCount() const { return onReadContainerElementCount(); }
	void setNextIndex(int index) { m_nextIndex = index; }	// setNextName() と同じように使う Array 版
	int getNextIndex() const { return m_nextIndex; }
	virtual bool hasKey(const String& name) const = 0;
	virtual const String& memberKey(int index) const = 0;
	bool readContainer()
	{
		int dummy;
		bool r = onReadContainer(&dummy);//&m_readingArrayElementCount);
		//m_nodeStack.push(NodeInfo{});
		//m_nodeStack.top().m_readingNodeType = (m_readingArrayElementCount >= 0) ? ArchiveNodeType::Array : ArchiveNodeType::Object;
		postRead();
		return r;
	}		// Object Container をカレントにする
	void readContainerEnd() { onReadContainerEnd(); /*m_nodeStack.pop();*/ }
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

	virtual bool onReadContainer(int* outElementCount) = 0;
	virtual void onReadContainerEnd() = 0;
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

	JsonArchiveStore(JsonDocument* doc)
		: m_localDoc(doc)
	{
		// TODO: 今 JsonDocument はルート Array に対応していないのでこんな感じ。
		//m_nodeStack.push(doc);
	}

	~JsonArchiveStore()
	{
	}

protected:
	virtual ArchiveContainerType onGetContainerType() const override
	{
		if (m_nodeStack.top()->type() == JsonElementType::Object)
			return ArchiveContainerType::Object;
		else if (m_nodeStack.top()->type() == JsonElementType::Array)
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
		else if (checkTopType(JsonElementType::Object))
		{
			if (LN_REQUIRE(hasNextName())) return;
			JsonArray* v = static_cast<JsonObject*>(m_nodeStack.top())->addArray(getNextName());
			m_nodeStack.push(v);
		}
		else if (checkTopType(JsonElementType::Array))
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
		if (checkTopType(JsonElementType::Object)) {
			static_cast<JsonObject*>(m_nodeStack.top())->addNullValue(getNextName());
		}
	}

#define ON_WRITE_VALUE_FUNC(type, name) \
	virtual void onWriteValue##name(type value) override \
	{ \
		if (checkTopType(JsonElementType::Object)) \
		{ \
			if (LN_REQUIRE(hasNextName())) return; \
			static_cast<JsonObject*>(m_nodeStack.top())->add##name##Value(getNextName(), value); \
		} \
		else if (checkTopType(JsonElementType::Array)) \
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

	virtual bool hasKey(const String& name) const override
	{
		if (checkTopType(JsonElementType::Object))
		{
			return static_cast<JsonObject*>(m_nodeStack.top())->find(name) != nullptr;
		}
		else
		{
			LN_NOTIMPLEMENTED();
			return false;
		}
	}

	virtual const String& memberKey(int index) const override
	{
		if (checkTopType(JsonElementType::Object))
		{
			return static_cast<JsonObject*>(m_nodeStack.top())->memberKey(index);
		}
		else
		{
			LN_NOTIMPLEMENTED();
			static const String empty;
			return empty;
		}
	}

	virtual bool onReadContainer(int* outElementCount) override
	{
		JsonElement* element = nullptr;
		if (m_nodeStack.empty())
		{
			element = m_localDoc->rootElement();
			if (!element) return false;
			m_nodeStack.push(element);
		}
		else if (checkTopType(JsonElementType::Object))
		{
			if (LN_REQUIRE(hasNextName())) return false;
			element = static_cast<JsonObject*>(m_nodeStack.top())->find(getNextName());
			if (!element) return false;
			m_nodeStack.push(element);
		}
		else if (checkTopType(JsonElementType::Array))
		{
			element = static_cast<JsonArray*>(m_nodeStack.top())->element(getNextIndex());
			m_nodeStack.push(element);
		}
		else
		{
			LN_NOTIMPLEMENTED();
		}

		if (element->type() == JsonElementType::Object)
		{
			*outElementCount = -1;
		}
		else if (element->type() == JsonElementType::Array)
		{
			*outElementCount = static_cast<JsonArray*>(element)->elementCount();
		}
		else
		{
			LN_NOTIMPLEMENTED();
		}

		return true;
	}

	virtual void onReadContainerEnd() override
	{
		m_nodeStack.pop();
	}

	virtual int onReadContainerElementCount() const override
	{
		if (m_nodeStack.top()->type() == JsonElementType::Object)
			return static_cast<JsonObject*>(m_nodeStack.top())->memberCount();
		else if (m_nodeStack.top()->type() == JsonElementType::Array)
			return static_cast<JsonArray*>(m_nodeStack.top())->elementCount();
		else
			LN_UNREACHABLE();
		return -1;
	}

	JsonValue* readValueHelper()
	{
		if (checkTopType(JsonElementType::Object))
		{
			if (LN_REQUIRE(hasNextName())) return nullptr;
			JsonElement* v = static_cast<JsonObject*>(m_nodeStack.top())->find(getNextName());
			return static_cast<JsonValue*>(v);
			//if (LN_ENSURE(v->type() == JsonElementType::internalName)) return;
			//*outValue = static_cast<type>(static_cast<JsonValue*>(v)->get##internalName());
		}
		else if (checkTopType(JsonElementType::Array))
		{
			JsonElement* v = static_cast<JsonArray*>(m_nodeStack.top())->element(getNextIndex());
			//if (LN_ENSURE(v->type() == JsonElementType::internalName)) return;
			return static_cast<JsonValue*>(v);
			//*outValue = static_cast<type>(static_cast<JsonValue*>(v)->get##internalName());
		}
		else
		{
			LN_NOTIMPLEMENTED();
			return nullptr;
		}
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
		JsonValue* v = readValueHelper();
		if (!v) return ArchiveNodeType::Null;

		switch (v->type())
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
	bool checkTopType(JsonElementType t) const { return m_nodeStack.top()->type() == t; }

	std::stack<JsonElement*>	m_nodeStack;	// Value 型は積まれない。コンテナのみ。
	Ref<JsonDocument>			m_localDoc;
};

} // namespace ln
