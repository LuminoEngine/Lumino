// Copyright (c) 2018+ lriki. Distributed under the MIT license...

#pragma once
#include <type_traits>
#include <stack>
#include <unordered_map>
#include "../Base/String.hpp"
#include "../Json/JsonDocument.hpp"
#include "Common.hpp"
#include "ArchiveStore.hpp"

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
	template<typename T> friend class ::ln::detail::has_member_serialize_function; \
	template<typename T> friend class ::ln::detail::non_member_serialize_function; \
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

	// Entry point
	template <class T>
	void process(T && head)
	{
		//bool rootCalled = m_nodeInfoStack.empty();

		switch (m_mode)
		{
		case ArchiveMode::Save:
			//if (rootCalled) {
			//	pushNodeWrite<T>();
			//}

			processSave(head);

			//if (rootCalled) {
			//	popNodeWrite();
			//}
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
		//
		if (isSaving())
		{
			moveState(NodeHeadState::Array);
		}
		else if (isLoading())
		{
			moveState(NodeHeadState::Array);
			// ArrayContainer としてデシリアライズしている場合、この時点で size を返したいので、store を ArrayContainer まで移動して size を得る必要がある
			preReadValue();
			if (outSize) *outSize = m_store->getContainerElementCount();
		}
	}

	// 普通の nvp ではなく、キー名も自分で制御したいときに使う
	void makeObjectTag(int* outSize)
	{
		if (isSaving())
		{
			moveState(NodeHeadState::Object);
		}
		else if (isLoading())
		{
			moveState(NodeHeadState::Object);
			// store を Container まで移動して size を得る必要がある
			preReadValue();
			if (outSize) *outSize = m_store->getContainerElementCount();
		}
	}
	template<class TKey, class TValue>
	void makeObjectMember(TKey& key, TValue& value)
	{
		if (isSaving())
		{
		}
		else
		{
			key = m_store->memberKey(m_nodeInfoStack.top().arrayIndex);
			processLoad(makeNVP(key, value));
		}
	}

	/*
	Note: オブジェクトを単純な文字列としてシリアライズする場合に使用します。
	この関数を呼び出した serialize() 内では他の値をシリアライズすることはできません。
	*/
	void makeStringTag(String* str)
	{
		moveState(NodeHeadState::PrimitiveValue);
		process(*str);
	}

    // 呼出し後、 save, load 共に、null の場合は process してはならない
	void makeSmartPtrTag(bool* outIsNull)
	{
        if (isSaving()) {
            moveState(NodeHeadState::WrapperNode);
            if ((*outIsNull)) {
                processNull();
            }
        }
        else if (isLoading()) {
            moveState(NodeHeadState::WrapperNode);
            preReadValue();
            *outIsNull = m_store->getReadingValueType() == ArchiveNodeType::Null;
        }
	}

	void makeOptionalTag(bool* outHasValue)
	{
		if (isSaving()) {
			moveState(NodeHeadState::WrapperNode);
			if (!(*outHasValue)) {
				processNull();
			}
		}
		else if (isLoading()) {
			moveState(NodeHeadState::WrapperNode);
            preReadValue();
			*outHasValue = m_store->getReadingValueType() != ArchiveNodeType::Null;
		}
	}

	// type: in,out
	void makeVariantTag(ArchiveNodeType* type)
	{
		if (isSaving())
		{
			moveState(NodeHeadState::PrimitiveValue);
		}
		else if (isLoading())
		{
			moveState(NodeHeadState::PrimitiveValue);
			*type = m_store->getReadingValueType();
		}

	}

	void makeTypeInfo(ln::String* value)
	{
		if (isSaving()) {
			m_nodeInfoStack.top().typeInfo = *value;
		}
		else {
			*value = m_nodeInfoStack.top().typeInfo;
		}
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
	}

private:

	// Node は "key": の右の部分や配列 [ ] の要素など、「値」の部分を示す。
	enum class NodeHeadState
	{
		Ready,			// 値の書き始め。"key": の直後や Array メンバ。まだ Node が Value か Array か Object かわからない。
		Object,			// Object confirmed
		Array,			// Array confirmed
		PrimitiveValue,			// Value confirmed (int, bool, null, string など、{} や [] ではないもの)
		WrapperNode,	// Ref<>, Optional<> ...
		//ContainerOpend,	// Object または Array を開始した (Value の場合はこの状態にはならない)
	};

	// stack の要素。Container Node (Object or Array) の時に作られる。
	struct NodeInfo
	{
		NodeHeadState headState = NodeHeadState::Ready;
		int arrayIndex = 0;
		int classVersion = 0;// 
		ln::String typeInfo;	// 
		bool root = false;
		bool containerOpend = false;	// m_store に、コンテナ開始タグの書き込み通知を行ったかどうか
		bool nextBaseCall = false;
	};

	//-----------------------------------------------------------------------------
	// Save

	template<typename TValue>
	bool processSave(NameValuePair<TValue>& nvp)
	{
		if (!moveState(NodeHeadState::Object)) {	// これから { name, vaue } を追加するので、'Object' confirmed.
			return false;
		}
		preWriteValue();
		m_store->setNextName(nvp.name);
		writeValue(*nvp.value);
		return true;
	}

	template<typename TValue>
	bool processSave(BaseClass<TValue>& base)
	{
		if (!moveState(NodeHeadState::Object)) {	// BaseClass は Object のシリアライズの一部なので、'Object' confirmed.
			return false;
		}
		preWriteValue();
		m_nodeInfoStack.top().nextBaseCall = true;
		m_store->setNextName(ClassBaseKey);
		writeValue(*base.basePtr);
		return true;
	}

	template<typename TValue>
	bool processSave(TValue& value)
	{
		//moveState((detail::ArchiveValueTraits<TValue>::isPrimitiveType()) ? NodeHeadState::PrimitiveValue : NodeHeadState::Object);
		preWriteValue();
		writeValue(value);
		return true;
	}

	void processNull()
	{
		moveState(NodeHeadState::PrimitiveValue);
		preWriteValue();
		writeValueNull();
	}

	bool preWriteValue()
	{
		//if (LN_REQUIRE(!m_nodeInfoStack.empty())) return false;	// not root node opend.
		if (m_nodeInfoStack.empty()) {
			// ルートノードの場合は NVP や Tag が何もセットされていない。
			// (これ用のダミーノードを作っても Ready にしか遷移しないので無駄)
			return true;
		}

		switch (m_nodeInfoStack.top().headState)
		{
		case NodeHeadState::Ready:
			//if (m_nodeInfoStack.size() == 1) {
			//	// NVP や Tag が何もセットされていない。
			//	// これを許可するのはルートノードのみ。
			//	return true;
			//}
			//else {
				onError();	// NVP も Tag も事前にセットされていない。
				return false;
			//}

		case NodeHeadState::Object:
			if (!m_nodeInfoStack.top().containerOpend) {
				m_store->writeObject();
				m_nodeInfoStack.top().containerOpend = true;
				writeClassVersion();
				writeTypeInfo();
			}
			return true;

		case NodeHeadState::Array:
			if (!m_nodeInfoStack.top().containerOpend) {
				m_store->writeArray();
				m_nodeInfoStack.top().containerOpend = true;
			}
			return true;

		case NodeHeadState::PrimitiveValue:
			return true;

        case NodeHeadState::WrapperNode:
            // makeSmartPtrTag() などの直後の process().
            // コンテナ書き込みを開始したりはせず、子値を親オブジェクトの一部として書き込む。
            return true;

		default:
			LN_UNREACHABLE();
			return false;
		}
	}

	bool moveState(NodeHeadState req)
	{
		if (LN_REQUIRE(!m_nodeInfoStack.empty())) return false;	// not root node opend.

		switch (m_nodeInfoStack.top().headState)
		{
		case NodeHeadState::Ready:
			m_nodeInfoStack.top().headState = req;	// always allow
			break;

		case NodeHeadState::Object:
		case NodeHeadState::Array:
		case NodeHeadState::PrimitiveValue:
			if (m_nodeInfoStack.top().headState == req) {
				// not transition
				return true;
			}
			// transition is prohibited.
			onError();
			return false;

		default:
			LN_UNREACHABLE();
			return false;
		}

		return true;
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

	void writeValueNull() { m_store->writeValueNull(); }
	void writeValue(bool value) { m_store->writeValue(value); }
	void writeValue(int8_t value) {m_store->writeValue(static_cast<int64_t>(value)); }
	void writeValue(int16_t value) { m_store->writeValue(static_cast<int64_t>(value)); }
	void writeValue(int32_t value) { m_store->writeValue(static_cast<int64_t>(value)); }
	void writeValue(int64_t value) { m_store->writeValue(static_cast<int64_t>(value)); }
	void writeValue(uint8_t value) { m_store->writeValue(static_cast<int64_t>(value)); }
	void writeValue(uint16_t value) { m_store->writeValue(static_cast<int64_t>(value)); }
	void writeValue(uint32_t value) {  m_store->writeValue(static_cast<int64_t>(value)); }
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

		pushNodeWrite<TValue>();
		if (baseCall) {
			value.TValue::serialize(*this);
		}
		else {
			value.serialize(*this);
		}
		popNodeWrite();
	}

	// メンバ関数として serialize を持たない型の writeValue()
	template<
		typename TValue,
		typename std::enable_if<detail::non_member_serialize_function<TValue>::value, std::nullptr_t>::type = nullptr>
		void writeValue(TValue& value)
	{
		pushNodeWrite<TValue>();
		serialize(*this, value);
		popNodeWrite();
	}

	template<typename TValue>	// for class version
	void pushNodeWrite()
	{
		m_nodeInfoStack.push(NodeInfo{});
		m_nodeInfoStack.top().classVersion = getClassVersion<TValue>();
	}

	void popNodeWrite()
	{
		// 空の serialize を呼び出した場合、state は変わっていない。
		// 空の Object として扱いたいので、ここで Object 状態にしておく。
		if (m_nodeInfoStack.top().headState == NodeHeadState::Ready) {
			moveState(NodeHeadState::Object);
		}

		// serialize が空実装ではないが、makeArrayTag など Tag 設定だけして子値の process を行わなかった場合はコンテナ開始タグが書き込まれていないため、ここで書き込む。
        if (m_nodeInfoStack.top().headState == NodeHeadState::Object ||
            m_nodeInfoStack.top().headState == NodeHeadState::Array) {
            if (!m_nodeInfoStack.top().containerOpend) {
                preWriteValue();
            }
        }

		//bool taggedValueObject = (m_nodeInfoStack.top().headState == NodeHeadState::PrimitiveValue);
        bool containerOpend = m_nodeInfoStack.top().containerOpend;
        NodeHeadState nodeType = m_nodeInfoStack.top().headState;

		m_nodeInfoStack.pop();

		// Close containers
        if (containerOpend)
		{
			if (nodeType == NodeHeadState::Object) {
				m_store->writeObjectEnd();
			}
			else if (nodeType == NodeHeadState::Array) {
				m_store->writeArrayEnd();
			}
            else {
                LN_UNREACHABLE();
            }
		}

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

	void writeTypeInfo()
	{
		if (!m_nodeInfoStack.top().typeInfo.isEmpty())
		{
			m_store->setNextName(u"_type");
			writeValue(m_nodeInfoStack.top().typeInfo);
		}
	}

	//-----------------------------------------------------------------------------
	// Load

	template<typename TValue>
	void processLoad(NameValuePair<TValue>& nvp)
	{
		moveState(NodeHeadState::Object);	// Current node は Object confirmed.
		preReadValue();

		if (m_store->hasKey(nvp.name) || nvp.hasDefaultValue())	// preReadValue() で Store の状態がコンテナ内に入るので、そのあとで存在確認
		{
			moveState(NodeHeadState::Object);	// BaseClass は Object のシリアライズの一部なので、親ノードは必ず Object

			m_store->setNextName(nvp.name);
			m_nextReadValueDefault = &nvp;
			readValue(*nvp.value);
			postReadValue();
		}
	}

	template<typename TValue>
	void processLoad(BaseClass<TValue>& base)
	{
		moveState(NodeHeadState::Object);
		preReadValue();
		m_nodeInfoStack.top().nextBaseCall = true;
		m_store->setNextName(ClassBaseKey);
		readValue(*base.basePtr);
		postReadValue();
	}

	template<typename TValue>
	void processLoad(TValue& value)
	{
		preReadValue();
		readValue(value);
		postReadValue();
	}

	bool preReadValue()
	{
		if (m_nodeInfoStack.empty()) {
			// write と同じく、ルートノードは Ready 状態以外になりようがない。
			return true;
		}


		// この時点で stack.top は今回読もうとしているオブジェクトの1つ上のコンテナを指している

        switch (m_nodeInfoStack.top().headState)
        {
        case NodeHeadState::Ready:
            break;
        case NodeHeadState::Object:
            if (!m_nodeInfoStack.top().containerOpend) {
                // setTagXXXX に備えて、コンテナか値かはまだ確定していないことがある。
                // 確定していないにも関わらず読み取ろうとした場合はコンテナ確定。
                m_store->readContainer();

                // verify
                if (LN_ENSURE(m_store->getContainerType() == ArchiveContainerType::Object)) return false;

                m_nodeInfoStack.top().containerOpend = true;
                readTypeInfo();
            }
            break;
        case NodeHeadState::Array:
            if (!m_nodeInfoStack.top().containerOpend) {
                // setTagXXXX に備えて、コンテナか値かはまだ確定していないことがある。
                // 確定していないにも関わらず読み取ろうとした場合はコンテナ確定。
                m_store->readContainer();

                // verify
                if (LN_ENSURE(m_store->getContainerType() == ArchiveContainerType::Array)) return false;

				m_nodeInfoStack.top().containerOpend = true;
            }
            break;
        case NodeHeadState::PrimitiveValue:
            break;
        case NodeHeadState::WrapperNode:
            // Ref<> や Optional<> は、次の子値の最初の process でコンテナノードを読み取る
            break;
        default:
            LN_UNREACHABLE();
            return false;
        }

		return true;
	}

	// after pop value node. stack top refers to parent container.
	void postReadValue()
	{
		if (!m_nodeInfoStack.empty())
		{
			m_nodeInfoStack.top().arrayIndex++;

			if (m_store->getContainerType() == ArchiveContainerType::Array)
			{
				m_store->setNextIndex(m_nodeInfoStack.top().arrayIndex);
			}
		}
	}

	void preReadContainer()
	{
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
		preReadContainer();

		m_nodeInfoStack.push(NodeInfo{});

		serialize(*this, outValue);

		postContainerRead();
	}

	void postContainerRead()
	{
		//if (m_nodeInfoStack.top().headState == NodeHeadState::Ready)
		//{
		//	// serialized empty object. start container, has not been started.
		//	preReadValue();
		//}


		// 空の serialize を呼び出した場合、state は変わっていない。
		// 空の Object として扱いたいので、ここで Object 状態にしておく。
		if (m_nodeInfoStack.top().headState == NodeHeadState::Ready) {
			moveState(NodeHeadState::Object);
		}

		// serialize が空実装ではないが、makeArrayTag など Tag 設定だけして子値の process を行わなかった場合はコンテナ開始タグが読み込まれていないため、ここで読み込む。
		if (!m_nodeInfoStack.top().containerOpend) {
			preReadValue();
		}


		//bool taggedValueObject = (m_nodeInfoStack.top().headState == NodeHeadState::PrimitiveValue);

		if (m_nodeInfoStack.top().containerOpend) {
			m_store->readContainerEnd();
		}
		m_nodeInfoStack.pop();

		if (!m_nodeInfoStack.empty()) {
			m_nodeInfoStack.top().nextBaseCall = false;
		}
	}

	void readClassVersion()
	{
		m_store->setNextName(ClassVersionKey);
		int64_t version;
		if (m_store->readValue(&version)) {
			m_nodeInfoStack.top().classVersion = static_cast<int>(version);
		}
	}

	void readTypeInfo()
	{
		m_store->setNextName(u"_type");
		ln::String type;
		if (m_store->readValue(&type)) {
			m_nodeInfoStack.top().typeInfo = type;
		}
	}

	void onError(const char* message = nullptr)
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

	String toString(JsonFormatting formatting = JsonFormatting::Indented);

private:
	JsonDocument	m_localDoc;
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
	JsonDocument	m_localDoc;
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
	static String serialize(TValue&& value, JsonFormatting formatting = JsonFormatting::Indented)
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

#include "SerializeTypes.inl"
