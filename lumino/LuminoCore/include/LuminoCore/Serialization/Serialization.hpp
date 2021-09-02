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

	int classVersion() const { return m_nodeInfoStack.back().classVersion; }

    void setBasePath(const String& path) { m_basePath = path; }
    const String& basePath() const { return m_basePath; }

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

	void makeArrayTag(int* outSize);
	void makeMapTag(int* outSize);

	template<typename TKey, typename TValue>
	void makeMapItem(const TKey& key, TValue& value)
	{
		if (isSaving()) {
			process(makeNVP(key, value));
		}
		else {
		}
	}

	template<typename TKey, typename TValue>
	void makeMapItem(TKey& key, TValue& value)
	{
		if (isSaving()) {
		}
		else {
			key = m_store->memberKey(m_nodeInfoStack.back().arrayIndex);
			NameValuePair<TValue> nvp = makeNVP<TValue>(key, value);
			processLoadNVP<TValue>(nvp);
			m_nodeInfoStack.back().arrayIndex++;	// container が Array ではないので、processLoad() 内ではインクリメントされない。ここでインクリメントする。
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
	//template<class TKey, class TValue>
	//void makeObjectMember(TKey& key, TValue& value)
	//{
	//	if (isSaving())
	//	{
	//	}
	//	else
	//	{
	//		key = m_store->memberKey(m_nodeInfoStack.top().arrayIndex);
	//		processLoad(makeNVP(key, value));
	//	}
	//}

	/*
	Note: オブジェクトを単純な文字列としてシリアライズする場合に使用します。
	この関数を呼び出した serialize() 内では他の値をシリアライズすることはできません。
	*/
	void makeStringTag(String* str)
	{
        if (isSaving()) {
            moveState(NodeHeadState::PrimitiveValue);
            process(*str);
        }
        else {

            //  process は、いま open しているコンテナに対して行いたい。
            // ここで閉じて、次に使えるようにする。current は閉じたコンテナになる。
            //m_store->closeContainer();
            //m_nodeInfoStack.back().containerOpend = false;

            // Value 確定。次の process で open container しない。
            moveState(NodeHeadState::PrimitiveValue);
            process(*str);
        }
    }

    // 呼出し後、 save, load 共に、null の場合は process してはならない
	void makeSmartPtrTag(bool* outIsNull)
	{
        if (isSaving()) {
            if ((*outIsNull)) {
                processNull();
            }
			else {
				moveState(NodeHeadState::WrapperObject);
			}
        }
        else if (isLoading()) {

            //moveState(NodeHeadState::Object);
            tryOpenContainer();
            *outIsNull = m_store->getOpendContainerType() == ArchiveContainerType::Null;
            moveState(NodeHeadState::WrapperObject);
			//if ((*outIsNull)) {
			//	moveState(NodeHeadState::PrimitiveValue);
			//}
			//else {
			//	moveState(NodeHeadState::WrapperObject);
			//}
        }
	}

	void makeOptionalTag(bool* outHasValue)
	{
		if (isSaving()) {
			if (!(*outHasValue)) {
				processNull();
			}
			else {
				moveState(NodeHeadState::WrapperObject);
			}
		}
		else if (isLoading()) {
            *outHasValue = m_store->getReadingValueType() != ArchiveNodeType::Null;


            //tryOpenContainer();
            //*outHasValue = m_store->getOpendContainerType() != ArchiveContainerType::Null;

            // makeOptionalTag を抜けた後の process は、いま open しているコンテナに対して行いたい。
            // ここで閉じて、次に使えるようにする。current は閉じたコンテナになる。
            //m_store->closeContainer();
            //m_nodeInfoStack.back().containerOpend = false;

            moveState(NodeHeadState::WrapperObject);

			//*outHasValue = m_store->getReadingValueType() != ArchiveNodeType::Null;
			//if (!(*outHasValue)) {
			//	moveState(NodeHeadState::PrimitiveValue);
			//}
			//else {
			//	moveState(NodeHeadState::WrapperObject);
			//}
		}
	}

	// type: in,out
    void makeVariantTag(ArchiveNodeType* type);

	// 事前に makeSmartPtrTag 必須
    void makeTypeInfo(String* value);

	// Ref<> or Optional<> 専用。state は遷移済み。
	void processNull()
	{
		moveState(NodeHeadState::PrimitiveValue);
		preWriteValue();
		writeValueNull();
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

        if (m_mode == ArchiveMode::Load) {
            m_store->setupLoad();

            // ルートノードも、内部的には serialize が呼ばれた直後の状態で process を開始したいので、1 push
            //m_nodeInfoStack.push_back(NodeInfo{});
            m_current = NodeInfo{};
        }
	}

private:

	// Node は "key": の右の部分や配列 [ ] の要素など、「値」の部分を示す。
	enum class NodeHeadState
	{
		Ready,			// 値の書き始め。"key": の直後や Array メンバ。まだ Node が Value か Array か Object かわからない。
		Object,			// Object confirmed
		Array,			// Array confirmed
		PrimitiveValue,			// Value confirmed (int, bool, null, string など、{} や [] ではないもの)
		WrapperObject,	// Ref<>, Optional<> など。実際にコンテナを open するかは innter type による。なので、PrimitiveValue　へ遷移可能。
						// save 時はダミーノードとして振る舞う。（出力される JSON は Ref<Class1> でひとつのノードだが、内部的には2つの Node で管理する。そうしないと Node 自体が「深さ」を管理する必要があり複雑になる）
		//InnerObject,	// Ref<>, Optional<> などの
		//ContainerOpend,	// Object または Array を開始した (Value の場合はこの状態にはならない)
	};

	// stack の要素。Container Node (Object or Array) の時に作られる。
    // ×(Load では) serialize を呼ぶ前に、コンテナであろうと値であろうと必ず作る ()
    // ×(Load では) store の container を open したときに push する。Value も push してしまうと、makeSmartPtrTag で開いていいのかどうかを判断するのに上へ検索掛ける必要がある
	//      makeSmartPtr -> process(NVP) したとき、両方で open しようとするが、常に同期して作り続けると「makeSmartPtr でもう open したので NVP では必要ない」みたいな状態管理ができなくなる。
    // 現在のノードの「ひとつうえのコンテナ」を表す。readValue 時に必ず push するが、これは次に serialize する値を表すのではない。
    struct NodeInfo
	{
		NodeHeadState headState = NodeHeadState::Ready;
		int arrayIndex = 0;
		int classVersion = 0;// 
		ln::String typeInfo;	// 
		bool root = false;
		bool containerOpend = false;	// m_store に、コンテナ開始タグの書き込み通知を行ったかどうか
		bool nextBaseCall = false;
		bool parentIsOpendWrapper = false;
		//bool innterType = false;


        // load 時に使う。
        ArchiveContainerType readingContainerType = ArchiveContainerType::Null;
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
		m_nodeInfoStack.back().nextBaseCall = true;
		m_store->setNextName(ClassBaseKey);
		writeValue(*base.basePtr);
		return true;
	}

	template<typename TValue>
	bool processSave(TValue& value)
	{
		//if (detail::ArchiveValueTraits<TValue>::isPrimitiveType()) {
		//	moveState(NodeHeadState::PrimitiveValue);
		//}
		//moveState((detail::ArchiveValueTraits<TValue>::isPrimitiveType()) ? NodeHeadState::PrimitiveValue : NodeHeadState::Object);
		preWriteValue();
		writeValue(value);
		return true;
	}

	bool preWriteValue()
	{
		//if (LN_REQUIRE(!m_nodeInfoStack.empty())) return false;	// not root node opend.
		if (m_nodeInfoStack.empty()) {
			// ルートノードの場合は NVP や Tag が何もセットされていない。
			// (これ用のダミーノードを作っても Ready にしか遷移しないので無駄)
			return true;
		}

		switch (m_nodeInfoStack.back().headState)
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
			if (!m_nodeInfoStack.back().containerOpend) {
				//if (m_nodeInfoStack.size() >= 2 && m_nodeInfoStack[m_nodeInfoStack.size() - 2].headState == NodeHeadState::WrapperObject) {
				//	// 親が WrapperObject ならコンテナを共有するので何もしない
				//}
				//else {
					m_store->writeObject();
					m_nodeInfoStack.back().containerOpend = true;
					writeClassVersion();
					writeTypeInfo();
				//}
			}
			return true;

		case NodeHeadState::Array:
			if (!m_nodeInfoStack.back().containerOpend) {
				m_store->writeArray();
				m_nodeInfoStack.back().containerOpend = true;
			}
			return true;

		case NodeHeadState::PrimitiveValue:
			return true;

		case NodeHeadState::WrapperObject:
			// ここではまだコンテナを開けることはできない。
			// Optional<List<>> の時、コンテナが List であるかは List の serialize に入らなければわからない。
			// read ではこの時点で open しないと TypeInfo などのメタ情報が読み取れないので、write と read でちょっとタイミングが違う点に注意。

			//if (!m_nodeInfoStack.back().containerOpend) {
			//	m_store->writeObject();
			//	m_nodeInfoStack.back().containerOpend = true;
			//	writeClassVersion();
			//	writeTypeInfo();
			//}
            return true;

		default:
			LN_UNREACHABLE();
			return false;
		}
	}

    // TORO: require
	bool moveState(NodeHeadState req)
	{
		if (LN_REQUIRE(!m_nodeInfoStack.empty())) return false;	// not root node opend.

		switch (m_nodeInfoStack.back().headState)
		{
		case NodeHeadState::Ready:
			m_nodeInfoStack.back().headState = req;	// always allow
			break;

		case NodeHeadState::Object:
		case NodeHeadState::Array:
		case NodeHeadState::PrimitiveValue:
			if (m_nodeInfoStack.back().headState == req) {
				// not transition
				return true;
			}
			// transition is prohibited.
			onError();
			return false;

		case NodeHeadState::WrapperObject:
            if (m_nodeInfoStack.back().headState == req) {
                // not transition
                return true;
            }
			if (req == NodeHeadState::PrimitiveValue) {
				// Optional<int> など、inner type が primitive な場合
				m_nodeInfoStack.back().headState = req;
				return true;
			}
            if (req == NodeHeadState::Object) {
                // WrapperObject の中で MVP serialize したときに Object に遷移しようとするが、Wrapper のままとする
                return true;
            }
			// transition is prohibited.
			onError();
			return false;

		//case NodeHeadState::WrapperObject:
		//	if (req == NodeHeadState::Object || req == NodeHeadState::Array) {
		//		// Ref<> の内部オブジェクトの process または makeArrayTag による遷移を許可する。
		//		// 
		//		return true;
		//	}
		//	// transition is prohibited.
		//	onError();
		//	return false;

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
		bool baseCall = (m_nodeInfoStack.empty()) ? false : m_nodeInfoStack.back().nextBaseCall;

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
		m_nodeInfoStack.push_back(NodeInfo{});
		m_nodeInfoStack.back().classVersion = getClassVersion<TValue>();
	}

    void popNodeWrite();

	void writeClassVersion()
	{
		if (m_nodeInfoStack.back().classVersion > 0)
		{
			m_store->setNextName(ClassVersionKey);
			writeValue(m_nodeInfoStack.back().classVersion);
		}
	}

	void writeTypeInfo()
	{
		NodeInfo* parentNode = (m_nodeInfoStack.size() >= 2) ? &m_nodeInfoStack[m_nodeInfoStack.size() - 2] : nullptr;
		if (parentNode && !parentNode->typeInfo.isEmpty())
		{
			m_store->setNextName(u"_type");
			writeValue(parentNode->typeInfo);
		}
	}

	//-----------------------------------------------------------------------------
	// Load

	template<typename TValue>
	void processLoad(NameValuePair<TValue>& nvp)
	{
		processLoadNVP(nvp);
	}

	template<typename TValue>
	void processLoadNVP(NameValuePair<TValue>& nvp)
	{
		moveState(NodeHeadState::Object);   // Current node は Object confirmed.
		tryOpenContainer();

		preReadValue();

		if (m_store->hasKey(nvp.name) || nvp.hasDefaultValue())	// preReadValue() で Store の状態がコンテナ内に入るので、そのあとで存在確認
		{
			moveState(NodeHeadState::Object);	// BaseClass は Object のシリアライズの一部なので、親ノードは必ず Object

			m_store->moveToNamedMember(nvp.name);
			m_nextReadValueDefault = &nvp;
			readValue(*nvp.value);
			postReadValue();
		}
	}

	template<typename TValue>
	void processLoad(BaseClass<TValue>& base)
	{
		//moveState(NodeHeadState::Object);
        //confirmObjectContainerAndOpen();


        moveState(NodeHeadState::Object);   // Current node は Object confirmed.
        tryOpenContainer();
		preReadValue();


		m_nodeInfoStack.back().nextBaseCall = true;
		m_store->moveToNamedMember(ClassBaseKey);
		readValue(*base.basePtr);
		postReadValue();
	}

	template<typename TValue>
	void processLoad(TValue& value)
	{
		//if (detail::ArchiveValueTraits<TValue>::isPrimitiveType()) {
		//	moveState(NodeHeadState::PrimitiveValue);
		//}
        //confirmObjectContainerAndOpen();

        // ここでの try open は不要。object か array を確定づけるものは NVP か makeArrayTag

		preReadValue();
		readValue(value);
		postReadValue();
	}

	bool preReadValue()
	{

        //if (m_store->getOpendContainerType() == ArchiveContainerType::Array/* &&
        //    m_nodeInfoStack.back().headState == NodeHeadState::Array*/)
        if (!m_nodeInfoStack.empty() && m_nodeInfoStack.back().headState == NodeHeadState::Array)
        {
            m_store->moveToIndexedMember(m_nodeInfoStack.back().arrayIndex);
        }

#if 0
		if (m_nodeInfoStack.empty()) {
			// write と同じく、ルートノードは Ready 状態以外になりようがない。
			return true;
		}


		// この時点で stack.top は今回読もうとしているオブジェクトの1つ上のコンテナを指している

        switch (m_nodeInfoStack.back().headState)
        {
        case NodeHeadState::Ready:
            break;
        case NodeHeadState::Object:
            if (!m_nodeInfoStack.back().containerOpend) {
				if (m_nodeInfoStack.size() >= 2 && m_nodeInfoStack[m_nodeInfoStack.size() - 2].headState == NodeHeadState::WrapperObject) {
					// 親が WrapperObject ならコンテナを共有するので何もしない
				}
				else {
					// setTagXXXX に備えて、コンテナか値かはまだ確定していないことがある。
					// 確定していないにも関わらず読み取ろうとした場合はコンテナ確定。
					m_store->readContainer();

					// verify
					if (LN_ENSURE(m_store->getContainerType() == ArchiveContainerType::Object)) return false;

					m_nodeInfoStack.back().containerOpend = true;
				}
            }
            break;
        case NodeHeadState::Array:
            if (!m_nodeInfoStack.back().containerOpend) {
				if (m_nodeInfoStack.size() >= 2 && m_nodeInfoStack[m_nodeInfoStack.size() - 2].headState == NodeHeadState::WrapperObject) {
					// 親が WrapperObject ならコンテナを共有するので何もしない
				}
				else {
					// setTagXXXX に備えて、コンテナか値かはまだ確定していないことがある。
					// 確定していないにも関わらず読み取ろうとした場合はコンテナ確定。
					m_store->readContainer();

					// verify
					if (LN_ENSURE(m_store->getContainerType() == ArchiveContainerType::Array)) return false;

					m_nodeInfoStack.back().containerOpend = true;
				}
            }
            break;
        case NodeHeadState::PrimitiveValue:
            break;
		case NodeHeadState::WrapperObject:
			if (m_store->getReadingValueType() == ArchiveNodeType::Array || m_store->getReadingValueType() == ArchiveNodeType::Object) {

				if (!m_nodeInfoStack.back().containerOpend) {
					m_store->readContainer();
					if (LN_ENSURE(m_store->getContainerType() == ArchiveContainerType::Object || m_store->getContainerType() == ArchiveContainerType::Array)) return false;
					m_nodeInfoStack.back().containerOpend = true;
				}
			}
            break;
        default:
            LN_UNREACHABLE();
            return false;
        }
#endif

		return true;
	}

	// after pop value node. stack top refers to parent container.
	void postReadValue()
	{
        //if (m_store->getOpendContainerType() == ArchiveContainerType::Array)
        if (!m_nodeInfoStack.empty() && m_nodeInfoStack.back().headState == NodeHeadState::Array)
        {
            m_nodeInfoStack.back().arrayIndex++;
        }


		//if (!m_nodeInfoStack.empty())
		//{
		//	m_nodeInfoStack.back().arrayIndex++;

		//	if (m_store->getContainerType() == ArchiveContainerType::Array)
		//	{
		//		m_store->setNextIndex(m_nodeInfoStack.back().arrayIndex);
		//	}
		//}
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
		bool baseCall = (m_nodeInfoStack.empty()) ? false : m_nodeInfoStack.back().nextBaseCall;
		preLoadSerialize();

        // TValue が ValueType であっても、serialize の階層を測りたいので、serialize を呼ぶ前に必ず push
        if (!m_nodeInfoStack.empty() && m_nodeInfoStack.back().headState == NodeHeadState::WrapperObject && m_nodeInfoStack.back().containerOpend) {
            m_current.parentIsOpendWrapper = true;
        }
        m_nodeInfoStack.push_back(m_current);
        m_current = NodeInfo{}; // Ready で始める

		if (baseCall)
			outValue.TValue::serialize(*this);
		else
			outValue.serialize(*this);

		postLoadSerialize();
	}

	// メンバ関数として serialize を持たない型の readValue()
	template<
		typename TValue,
		typename std::enable_if<detail::non_member_serialize_function<TValue>::value, std::nullptr_t>::type = nullptr>
	void readValue(TValue& outValue)
	{
		preLoadSerialize();

        // TValue が ValueType であっても、serialize の階層を測りたいので、serialize を呼ぶ前に必ず push
        if (!m_nodeInfoStack.empty() && m_nodeInfoStack.back().headState == NodeHeadState::WrapperObject && m_nodeInfoStack.back().containerOpend) {
            m_current.parentIsOpendWrapper = true;
        }
        m_nodeInfoStack.push_back(m_current);
        m_current = NodeInfo{};

		serialize(*this, outValue);

		postLoadSerialize();
	}


    bool preLoadSerialize()
    {
        //NodeHeadState parentState = NodeHeadState::Ready;
        //if (!m_nodeInfoStack.empty()) {
        //    parentState = m_nodeInfoStack.back().headState;
        //}
/*
        ArchiveContainerType type = m_store->getOpendContainerType();

        NodeInfo node;
        node.readingContainerType = type;
        m_nodeInfoStack.push_back(node);*/


        ////if (type == ArchiveNodeType::Array || type == ArchiveNodeType::Object) {
        //if (parentState == NodeHeadState::WrapperObject) {
        //    // ひとつ前の process 直前で open された状態を維持する。
        //}
        //else {
        //    if (!m_nodeInfoStack.back().containerOpend) {
        //        if (!m_store->openContainer()) {
        //            return false;
        //        }
        //        //if (LN_ENSURE(m_store->getContainerType() == ArchiveContainerType::Object || m_store->getContainerType() == ArchiveContainerType::Array)) return false;
        //        m_nodeInfoStack.back().containerOpend = true;
        //    }
        //}



        return true;
    }

	void postLoadSerialize()
	{
		//if (m_nodeInfoStack.top().headState == NodeHeadState::Ready)
		//{
		//	// serialized empty object. start container, has not been started.
		//	preReadValue();
		//}


		// 空の serialize を呼び出した場合、state は変わっていない。
		// 空の Object として扱いたいので、ここで Object 状態にしておく。
		if (m_nodeInfoStack.back().headState == NodeHeadState::Ready) {
			moveState(NodeHeadState::Object);
		}

		// serialize が空実装ではないが、makeArrayTag など Tag 設定だけして子値の process を行わなかった場合はコンテナ開始タグが読み込まれていないため、ここで読み込む。
		//if (m_nodeInfoStack.back().headState == NodeHeadState::Object ||
		//	m_nodeInfoStack.back().headState == NodeHeadState::Array) {
		//	if (!m_nodeInfoStack.back().containerOpend) {
		//		preReadValue();
		//	}
		//}


		//bool taggedValueObject = (m_nodeInfoStack.top().headState == NodeHeadState::PrimitiveValue);

		if (m_nodeInfoStack.back().containerOpend) {
			m_store->closeContainer();
		}

        // top は捨てる。捨てないと今の状態 (例えば、State::Array とか が m_current になってしまう。それはいらない。)
        m_current = NodeInfo{};// m_nodeInfoStack.back();
		m_nodeInfoStack.pop_back();

		if (!m_nodeInfoStack.empty()) {
			m_nodeInfoStack.back().nextBaseCall = false;
		}
	}

    //bool confirmObjectContainerAndOpen()
    //{
    //    moveState(NodeHeadState::Object);

    //    NodeHeadState parentState = NodeHeadState::Ready;
    //    if (m_nodeInfoStack.size() >= 2) {
    //        parentState = m_nodeInfoStack.back().headState;
    //    }

    //    //if (type == ArchiveNodeType::Array || type == ArchiveNodeType::Object) {
    //    if (parentState == NodeHeadState::WrapperObject) {
    //        // ひとつ前の process 直前で open された状態を維持する。
    //    }
    //    else if (m_nodeInfoStack.back().headState == NodeHeadState::PrimitiveValue) {
    //    }
    //    else {
    //        if (!m_nodeInfoStack.back().containerOpend) {
    //            if (!m_store->openContainer()) {
    //                return false;
    //            }
    //            //if (LN_ENSURE(m_store->getContainerType() == ArchiveContainerType::Object || m_store->getContainerType() == ArchiveContainerType::Array)) return false;
    //            m_nodeInfoStack.back().containerOpend = true;
    //        }
    //    }

    //    return true;
    //}

    bool tryOpenContainer()
    {
        //NodeHeadState parentState = NodeHeadState::Ready;
        ////if (m_nodeInfoStack.size() >= 2) {
        //if (m_nodeInfoStack.empty()) {
        //    parentState = m_nodeInfoStack.back().headState;
        //}

        //if (type == ArchiveNodeType::Array || type == ArchiveNodeType::Object) {
        //if (parentState == NodeHeadState::WrapperObject) {
        //    // ひとつ前の process 直前で open された状態を維持する。
        //}
        //else 
        if (m_nodeInfoStack.back().headState == NodeHeadState::PrimitiveValue ||
            m_nodeInfoStack.back().headState == NodeHeadState::WrapperObject ||
            m_nodeInfoStack.back().parentIsOpendWrapper) {
        }
        else {
            //m_nodeInfoStack.push_back(NodeInfo{});


            if (!m_nodeInfoStack.back().containerOpend) {
                if (!m_store->openContainer()) {
                    return false;
                }
                //if (LN_ENSURE(m_store->getContainerType() == ArchiveContainerType::Object || m_store->getContainerType() == ArchiveContainerType::Array)) return false;
                m_nodeInfoStack.back().containerOpend = true;
            }
        }

        return true;
    }

	void readClassVersion()
	{
		m_store->setNextName(ClassVersionKey);
		int64_t version;
		if (m_store->readValue(&version)) {
			m_nodeInfoStack.back().classVersion = static_cast<int>(version);
		}
	}

    const String& readTypeInfo();

	void onError(const char* message = nullptr)
	{
		LN_NOTIMPLEMENTED();
	}

	ArchiveStore* m_store;
	ArchiveMode m_mode;
	std::vector<NodeInfo>	m_nodeInfoStack;    // (Load) m_store ではなく serialize の呼び出し階層に対応する。
    NodeInfo m_current; // (Load) Value も含む。serialize の直前、current は m_nodeInfoStack に積まれ、新しい Node を current とする。
	int64_t	m_archiveVersion;
	detail::NameValuePairBase*	m_nextReadValueDefault = nullptr;
    String m_basePath;
};



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

    template<typename TValue>
    static String serialize(TValue&& value, const String& basePath, JsonFormatting formatting = JsonFormatting::Indented)
    {
        JsonTextOutputArchive ar;
        ar.setBasePath(basePath);
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

    template<typename TValue>
    static void deserialize(const StringRef& jsonText, const String& basePath, TValue& value)
    {
        JsonTextInputArchive ar(jsonText);
        ar.setBasePath(basePath);
        ar.load(value);
    }
};

} // namespace ln

#include "SerializeTypes.inl"
