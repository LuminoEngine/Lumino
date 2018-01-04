
#pragma once
#include <type_traits>
#include <stack>
#include <unordered_map>
#include "String.h"
#include "../Reflection/ReflectionObject.h"
#include "../Base/Serialization.h"
#include "../Json/JsonDocument.h"

LN_NAMESPACE_BEGIN
class Archive2;

enum class ArchiveMode
{
	Save,
	Load,
};

enum class ArchiveNodeType
{
	Object,
	Array,
};

enum class SerializeClassFormat
{
	Default,
	String,
};

// save...
// サブクラスは作成直後は root を作る必要は無い。
// 最初はかならず writeObject か writeArray が呼ばれる。
//
// load...
// 最初にルート要素を準備しておく。
// アーカイブバージョンのチェックのため、ルート要素に対して find がかけられる。
class ArchiveStore
{
public:
	ArchiveStore()
		: m_nextName()
		, m_nextIndex(0)
		, m_readingNodeType()
		, m_readingArrayElementCount(0)
	{}

	// save のときは、次に write する値の名前。
	// load のときは、次に read する値の名前。
	void setNextName(const String& name) { m_nextName = name; }
	const String& getNextName() const { return m_nextName; }
	bool hasNextName() const { return !m_nextName.isEmpty(); }

	//-----------------------------------------------------------------------------
	// Save
	void writeObject() { onWriteObject(); m_nextName.clear(); }
	void writeArray() { onWriteArray(); m_nextName.clear(); }
	void writeValue(bool value) { onWriteValueBool(value); m_nextName.clear(); }
	void writeValue(int64_t value) { onWriteValueInt64(value); m_nextName.clear(); }
	void writeValue(double value) { onWriteValueDouble(value); m_nextName.clear(); }
	void writeValue(const String& value) { onWriteValueString(value); m_nextName.clear(); }

	//-----------------------------------------------------------------------------
	// Load
	ArchiveNodeType getNodeType() const { return m_readingNodeType; }
	int getArrayElementCount() const { return m_readingArrayElementCount; }
	void setNextIndex(int index) { m_nextIndex = index; }	// setNextName() と同じように使う Array 版
	int getNextIndex() const { return m_nextIndex; }
	bool readNode()
	{
		bool r = onReadNode(&m_readingArrayElementCount);
		m_readingNodeType = (m_readingArrayElementCount >= 0) ? ArchiveNodeType::Array : ArchiveNodeType::Object;
		postRead();
		return r;
	}		// Object Node をカレントにする
	bool readValue(bool* outValue) { bool r = onReadValueBool(outValue); postRead(); return r; }
	bool readValue(int64_t* outValue) { bool r = onReadValueInt64(outValue); postRead(); return r; }
	bool readValue(double* outValue) { bool r = onReadValueDouble(outValue); postRead(); return r; }
	bool readValue(String* outValue) { bool r = onReadValueString(outValue); postRead(); return r; }

protected:
	virtual void onWriteObject() = 0;
	virtual void onWriteArray() = 0;
	virtual void onWriteValueBool(bool value) = 0;
	virtual void onWriteValueInt64(int64_t value) = 0;
	virtual void onWriteValueDouble(double value) = 0;
	virtual void onWriteValueString(const String& value) = 0;

	virtual bool onReadNode(int* outElementCount) = 0;
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

	String	m_nextName;
	int		m_nextIndex;
	ArchiveNodeType	m_readingNodeType;
	int				m_readingArrayElementCount;
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

template<typename TValue>
class NameValuePair
{
public:
	const Char* name;
	TValue* value;

	NameValuePair(const Char* n, TValue* v) : name(n), value(v) {}

private:
	NameValuePair & operator=(NameValuePair const &) = delete;
};


namespace detail
{

// void serialize(Archive2& ar) をメンバ関数として持っているか
template<typename T>
class has_member_serialize_function
{
private:
	template<typename U>
	static auto check(U v) -> decltype(v.serialize(*reinterpret_cast<Archive2*>(nullptr)), std::true_type());
	static auto check(...) -> decltype(std::false_type());

public:
	typedef decltype(check(std::declval<T>())) type;
	static bool const value = type::value;
};

// void serialize(Archive2& ar) をメンバ関数として持っていないか
template<typename T>
class non_member_serialize_function
{
private:
	template<typename U>
	static auto check(U v) -> decltype(v.serialize(*reinterpret_cast<Archive2*>(nullptr)), std::true_type());
	static auto check(...) -> decltype(std::false_type());

public:
	typedef decltype(check(std::declval<T>())) type;
	static bool const value = !type::value;
};

template <class T> struct SerializeClassVersionInfo
{
	static const int value = 0;
};

template <class T> struct SerializeClassFormatInfo
{
	static const SerializeClassFormat value = SerializeClassFormat::Default;
};

} // namespace detail


#define LN_SERIALIZE_CLASS_VERSION(type, version) \
	namespace ln { namespace detail { \
		template<> struct SerializeClassVersionInfo<type> \
		{ \
			static const int value = version; \
		}; \
	} }

#define LN_SERIALIZE_CLASS_FORMAT(type, format)	\
	namespace ln { namespace detail { \
		template<> struct SerializeClassFormatInfo<type> \
		{ \
			static const SerializeClassFormat value = format; \
		}; \
	} }




/**
	@brief
	@details	処理開始前に DOM 構造をオンメモリに展開します。
				非常に大きいデータの読み書きには BinaryReader や BinaryWriter を使用してください。
*/
class Archive2
{
public:
	static const int ArchiveVersion;
	static const Char* ArchiveVersionKey;
	static const Char* ArchiveRootValueKey;
	static const Char* ClassNameKey;
	static const Char* ClassVersionKey;

	Archive2(ArchiveStore* store, ArchiveMode mode)
		: m_store(store)
		, m_mode(mode)
		, m_archiveVersion(0)
	{
		// コンテナの種類が本当に ArchiveStore に書き込まれるのは、最初の processWrite()
		m_nodeInfoStack.push(NodeInfo{});

		if (m_mode == ArchiveMode::Save)
		{
			m_archiveVersion = ArchiveVersion;
			writeArchiveHeader();
		}
		else
		{
			readArchiveHeader();
		}
	}

	~Archive2()
	{
	}

	bool isSaving() const { return m_mode == ArchiveMode::Save; }

	bool isLoading() const { return m_mode == ArchiveMode::Load; }

	int version() const { return m_nodeInfoStack.top().classVersion; }

	template<typename T>
	Archive2& operator & (T && value)
	{
		process(std::forward<T>(value));
		return *this;
	}

	template <class T>
	void process(T && head)
	{
		switch (m_mode)
		{
		case ArchiveMode::Save:
			processSave(head);
			break;
		case ArchiveMode::Load:
			processLoad(head);
			break;
		default:
			assert(0);
			break;
		}
	}

	void makeArrayTag()
	{
		moveState(NodeHeadState::Array);
	}

private:

	enum class NodeHeadState
	{
		Ready,			// 値の書き始め。"key": の直後や Array メンバ
		RequestPrimitiveValue,
		UserObject,	// ユーザー定義の型を書こうとしている。まだ Object にするべきか Array にするべきかわからない。
		//Pending,	
		Object,			// Object 確定状態
		Array,			// Array 確定状態
		Commited,
	};

	struct NodeInfo
	{
		NodeHeadState	headState = NodeHeadState::Ready;
		//bool			root = false;	// ユーザーデータのルート
		//bool	nodeHeadCommited = false;
		int				arrayIndex = -1;
		int classVersion = 0;
	};

	//-----------------------------------------------------------------------------
	// Save

	void writeArchiveHeader()
	{
		//TODO: JsonDocument2 がルート Object 固定なので今は制御できない
		//m_store->writeObject();
		moveState(NodeHeadState::Commited);
		m_store->setNextName(ArchiveVersionKey);
		m_store->writeValue(static_cast<int64_t>(ArchiveVersion));
		m_store->setNextName(ArchiveRootValueKey);

		// "root": の下にいる状態にする
		//m_nodeInfoStack.push(NodeInfo{});	// Object か Array かはまだわからない。待ち。
		//m_nodeInfoStack.top().root = true;
	}

	template<typename TValue>
	void processSave(NameValuePair<TValue>& nvp)
	{
		moveState(NodeHeadState::Object);	// これから { name, vaue } を追加するので、その親ノードは必ず Object
		preWriteValue();

		m_store->setNextName(nvp.name);

		//preWriteValue();
		writeValue(*nvp.value);
	}

	template<typename TValue>
	void processSave(TValue& value)
	{
		//preWriteValue();
		moveState((isPrimitiveType<TValue>()) ? NodeHeadState::RequestPrimitiveValue : NodeHeadState::UserObject);
		preWriteValue();
		writeValue(value);
	}

	void preWriteValue()
	{
		if (m_nodeInfoStack.top().headState == NodeHeadState::Object)
		{
			m_store->writeObject();
			writeClassVersion();
			moveState(NodeHeadState::Commited);
		}
		else if (m_nodeInfoStack.top().headState == NodeHeadState::Array)
		{
			m_store->writeArray();
			moveState(NodeHeadState::Commited);
		}
	}

	void moveState(NodeHeadState req)
	{
		switch (m_nodeInfoStack.top().headState)
		{
			case NodeHeadState::Ready:

				if (req == NodeHeadState::RequestPrimitiveValue)	// キー名無しでいきなり値を書こうとした。Array 確定
				{
					m_nodeInfoStack.top().headState = NodeHeadState::Object;
				}
				else
					m_nodeInfoStack.top().headState = req;
				break;
			case NodeHeadState::UserObject:
				if (req == NodeHeadState::RequestPrimitiveValue)
				{
					m_nodeInfoStack.top().headState = NodeHeadState::Object;
				}
				break;
			case NodeHeadState::Object:
				if (req == NodeHeadState::Commited)
					m_nodeInfoStack.top().headState = NodeHeadState::Commited;
				break;
			case NodeHeadState::Array:
				if (req == NodeHeadState::Commited)
					m_nodeInfoStack.top().headState = NodeHeadState::Commited;
				break;
			case NodeHeadState::Commited:
				break;
		}
	}

	template<typename T> bool isPrimitiveType() const { return false; }
	template<> bool isPrimitiveType<bool>() const { return true; }
	template<> bool isPrimitiveType<int8_t>() const { return true; }
	template<> bool isPrimitiveType<int16_t>() const { return true; }
	template<> bool isPrimitiveType<int32_t>() const { return true; }
	template<> bool isPrimitiveType<int64_t>() const { return true; }
	template<> bool isPrimitiveType<uint8_t>() const { return true; }
	template<> bool isPrimitiveType<uint16_t>() const { return true; }
	template<> bool isPrimitiveType<uint32_t>() const { return true; }
	template<> bool isPrimitiveType<uint64_t>() const { return true; }
	template<> bool isPrimitiveType<float>() const { return true; }
	template<> bool isPrimitiveType<double>() const { return true; }
	template<> bool isPrimitiveType<String>() const { return true; }

	void writeValue(bool value) { m_store->writeValue(value); }
	void writeValue(int8_t value) { m_store->writeValue(static_cast<int64_t>(value)); }
	void writeValue(int16_t value) { m_store->writeValue(static_cast<int64_t>(value)); }
	void writeValue(int32_t value) { m_store->writeValue(static_cast<int64_t>(value)); }
	void writeValue(int64_t value) { m_store->writeValue(static_cast<int64_t>(value)); }
	void writeValue(uint8_t value) {  m_store->writeValue(static_cast<int64_t>(value)); }
	void writeValue(uint16_t value) { m_store->writeValue(static_cast<int64_t>(value)); }
	void writeValue(uint32_t value) { m_store->writeValue(static_cast<int64_t>(value)); }
	void writeValue(uint64_t value) { m_store->writeValue(static_cast<int64_t>(value)); }
	void writeValue(float value) { m_store->writeValue(static_cast<double>(value)); }
	void writeValue(double value) { m_store->writeValue(static_cast<double>(value)); }
	void writeValue(String& value) { m_store->writeValue(value); }

	// メンバ関数として serialize を持つ型の writeValue()
	template<
		typename TValue,
		typename std::enable_if<detail::has_member_serialize_function<TValue>::value, std::nullptr_t>::type = nullptr>
		void writeValue(TValue& value)	// メンバ serialize() が const 関数とは限らないのでここは非 const 参照
	{
		m_nodeInfoStack.push(NodeInfo{});
		m_nodeInfoStack.top().classVersion = ln::detail::SerializeClassVersionInfo<TValue>::value;
		value.serialize(*this);
	}

	// メンバ関数として serialize を持たない型の writeValue()
	template<
		typename TValue,
		typename std::enable_if<detail::non_member_serialize_function<TValue>::value, std::nullptr_t>::type = nullptr>
		void writeValue(TValue& value)
	{
		m_nodeInfoStack.push(NodeInfo{});
		m_nodeInfoStack.top().classVersion = ln::detail::SerializeClassVersionInfo<TValue>::value;
		serialize(*this, value);
	}

	void writeClassVersion()
	{
		if (m_nodeInfoStack.top().classVersion > 0)
		{
			m_store->setNextName(ClassVersionKey);
			writeValue(m_nodeInfoStack.top().classVersion);
		}
	}

	//-----------------------------------------------------------------------------
	// Load

	template<typename TValue>
	void processLoad(NameValuePair<TValue>& nvp)
	{
		//preReadValue(NodeHeadSeq::Object);	// これから { name, vaue } を読むので、その親ノードは必ず Object
		m_store->setNextName(nvp.name);
		preReadValue();
		readValue(*nvp.value);
	}

	template<typename TValue>
	void processLoad(TValue& value)
	{
		preReadValue();
		readValue(value);
	}

	void preReadValue()
	{
		if (m_store->getNodeType() == ArchiveNodeType::Array)
		{
			m_nodeInfoStack.top().arrayIndex++;
			if (m_store->getArrayElementCount() <= m_nodeInfoStack.top().arrayIndex)
			{
				LN_UNREACHABLE();
			}
			m_store->setNextIndex(m_nodeInfoStack.top().arrayIndex);
		}
	}

	void readArchiveHeader()
	{
		//TODO: JsonDocument2 がルート Object 固定なので今は制御できない
		//m_store->readObject();
		m_store->setNextName(ArchiveVersionKey);
		m_store->readValue(&m_archiveVersion);
		m_store->setNextName(ArchiveRootValueKey);
	}

	void readValue(bool& outValue) { m_store->readValue(&outValue); }
	void readValue(int8_t& outValue) { int64_t tmp; m_store->readValue(&tmp); outValue = static_cast<int8_t>(tmp); }
	void readValue(int16_t& outValue) { int64_t tmp; m_store->readValue(&tmp); outValue = static_cast<int16_t>(tmp); }
	void readValue(int32_t& outValue) { int64_t tmp; m_store->readValue(&tmp); outValue = static_cast<int32_t>(tmp); }
	void readValue(int64_t& outValue) { int64_t tmp; m_store->readValue(&tmp); outValue = static_cast<int64_t>(tmp); }
	void readValue(uint8_t& outValue) { int64_t tmp; m_store->readValue(&tmp); outValue = static_cast<uint8_t>(tmp); }
	void readValue(uint16_t& outValue) { int64_t tmp; m_store->readValue(&tmp); outValue = static_cast<uint16_t>(tmp); }
	void readValue(uint32_t& outValue) { int64_t tmp; m_store->readValue(&tmp); outValue = static_cast<uint32_t>(tmp); }
	void readValue(uint64_t& outValue) { int64_t tmp; m_store->readValue(&tmp); outValue = static_cast<uint64_t>(tmp); }
	void readValue(float& outValue) { double tmp; m_store->readValue(&tmp); outValue = static_cast<float>(tmp); }
	void readValue(double& outValue) { double tmp; m_store->readValue(&tmp); outValue = static_cast<double>(tmp); }
	void readValue(String& outValue) { m_store->readValue(&outValue); }

	// メンバ関数として serialize を持つ型の readValue()
	template<
		typename TValue,
		typename std::enable_if<detail::has_member_serialize_function<TValue>::value, std::nullptr_t>::type = nullptr>
	void readValue(TValue& outValue)
	{
		m_nodeInfoStack.push(NodeInfo{});
		m_store->readNode();
		readClassVersion();
		outValue.serialize(*this);
	}

	// メンバ関数として serialize を持たない型の readValue()
	template<
		typename TValue,
		typename std::enable_if<detail::non_member_serialize_function<TValue>::value, std::nullptr_t>::type = nullptr>
	void readValue(TValue& outValue)
	{
		m_nodeInfoStack.push(NodeInfo{});
		m_store->readNode();
		readClassVersion();
		serialize(*this, outValue);
	}

	void readClassVersion()
	{
		m_store->setNextName(ClassVersionKey);
		int64_t version;
		if (m_store->readValue(&version))
		{
			m_nodeInfoStack.top().classVersion = version;
		}
	}

	ArchiveStore* m_store;
	ArchiveMode m_mode;
	std::stack<NodeInfo>	m_nodeInfoStack;
	int64_t	m_archiveVersion;
};

//void serialize(Archive2& ar, int value)
//{
//
//}
//




#define LN_NVP2(var)		ln::makeNVP(_LT(#var), var)

template<typename TValue>
NameValuePair<TValue> makeNVP(const Char* name, TValue& valueRef)
{
	return NameValuePair<TValue>(name, &valueRef);
}

template<typename TValue>
NameValuePair<TValue> makeNVP(const Char* name, TValue& valueRef, const TValue& defaultValue)
{
	return NameValuePair<TValue>(name, &valueRef, &defaultValue);
}

LN_NAMESPACE_END
