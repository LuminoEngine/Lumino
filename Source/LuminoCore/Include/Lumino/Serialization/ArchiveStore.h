
#pragma once
#include <stack>
#include "String.h"
#include "../Json/JsonDocument.h"

LN_NAMESPACE_BEGIN
class Archive2;

enum class ArchiveNodeType
{
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
	void writeValue(bool value) { onWriteValueBool(value); m_nextName.clear(); }
	void writeValue(int64_t value) { onWriteValueInt64(value); m_nextName.clear(); }
	void writeValue(double value) { onWriteValueDouble(value); m_nextName.clear(); }
	void writeValue(const String& value) { onWriteValueString(value); m_nextName.clear(); }

	//-----------------------------------------------------------------------------
	// Load
	ArchiveNodeType getNodeType() const { return onGetNodeType(); }// { return m_nodeStack.top().m_readingNodeType; }
	int getArrayElementCount() const { return onReadArrayElementCount(); }
	void setNextIndex(int index) { m_nextIndex = index; }	// setNextName() と同じように使う Array 版
	int getNextIndex() const { return m_nextIndex; }
	bool readNode()
	{
		int dummy;
		bool r = onReadNode(&dummy);//&m_readingArrayElementCount);
		//m_nodeStack.push(NodeInfo{});
		//m_nodeStack.top().m_readingNodeType = (m_readingArrayElementCount >= 0) ? ArchiveNodeType::Array : ArchiveNodeType::Object;
		postRead();
		return r;
	}		// Object Node をカレントにする
	void readNodeEnd() { onReadNodeEnd(); /*m_nodeStack.pop();*/ }
	bool readValue(bool* outValue) { bool r = onReadValueBool(outValue); postRead(); return r; }
	bool readValue(int64_t* outValue) { bool r = onReadValueInt64(outValue); postRead(); return r; }
	bool readValue(double* outValue) { bool r = onReadValueDouble(outValue); postRead(); return r; }
	bool readValue(String* outValue) { bool r = onReadValueString(outValue); postRead(); return r; }

protected:
	virtual ArchiveNodeType onGetNodeType() const = 0;

	virtual void onWriteObject() = 0;
	virtual void onWriteArray() = 0;
	virtual void onWriteObjectEnd() = 0;
	virtual void onWriteArrayEnd() = 0;
	virtual void onWriteValueBool(bool value) = 0;
	virtual void onWriteValueInt64(int64_t value) = 0;
	virtual void onWriteValueDouble(double value) = 0;
	virtual void onWriteValueString(const String& value) = 0;

	virtual bool onReadNode(int* outElementCount) = 0;
	virtual void onReadNodeEnd() = 0;
	virtual int onReadArrayElementCount() const = 0;
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
	JsonArchiveStore(tr::JsonDocument2* doc)
	{
		// TODO: 今 JsonDocument2 はルート Array に対応していないのでこんな感じ。
		m_nodeStack.push(doc);
	}

	~JsonArchiveStore()
	{
	}

protected:
	virtual ArchiveNodeType onGetNodeType() const override
	{
		if (m_nodeStack.top()->getType() == tr::JsonValueType::Object)
			return ArchiveNodeType::Object;
		else if (m_nodeStack.top()->getType() == tr::JsonValueType::Array)
			return ArchiveNodeType::Array;
		else
			LN_UNREACHABLE();
		return ArchiveNodeType::Object;
	}

	virtual void onWriteObject() override
	{
		if (m_nodeStack.top()->getType() == tr::JsonValueType::Object)
		{
			if (LN_REQUIRE(hasNextName())) return;
			tr::JsonObject2* v = static_cast<tr::JsonObject2*>(m_nodeStack.top())->addMemberObject(getNextName());
			m_nodeStack.push(v);
		}
		else if (m_nodeStack.top()->getType() == tr::JsonValueType::Array)
		{
			tr::JsonObject2* v = static_cast<tr::JsonArray2*>(m_nodeStack.top())->addObject();
			m_nodeStack.push(v);
		}
		else
		{
			LN_UNREACHABLE();
		}
	}

	virtual void onWriteArray() override
	{
		if (checkTopType(tr::JsonValueType::Object))
		{
			if (LN_REQUIRE(hasNextName())) return;
			tr::JsonArray2* v = static_cast<tr::JsonObject2*>(m_nodeStack.top())->addMemberArray(getNextName());
			m_nodeStack.push(v);
		}
		else if (checkTopType(tr::JsonValueType::Array))
		{
			tr::JsonArray2* v = static_cast<tr::JsonArray2*>(m_nodeStack.top())->addArray();
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

#define ON_WRITE_VALUE_FUNC(type, name) \
	virtual void onWriteValue##name(type value) override \
	{ \
		if (checkTopType(tr::JsonValueType::Object)) \
		{ \
			if (LN_REQUIRE(hasNextName())) return; \
			static_cast<tr::JsonObject2*>(m_nodeStack.top())->addMember##name(getNextName(), value); \
		} \
		else if (checkTopType(tr::JsonValueType::Array)) \
		{ \
			static_cast<tr::JsonArray2*>(m_nodeStack.top())->add##name(value); \
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

	virtual bool onReadNode(int* outElementCount) override
	{
		tr::JsonElement2* element;
		if (checkTopType(tr::JsonValueType::Object))
		{
			if (LN_REQUIRE(hasNextName())) return false;
			element = static_cast<tr::JsonObject2*>(m_nodeStack.top())->find(getNextName());
			if (!element) return false;
			m_nodeStack.push(element);
		}
		else if (checkTopType(tr::JsonValueType::Array))
		{
			element = static_cast<tr::JsonArray2*>(m_nodeStack.top())->getElement(getNextIndex());
			m_nodeStack.push(element);
		}

		if (element->getType() == tr::JsonValueType::Object)
		{
			*outElementCount = -1;
		}
		else if (element->getType() == tr::JsonValueType::Array)
		{
			*outElementCount = static_cast<tr::JsonArray2*>(element)->getElementCount();
		}
		else
		{
			LN_NOTIMPLEMENTED();
		}

		return true;
	}

	virtual void onReadNodeEnd() override
	{
		m_nodeStack.pop();
	}

	virtual int onReadArrayElementCount() const override
	{
		if (m_nodeStack.top()->getType() == tr::JsonValueType::Object)
			return -1;
		else if (m_nodeStack.top()->getType() == tr::JsonValueType::Array)
			return static_cast<tr::JsonArray2*>(m_nodeStack.top())->getElementCount();
		else
			LN_UNREACHABLE();
		return -1;
	}

	tr::JsonValue2* readValueHelper()
	{
		if (checkTopType(tr::JsonValueType::Object))
		{
			if (LN_REQUIRE(hasNextName())) return nullptr;
			tr::JsonElement2* v = static_cast<tr::JsonObject2*>(m_nodeStack.top())->find(getNextName());
			return static_cast<tr::JsonValue2*>(v);
			//if (LN_ENSURE(v->getType() == tr::JsonValueType::internalName)) return;
			//*outValue = static_cast<type>(static_cast<tr::JsonValue2*>(v)->get##internalName());
		}
		else if (checkTopType(tr::JsonValueType::Array))
		{
			tr::JsonElement2* v = static_cast<tr::JsonArray2*>(m_nodeStack.top())->getElement(getNextIndex());
			//if (LN_ENSURE(v->getType() == tr::JsonValueType::internalName)) return;
			return static_cast<tr::JsonValue2*>(v);
			//*outValue = static_cast<type>(static_cast<tr::JsonValue2*>(v)->get##internalName());
		}
		else
		{
			LN_NOTIMPLEMENTED();
			return nullptr;
		}
	}

	virtual bool onReadValueBool(bool* outValue) override
	{
		tr::JsonValue2* v = readValueHelper();
		if (!v) return false;

		if (v->getType() == tr::JsonValueType::Bool)
			*outValue = v->getBool();
		else
			LN_UNREACHABLE();

		return true;
	}

	virtual bool onReadValueInt64(int64_t* outValue) override
	{
		tr::JsonValue2* v = readValueHelper();
		if (!v) return false;

		if (v->getType() == tr::JsonValueType::Int32)
			*outValue = v->getInt32();
		else if (v->getType() == tr::JsonValueType::Int64)
			*outValue = v->getInt64();
		else
			LN_UNREACHABLE();

		return true;
	}

	virtual bool onReadValueDouble(double* outValue) override
	{
		tr::JsonValue2* v = readValueHelper();
		if (!v) return false;

		if (v->getType() == tr::JsonValueType::Float)
			*outValue = v->getFloat();
		else if (v->getType() == tr::JsonValueType::Double)
			*outValue = v->getDouble();
		else
			LN_UNREACHABLE();

		return true;
	}

	virtual bool onReadValueString(String* outValue) override
	{
		tr::JsonValue2* v = readValueHelper();
		if (!v) return false;

		if (v->getType() == tr::JsonValueType::String)
			*outValue = v->getString();
		else
			LN_UNREACHABLE();

		return true;
	}

private:
	bool checkTopType(tr::JsonValueType t) { return m_nodeStack.top()->getType() == t; }

	std::stack<tr::JsonElement2*>	m_nodeStack;
};

LN_NAMESPACE_END
