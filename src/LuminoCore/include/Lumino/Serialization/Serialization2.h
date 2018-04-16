// Copyright (c) 2018 lriki. Distributed under the MIT license.

#pragma once
#include <type_traits>
#include <stack>
#include <unordered_map>
#include "String.h"
#include "../Json/JsonDocument.h"
#include "Common.hpp"
#include "ArchiveStore.h"

namespace ln {

// non‐intrusive
#define LN_SERIALIZE_CLASS_VERSION_NI(type, version) \
	namespace ln { namespace detail { \
		template<> struct SerializeClassVersionInfo<type> \
		{ \
			static const int value = version; \
		}; \
	} }

#define LN_SERIALIZE_CLASS_VERSION(version) \
	friend class ::ln::Archive; \
	static const int lumino_class_version = version;

#define LN_SERIALIZE_CLASS_FORMAT(type, format)	\
	namespace ln { namespace detail { \
		template<> struct SerializeClassFormatInfo<type> \
		{ \
			static const SerializeClassFormat value = format; \
		}; \
	} }

/**
 * オブジェクトのシリアライズ/デシリアライズを実装するためのインターフェイスです。
 */
class Archive
{
public:
	template<typename TBase>
	struct BaseClass
	{
		TBase* basePtr;
		BaseClass(TBase* ptr) : basePtr(ptr) {}
	};

	static const String ClassNameKey;
	static const String ClassVersionKey;
	static const String ClassBaseKey;

	Archive(ArchiveStore* store, ArchiveMode mode)
		: Archive()
	{
		setup(store, mode);
	}

	~Archive()
	{
	}

	bool isSaving() const { return m_mode == ArchiveMode::Save; }

	bool isLoading() const { return m_mode == ArchiveMode::Load; }

	int classVersion() const { return m_nodeInfoStack.top().classVersion; }

	template<typename T>
	Archive& operator & (T && value)
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
			//preReadValue(false);

			//preReadContainer();
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

	Archive()
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
		//NodeInfo node = {};
		//node.root = true;
		//node.headState = NodeHeadState::Object;
		//m_nodeInfoStack.push(node);

		//if (m_mode == ArchiveMode::Save)
		//{
		//	m_archiveVersion = ArchiveVersion;
		//	writeArchiveHeader();
		//}
		//else
		//{
		//	readArchiveHeader();
		//}
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
		bool root = false;
		bool nextBaseCall = false;
	};

	//-----------------------------------------------------------------------------
	// Save

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
		if (m_nodeInfoStack.empty()) {

		}
		else {
			auto& topNode = m_nodeInfoStack.top();

			if (topNode.headState == NodeHeadState::Object)
			{
				m_store->writeObject();
				writeClassVersion();
				moveState(NodeHeadState::ContainerOpend);
			}
			else if (topNode.headState == NodeHeadState::Array)
			{
				m_store->writeArray();
				moveState(NodeHeadState::ContainerOpend);
			}
		}
	}

	void moveState(NodeHeadState req)
	{
		if (m_nodeInfoStack.empty()) {

		}
		else {
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
		bool baseCall = (m_nodeInfoStack.empty()) ? false : m_nodeInfoStack.top().nextBaseCall;
		m_nodeInfoStack.push(NodeInfo{});
		m_nodeInfoStack.top().classVersion = getClassVersion<TValue>();//ln::detail::SerializeClassVersionInfo<TValue>::value;
		//m_nodeInfoStack.top().headState = (m_nodeInfoStack.size() == 1) ? NodeHeadState::ContainerOpend : NodeHeadState::Ready;

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
		//m_nodeInfoStack.top().headState = (m_nodeInfoStack.size() == 1) ? NodeHeadState::ContainerOpend : NodeHeadState::Ready;
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

		if (!m_nodeInfoStack.empty()) {
			m_nodeInfoStack.top().nextBaseCall = false;
		}
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

		m_store->setNextName(nvp.name);
		m_nextReadValueDefault = &nvp;
		preReadValue();
		readValue(*nvp.value);
	}

	template<typename TValue>
	void processLoad(BaseClass<TValue>& base)
	{
		moveState(NodeHeadState::Object);
		m_nodeInfoStack.top().nextBaseCall = true;
		m_store->setNextName(ClassBaseKey);
		preReadValue();
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
		if (!m_nodeInfoStack.empty())
		{
			if (m_store->getContainerType() == ArchiveContainerType::Array)
			{
				m_nodeInfoStack.top().arrayIndex++;
				m_store->setNextIndex(m_nodeInfoStack.top().arrayIndex);
			}
		}
	}

	void preReadContainer()
	{
		m_store->readContainer();

		if (m_nodeInfoStack.empty())
		{
		}
		else if (m_nodeInfoStack.top().headState == NodeHeadState::Object)
		{
			if (m_store->getContainerType() == ArchiveContainerType::Object)
				readClassVersion();
			moveState(NodeHeadState::ContainerOpend);
		}
		else if (m_nodeInfoStack.top().headState == NodeHeadState::Array)
		{
			moveState(NodeHeadState::ContainerOpend);
		}
		else if (m_nodeInfoStack.top().headState == NodeHeadState::ContainerOpend)
		{
			if (m_store->getContainerType() == ArchiveContainerType::Array)
			{
				//m_nodeInfoStack.top().arrayIndex++;
				//m_store->setNextIndex(m_nodeInfoStack.top().arrayIndex);
			}
		}
	}

	template<typename TValue>
	bool setDefaultValueHelper(TValue& outValue)
	{
		auto* nvp = static_cast<NameValuePair<TValue>*>(m_nextReadValueDefault);
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

		preReadContainer();

		bool baseCall = (m_nodeInfoStack.empty()) ? false : m_nodeInfoStack.top().nextBaseCall;
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
		preReadContainer();

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
		//if (m_nodeInfoStack.top().headState != NodeHeadState::ContainerOpend)
		//{
		//	preReadValue();
		//}


		//if (m_nodeInfoStack.top().headState == NodeHeadState::ContainerOpend)
		m_store->readContainerEnd();
		m_nodeInfoStack.pop();

		if (!m_nodeInfoStack.empty()) {
			m_nodeInfoStack.top().nextBaseCall = false;
		}
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
		LN_NOTIMPLEMENTED();
	}

	ArchiveStore* m_store;
	ArchiveMode m_mode;
	std::stack<NodeInfo>	m_nodeInfoStack;
	int64_t	m_archiveVersion;
	detail::NameValuePairBase*	m_nextReadValueDefault = nullptr;
};


#define LN_NVP(var, ...)		::ln::makeNVP(_LT(#var), var, ##__VA_ARGS__)

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
	: public Archive
{
public:
	JsonTextOutputArchive();
	virtual ~JsonTextOutputArchive();

	template<typename TValue>
	void save(TValue && value)
	{
		if (LN_REQUIRE(!m_processing)) return;
		m_processing = true;
		Archive::process(std::forward<TValue>(value));
		m_processing = false;
	}

	String toString(tr::JsonFormatting formatting = tr::JsonFormatting::None);

private:
	tr::JsonDocument2	m_localDoc;
	JsonArchiveStore	m_localStore;
	bool				m_processing;
};


class JsonTextInputArchive
	: public Archive
{
public:
	JsonTextInputArchive(const String& jsonText);
	virtual ~JsonTextInputArchive();

	template<typename TValue>
	void load(TValue && value)
	{
		if (LN_REQUIRE(!m_processing)) return;
		m_processing = true;
		Archive::process(std::forward<TValue>(value));
		m_processing = false;
	}

private:
	tr::JsonDocument2	m_localDoc;
	JsonArchiveStore	m_localStore;
	bool				m_processing;
};

/**
 * オブジェクトと JSON 文字列間のシリアライズ/デシリアライズ行うユーティリティです。
 */
class JsonSerializer
{
public:

	/**
	 * オブジェクトを JSON 文字列へシリアライズします。
	 * @param[in] 	value 		: データが格納されたオブジェクトへの参照
	 * @param[in] 	formatting	: JSON 文字列の整形方法
	 * @return		JSON 文字列
	 */
	template<typename TValue>
	static String serialize(TValue&& value, tr::JsonFormatting formatting = tr::JsonFormatting::None)
	{
		JsonTextOutputArchive ar;
		ar.save(std::forward<TValue>(value));
		return ar.toString(formatting);
	}

	/**
	 * JSON 文字列をオブジェクトへデシリアライズします。
	 * @param[in] 	jsonText	: JSON 文字列
	 * @param[in] 	value 		: データを格納するオブジェクトへの参照
	 */
	template<typename TValue>
	static void deserialize(const StringRef& jsonText, TValue& value)
	{
		JsonTextInputArchive ar(jsonText);
		ar.load(value);
	}
};

} // namespace ln

#include "SerializeTypes.h"
