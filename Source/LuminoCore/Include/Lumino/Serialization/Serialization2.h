
#pragma once
#include <type_traits>
#include <stack>
#include <unordered_map>
#include "String.h"
#include "../Reflection/ReflectionObject.h"
#include "../Base/Serialization.h"
#include "../Json/JsonDocument.h"
#include "ArchiveStore.h"

LN_NAMESPACE_BEGIN
class Archive2;

enum class ArchiveMode
{
	Save,
	Load,
};

enum class SerializeClassFormat
{
	Default,
	String,
};

namespace detail
{

struct NameValuePairBase {};

} // namespace detail

template<typename TValue>
struct NameValuePair : public detail::NameValuePairBase
{
public:
	const StringRef& name;
	TValue* value;
	const TValue* defaultValue;

	NameValuePair(const StringRef& n, TValue* v) : name(n), value(v), defaultValue(nullptr) {}
	NameValuePair(const StringRef& n, TValue* v, const TValue& defaultValue) : name(n), value(v), defaultValue(&defaultValue) {}

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
	static auto check(U&& v) -> decltype(v.serialize(*reinterpret_cast<Archive2*>(0)), std::true_type());
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
	static auto check(U&& v) -> decltype(v.serialize(*reinterpret_cast<Archive2*>(0)), std::true_type());
	static auto check(...) -> decltype(std::false_type());

public:
	typedef decltype(check(std::declval<T>())) type;
	static bool const value = !type::value;
};

// void serialize(Archive2& ar) をメンバ関数として持っているか
template<typename T>
class has_static_member_class_version
{
private:
	template<typename U>
	static auto check(U&& v) -> decltype(U::lumino_class_version, std::true_type());
	static auto check(...) -> decltype(std::false_type());

public:
	typedef decltype(check(std::declval<T>())) type;
	static bool const value = type::value;
};

template <class T> struct SerializeClassVersionInfo
{
	static const int value = 0;
};

template <class T> struct SerializeClassFormatInfo
{
	static const SerializeClassFormat value = SerializeClassFormat::Default;
};

template<class T>
struct ArchiveValueTraits
{
	static bool isPrimitiveType() { return false; }
};

template<>
struct ArchiveValueTraits<bool>
{
	static bool isPrimitiveType() { return true; }
};

template<>
struct ArchiveValueTraits<int8_t>
{
	static bool isPrimitiveType() { return true; }
};

template<>
struct ArchiveValueTraits<int16_t>
{
	static bool isPrimitiveType() { return true; }
};

template<>
struct ArchiveValueTraits<int32_t>
{
	static bool isPrimitiveType() { return true; }
};

template<>
struct ArchiveValueTraits<int64_t>
{
	static bool isPrimitiveType() { return true; }
};

template<>
struct ArchiveValueTraits<uint8_t>
{
	static bool isPrimitiveType() { return true; }
};

template<>
struct ArchiveValueTraits<uint16_t>
{
	static bool isPrimitiveType() { return true; }
};

template<>
struct ArchiveValueTraits<uint32_t>
{
	static bool isPrimitiveType() { return true; }
};

template<>
struct ArchiveValueTraits<uint64_t>
{
	static bool isPrimitiveType() { return true; }
};

template<>
struct ArchiveValueTraits<float>
{
	static bool isPrimitiveType() { return true; }
};

template<>
struct ArchiveValueTraits<double>
{
	static bool isPrimitiveType() { return true; }
};

template<>
struct ArchiveValueTraits<String>
{
	static bool isPrimitiveType() { return true; }
};

} // namespace detail

// non‐intrusive
#define LN_SERIALIZE_CLASS_VERSION_NI(type, version) \
	namespace ln { namespace detail { \
		template<> struct SerializeClassVersionInfo<type> \
		{ \
			static const int value = version; \
		}; \
	} }

#define LN_SERIALIZE_CLASS_VERSION(version) \
	friend class ::ln::Archive2; \
	static const int lumino_class_version = version;

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
	template<typename TBase>
	struct BaseClass
	{
		TBase* basePtr;
		BaseClass(TBase* ptr) : basePtr(ptr) {}
	};

	static const int ArchiveVersion;
	static const String ArchiveVersionKey;
	static const String ArchiveRootValueKey;
	static const String ClassNameKey;
	static const String ClassVersionKey;
	static const String ClassBaseKey;

	Archive2(ArchiveStore* store, ArchiveMode mode)
		: Archive2()
	{
		setup(store, mode);
	}

	~Archive2()
	{
	}

	bool isSaving() const { return m_mode == ArchiveMode::Save; }

	bool isLoading() const { return m_mode == ArchiveMode::Load; }

	int classVersion() const { return m_nodeInfoStack.top().classVersion; }

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

	void makeArrayTag(int* outSize)
	{
		moveState(NodeHeadState::Array);

		if (isLoading())
		{
			// この時点で size を返したいので、store を ArrayContainer まで移動して size を得る必要がある
			preReadValue();
			if (LN_REQUIRE(m_store->getContainerType() == ArchiveContainerType::Array)) return;
			if (outSize) *outSize = m_store->getArrayElementCount();
		}
	}

	/**
	Note: オブジェクトを単純な文字列としてシリアライズする場合に使用します。
	この関数を呼び出した serialize() 内では他の値をシリアライズすることはできません。
	*/
	void makeStringTag(String* str)
	{
		moveState(NodeHeadState::Value);
		process(*str);
	}

protected:

	Archive2()
		: m_store(nullptr)
		, m_mode(ArchiveMode::Save)
		, m_archiveVersion(0)
	{
	}

	void setup(ArchiveStore* store, ArchiveMode mode)
	{
		m_store = store;
		m_mode = mode;

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

private:

	enum class NodeHeadState
	{
		Ready,			// 値の書き始め。"key": の直後や Array メンバ
		RequestPrimitiveValue,
		UserObject,	// ユーザー定義の型を書こうとしている。まだ Object にするべきか Array にするべきかわからない。
		//Pending,	
		Object,			// Object 確定状態
		Array,			// Array 確定状態
		Value,			
		ContainerOpend,	// Object または Array を開始した (Value の場合はこの状態にはならない)
	};

	struct NodeInfo
	{
		NodeHeadState headState = NodeHeadState::Ready;
		int arrayIndex = -1;
		int classVersion = 0;
		bool nextBaseCall = false;
		detail::NameValuePairBase*	lastNVP = nullptr;
	};

	//-----------------------------------------------------------------------------
	// Save

	void writeArchiveHeader()
	{
		//TODO: JsonDocument2 がルート Object 固定なので今は制御できない
		//m_store->writeObject();
		moveState(NodeHeadState::ContainerOpend);
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
		writeValue(*nvp.value);
	}

	template<typename TValue>
	void processSave(BaseClass<TValue>& base)
	{
		moveState(NodeHeadState::Object);	// BaseClass は Object のシリアライズの一部なので、親ノードは必ず Object
		preWriteValue();
		m_nodeInfoStack.top().nextBaseCall = true;
		m_store->setNextName(ClassBaseKey);
		writeValue(*base.basePtr);
	}

	template<typename TValue>
	void processSave(TValue& value)
	{
		moveState((detail::ArchiveValueTraits<TValue>::isPrimitiveType()) ? NodeHeadState::RequestPrimitiveValue : NodeHeadState::UserObject);
		preWriteValue();
		writeValue(value);
	}

	void preWriteValue()
	{
		if (m_nodeInfoStack.top().headState == NodeHeadState::Object)
		{
			m_store->writeObject();
			writeClassVersion();
			moveState(NodeHeadState::ContainerOpend);
		}
		else if (m_nodeInfoStack.top().headState == NodeHeadState::Array)
		{
			m_store->writeArray();
			moveState(NodeHeadState::ContainerOpend);
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
				if (req == NodeHeadState::ContainerOpend)
					m_nodeInfoStack.top().headState = NodeHeadState::ContainerOpend;
				break;
			case NodeHeadState::Array:
				if (req == NodeHeadState::ContainerOpend)
					m_nodeInfoStack.top().headState = NodeHeadState::ContainerOpend;
				break;
			case NodeHeadState::Value:
				break;
			case NodeHeadState::ContainerOpend:
				break;
		}
	}



	template<
		typename TValue,
		typename std::enable_if<detail::has_static_member_class_version<TValue>::value, std::nullptr_t>::type = nullptr>
		int getClassVersion()
	{
		return TValue::lumino_class_version;
	}

	template<
		typename TValue,
		typename std::enable_if<!detail::has_static_member_class_version<TValue>::value, std::nullptr_t>::type = nullptr>
		int getClassVersion()
	{
		return ln::detail::SerializeClassVersionInfo<TValue>::value;
	}

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
		bool baseCall = m_nodeInfoStack.top().nextBaseCall;
		m_nodeInfoStack.push(NodeInfo{});
		m_nodeInfoStack.top().classVersion = getClassVersion<TValue>();//ln::detail::SerializeClassVersionInfo<TValue>::value;

		if (baseCall)
			value.TValue::serialize(*this);
		else
			value.serialize(*this);

		postContainerWrite();
	}

	// メンバ関数として serialize を持たない型の writeValue()
	template<
		typename TValue,
		typename std::enable_if<detail::non_member_serialize_function<TValue>::value, std::nullptr_t>::type = nullptr>
		void writeValue(TValue& value)
	{
		m_nodeInfoStack.push(NodeInfo{});
		m_nodeInfoStack.top().classVersion = getClassVersion<TValue>();//ln::detail::SerializeClassVersionInfo<TValue>::value;
		serialize(*this, value);
		postContainerWrite();
	}

	void postContainerWrite()
	{
		if (m_nodeInfoStack.top().headState == NodeHeadState::Ready)
		{
			// 空オブジェクトをシリアライズした。コンテナを開始していないので開始する
			moveState(NodeHeadState::Object);
		}
		if (m_nodeInfoStack.top().headState != NodeHeadState::ContainerOpend)
		{
			preWriteValue();
		}



		m_nodeInfoStack.pop();
		if (m_store->getContainerType() == ArchiveContainerType::Object)
			m_store->writeObjectEnd();
		else if (m_store->getContainerType() == ArchiveContainerType::Array)
			m_store->writeArrayEnd();
		m_nodeInfoStack.top().nextBaseCall = false;
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
		moveState(NodeHeadState::Object);	// BaseClass は Object のシリアライズの一部なので、親ノードは必ず Object
		preReadValue();

		m_store->setNextName(nvp.name);
		m_nodeInfoStack.top().lastNVP = &nvp;
		readValue(*nvp.value);
	}

	template<typename TValue>
	void processLoad(BaseClass<TValue>& base)
	{
		moveState(NodeHeadState::Object);
		preReadValue();
		m_nodeInfoStack.top().nextBaseCall = true;
		m_store->setNextName(ClassBaseKey);
		readValue(*base.basePtr);
	}

	template<typename TValue>
	void processLoad(TValue& value)
	{
		preReadValue();
		readValue(value);
	}

	void preReadValue()
	{
		if (m_nodeInfoStack.top().headState == NodeHeadState::Object)
		{
			m_store->readContainer();
			if (m_store->getContainerType() == ArchiveContainerType::Object)
				readClassVersion();
			moveState(NodeHeadState::ContainerOpend);
		}
		else if (m_nodeInfoStack.top().headState == NodeHeadState::Array)
		{
			m_store->readContainer();
			moveState(NodeHeadState::ContainerOpend);
		}
		else if (m_nodeInfoStack.top().headState == NodeHeadState::ContainerOpend)
		{
			if (m_store->getContainerType() == ArchiveContainerType::Array)
			{
				m_nodeInfoStack.top().arrayIndex++;
				if (m_store->getArrayElementCount() <= m_nodeInfoStack.top().arrayIndex)
				{
					LN_UNREACHABLE();
				}
				m_store->setNextIndex(m_nodeInfoStack.top().arrayIndex);
			}
		}
	}

	void readArchiveHeader()
	{
		//TODO: JsonDocument2 がルート Object 固定なので今は制御できない
		//m_store->readObject();
		m_store->setNextName(ArchiveVersionKey);
		if (!m_store->readValue(&m_archiveVersion)) { onError(); return; }
		m_store->setNextName(ArchiveRootValueKey);
	}

	template<typename TValue>
	bool setDefaultValueHelper(TValue& outValue)
	{
		auto* nvp = static_cast<NameValuePair<TValue>*>(m_nodeInfoStack.top().lastNVP);
		if (nvp &&
			nvp->defaultValue)
		{
			outValue = *nvp->defaultValue;
			return true;
		}
		return false;
	}

	void readValue(bool& outValue) { if (!m_store->readValue(&outValue)) { if (!setDefaultValueHelper(outValue)) onError(); return; } }
	void readValue(int8_t& outValue) { int64_t tmp; if (!m_store->readValue(&tmp)) { if (!setDefaultValueHelper(outValue)) onError(); return; } outValue = static_cast<int8_t>(tmp); }
	void readValue(int16_t& outValue) { int64_t tmp; if (!m_store->readValue(&tmp)) { if (!setDefaultValueHelper(outValue)) onError(); return; } outValue = static_cast<int16_t>(tmp); }
	void readValue(int32_t& outValue) { int64_t tmp; if (!m_store->readValue(&tmp)) { if (!setDefaultValueHelper(outValue)) onError(); return; } outValue = static_cast<int32_t>(tmp); }
	void readValue(int64_t& outValue) { int64_t tmp; if (!m_store->readValue(&tmp)) { if (!setDefaultValueHelper(outValue)) onError(); return; } outValue = static_cast<int64_t>(tmp); }
	void readValue(uint8_t& outValue) { int64_t tmp; if (!m_store->readValue(&tmp)) { if (!setDefaultValueHelper(outValue)) onError(); return; } outValue = static_cast<uint8_t>(tmp); }
	void readValue(uint16_t& outValue) { int64_t tmp; if (!m_store->readValue(&tmp)) { if (!setDefaultValueHelper(outValue)) onError(); return; } outValue = static_cast<uint16_t>(tmp); }
	void readValue(uint32_t& outValue) { int64_t tmp; if (!m_store->readValue(&tmp)) { if (!setDefaultValueHelper(outValue)) onError(); return; } outValue = static_cast<uint32_t>(tmp); }
	void readValue(uint64_t& outValue) { int64_t tmp; if (!m_store->readValue(&tmp)) { if (!setDefaultValueHelper(outValue)) onError(); return; } outValue = static_cast<uint64_t>(tmp); }
	void readValue(float& outValue) { double tmp; if (!m_store->readValue(&tmp)) { if (!setDefaultValueHelper(outValue)) onError(); return; } outValue = static_cast<float>(tmp); }
	void readValue(double& outValue) { double tmp; if (!m_store->readValue(&tmp)) { if (!setDefaultValueHelper(outValue)) onError(); return; } outValue = static_cast<double>(tmp); }
	void readValue(String& outValue) { if (!m_store->readValue(&outValue)) { if (!setDefaultValueHelper(outValue)) onError(); return; } }

	// メンバ関数として serialize を持つ型の readValue()
	template<
		typename TValue,
		typename std::enable_if<detail::has_member_serialize_function<TValue>::value, std::nullptr_t>::type = nullptr>
	void readValue(TValue& outValue)
	{
		// ユーザーオブジェクトのシリアライズの場合、実際にコンテナ内に入るのは最初の値が書かれてからになる。
		// しかし、デフォルト値のためのプロパティ存在チェックはそのタイミングだと遅い。ここで行う。
		//auto* nvp = static_cast<NameValuePair<TValue>*>(m_nodeInfoStack.top().lastNVP);
		//if (nvp &&
		//	nvp->defaultValue &&
		//	!m_store->hasKey(nvp->name))
		//{
		//	outValue = *nvp->defaultValue;
		//	return;
		//}

		bool baseCall = m_nodeInfoStack.top().nextBaseCall;
		m_nodeInfoStack.push(NodeInfo{});

		if (baseCall)
			outValue.TValue::serialize(*this);
		else
			outValue.serialize(*this);

		postContainerRead();
	}

	// メンバ関数として serialize を持たない型の readValue()
	template<
		typename TValue,
		typename std::enable_if<detail::non_member_serialize_function<TValue>::value, std::nullptr_t>::type = nullptr>
	void readValue(TValue& outValue)
	{
		// ユーザーオブジェクトのシリアライズの場合、実際にコンテナ内に入るのは最初の値が書かれてからになる。
		// しかし、デフォルト値のためのプロパティ存在チェックはそのタイミングだと遅い。ここで行う。
		//auto* nvp = static_cast<NameValuePair<TValue>*>(m_nodeInfoStack.top().lastNVP);
		//if (nvp &&
		//	nvp->defaultValue &&
		//	!m_store->hasKey(nvp->name))
		//{
		//	outValue = *nvp->defaultValue;
		//	return;
		//}

		m_nodeInfoStack.push(NodeInfo{});

		serialize(*this, outValue);

		postContainerRead();
	}

	void postContainerRead()
	{
		if (m_nodeInfoStack.top().headState == NodeHeadState::Ready)
		{
			// 空オブジェクトをシリアライズした。コンテナを開始していないので開始する
			moveState(NodeHeadState::Object);
		}
		if (m_nodeInfoStack.top().headState != NodeHeadState::ContainerOpend)
		{
			preReadValue();
		}


		if (m_nodeInfoStack.top().headState == NodeHeadState::ContainerOpend)
			m_store->readContainerEnd();
		m_nodeInfoStack.pop();
		m_nodeInfoStack.top().nextBaseCall = false;
	}

	void readClassVersion()
	{
		m_store->setNextName(ClassVersionKey);
		int64_t version;
		if (m_store->readValue(&version))
		{
			m_nodeInfoStack.top().classVersion = static_cast<int>(version);
		}
	}

	void onError()
	{
		LN_UNREACHABLE();	// TODO
	}

	ArchiveStore* m_store;
	ArchiveMode m_mode;
	std::stack<NodeInfo>	m_nodeInfoStack;
	int64_t	m_archiveVersion;
};


#define LN_NVP2(var, ...)		::ln::makeNVP(_LT(#var), var, ##__VA_ARGS__)

template<typename TValue>
NameValuePair<TValue> makeNVP(const StringRef& name, TValue& valueRef)
{
	return NameValuePair<TValue>(name, &valueRef);
}

template<typename TValue>
NameValuePair<TValue> makeNVP(const StringRef& name, TValue& valueRef, const TValue& defaultValue)
{
	return NameValuePair<TValue>(name, &valueRef, defaultValue);
}


class JsonTextOutputArchive
	: public Archive2
{
public:
	JsonTextOutputArchive();
	virtual ~JsonTextOutputArchive();

	template<typename TValue>
	void save(TValue && value)
	{
		if (LN_REQUIRE(!m_processing)) return;
		m_processing = true;
		Archive2::process(std::forward<TValue>(value));
		m_processing = false;
	}

	String toString(tr::JsonFormatting formatting = tr::JsonFormatting::Indented);

private:
	tr::JsonDocument2	m_localDoc;
	JsonArchiveStore	m_localStore;
	bool				m_processing;
};


class JsonTextInputArchive
	: public Archive2
{
public:
	JsonTextInputArchive(const String& jsonText);
	virtual ~JsonTextInputArchive();

	template<typename TValue>
	void load(TValue && value)
	{
		if (LN_REQUIRE(!m_processing)) return;
		m_processing = true;
		Archive2::process(std::forward<TValue>(value));
		m_processing = false;
	}

private:
	tr::JsonDocument2	m_localDoc;
	JsonArchiveStore	m_localStore;
	bool				m_processing;
};

LN_NAMESPACE_END

#include "SerializeTypes.h"
