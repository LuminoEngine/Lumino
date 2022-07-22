#pragma once
#include "../Base/Result.hpp"
#include "../Base/Uuid.hpp"
#include "../Serialization/Serialization.hpp"
#include "Common.hpp"
#include "Object.hpp"
#include "Variant.hpp"

namespace ln {
class Archive;

namespace detail {
class ArchiveStore3;
class JsonArchiveStore3;

    // void serialize(Archive& ar) をメンバ関数として持っているか
template<typename T>
class has_member_serialize_function
{
private:
	template<typename U>
	static auto check(U&& v) -> decltype(v.serialize(*reinterpret_cast<Archive*>(0)), std::true_type());
	static auto check(...) -> decltype(std::false_type());

public:
	typedef decltype(check(std::declval<T>())) type;
	static bool const value = type::value;
};

// void serialize(Archive& ar) をメンバ関数として持っていないか
template<typename T>
class non_member_serialize_function
{
private:
	template<typename U>
    static auto check(U&& v) -> decltype(v.serialize(*reinterpret_cast<Archive*>(0)), std::true_type());
	static auto check(...) -> decltype(std::false_type());

public:
	typedef decltype(check(std::declval<T>())) type;
	static bool const value = !type::value;
};
} // namespace detail

class SerializeException {
public:
    SerializeException(const String& message) : m_message(message) {}
	const String& message() const { return m_message; }

private:
    String m_message;
};

/**
 *
 * 
 * 
 * ポインタのシリアライズはできません。
 * スマートポインタのシリアライズは可能です。
 * ```
 * class MyObjectA {
 *     MyObject1* m_obj1;
 *     Ref<MyObject2> m_obj2;
 *     URef<MyObject3> m_obj2;
 *     void serialize(Serializer& ar) {
 *         ar & LN_NVP(m_obj1);     // MG!
 *         ar & LN_NVP(m_obj2);     // OK.
 *         ar & LN_NVP(m_obj3);     // OK.
 *     }
 * };
 * ```
 * 
 * ポリモーフィズムは Ref を使う場合のみ有効です。URef では無効です。
 */
class Archive : public Object {
public:
    template<typename TBase>
    struct BaseClass {
        TBase* basePtr;
        BaseClass(TBase* ptr)
            : basePtr(ptr) {}
    };

    static const String ClassNameKey;
    static const String ClassBaseKey;

    /** LN_SERIALIZE_VERSION() によるバージョン番号を示すプロパティ名を設定します。 */
    static void setDefaultVersionKey(const String& key) { s_defaultVersionKey = key; }
	
    /** LN_SERIALIZE_VERSION() によるバージョン番号を示すプロパティ名を取得します。 */
    static const String& defaultVersionKey(const String& key) { return s_defaultVersionKey; }

    Archive(ArchiveStore* store, ArchiveMode mode)
        : Archive() {
        setup(store, mode);
    }

    ~Archive() {
    }

    bool isSaving() const { return m_mode == ArchiveMode::Save; }

    bool isLoading() const { return m_mode == ArchiveMode::Load; }

    int classVersion() const { return currentNode()->classVersion; }

    void setBasePath(const String& path) { m_basePath = path; }
    const String& basePath() const { return m_basePath; }

    template<typename T>
    Archive& operator&(T&& value) {
        process(std::forward<T>(value));
        return *this;
    }

    // Entry point
    template<class T>
    void process(T&& head) {
        //bool rootCalled = m_nodeInfoStack.empty();

        switch (m_mode) {
            case ArchiveMode::Save:
                //if (rootCalled) {
                //    pushNodeWrite<T>();
                //}

                processSave(head);

                //if (rootCalled) {
                //    popNodeWrite();
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

    /*
    Note: オブジェクトを単純な文字列としてシリアライズする場合に使用します。
    この関数を呼び出した serialize() 内では他の値をシリアライズすることはできません。
    */
    void makePrimitiveValue();
    void makeStringTag(String* str);
    void makeArrayTag(int* outSize);
    void makeMapTag(int* outSize);

    template<typename TKey, typename TValue>
    void makeMapItem(const TKey& key, TValue& value) {
        if (isSaving()) {
            process(makeNVP(key, value));
        }
        else {
        }
    }

    template<typename TKey, typename TValue>
    void makeMapItem(TKey& key, TValue& value) {
        if (isSaving()) {
        }
        else {
            key = m_store->memberKey(m_nodeInfoStack.back().arrayIndex);
            NameValuePair<TValue> nvp = makeNVP<TValue>(key, value);
            processLoadNVP<TValue>(nvp);
            m_nodeInfoStack.back().arrayIndex++; // container が Array ではないので、processLoad() 内ではインクリメントされない。ここでインクリメントする。
        }
    }

    // 普通の nvp ではなく、キー名も自分で制御したいときに使う
    void makeObjectTag(int* outSize) {
        if (isSaving()) {
            setParentNodeType(NodeType::Object);
        }
        else if (isLoading()) {
            setParentNodeType(NodeType::Object);
            // store を Container まで移動して size を得る必要がある
            preReadValue();
            if (outSize) *outSize = m_store->getContainerElementCount();
        }
    }
    //template<class TKey, class TValue>
    //void makeObjectMember(TKey& key, TValue& value)
    //{
    //    if (isSaving())
    //    {
    //    }
    //    else
    //    {
    //        key = m_store->memberKey(m_nodeInfoStack.top().arrayIndex);
    //        processLoad(makeNVP(key, value));
    //    }
    //}


    // 呼出し後、 save, load 共に、null の場合は process してはならない
    void makeSmartPtrTag(bool* outIsNull) {
        if (isSaving()) {
            if ((*outIsNull)) {
                processNull();
            }
            else {
                setParentNodeType(NodeType::WrapperObject);
            }
        }
        else if (isLoading()) {

            //setParentNodeType(NodeType::Object);
            tryOpenContainer();
            *outIsNull = m_store->getOpendContainerType() == ArchiveContainerType::Null;
            setParentNodeType(NodeType::WrapperObject);
            //if ((*outIsNull)) {
            //    setParentNodeType(NodeType::PrimitiveValue);
            //}
            //else {
            //    setParentNodeType(NodeType::WrapperObject);
            //}
        }
    }

    void makeOptionalTag(bool* outHasValue) {
        if (isSaving()) {
            if (!(*outHasValue)) {
                processNull();
            }
            else {
                setCurrentNodeType(NodeType::WrapperObject);
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

            setCurrentNodeType(NodeType::WrapperObject);

            //*outHasValue = m_store->getReadingValueType() != ArchiveNodeType::Null;
            //if (!(*outHasValue)) {
            //    setParentNodeType(NodeType::PrimitiveValue);
            //}
            //else {
            //    setParentNodeType(NodeType::WrapperObject);
            //}
        }
    }

    // type: in,out
    void makeVariantTag(ArchiveNodeType* type);

    // 事前に makeSmartPtrTag 必須
    void makeTypeInfo(String* value);

    // Ref<> or Optional<> 専用。state は遷移済み。
    void processNull() {
        setCurrentNodeType(NodeType::PrimitiveValue);
        preWriteValue();
        writeValueNull();
    }

	/** serialize() の実行中に問題が発生した場合、それを通知するために呼び出すことができます。例外がスローされ、シリアライズ処理は直ちに終了します。 */
	//void fail(const char* message = nullptr);

	/** シリアライズ中のエラーの有無を確認します。 */
	bool hasError() const { return m_hasError; }

    /** シリアライズ中に発生したエラーメッセージを取得します。 */
	const String& errorMessage() const { return m_errorMessage; }

protected:
    Archive()
        : m_store(nullptr)
        , m_mode(ArchiveMode::Save)
        , m_archiveVersion(0)
        , m_hasError(false)
        , m_errorMessage() {
    }

    void setup(ArchiveStore* store, ArchiveMode mode) {
        m_store = store;
        m_mode = mode;

        if (m_mode == ArchiveMode::Load) {
            m_store->setupLoad();

            // ルートノードも、内部的には serialize が呼ばれた直後の状態で process を開始したいので、1 push
            //m_nodeInfoStack.push_back(NodeInfo{});
            m_current = NodeInfo{};
        }
    }
	
    void onError(const String& message = String::Empty) {
        m_hasError = true;
		if (message) {
			m_errorMessage = message;
		}
    }

private:
    // Node は "key": の右の部分や配列 [ ] の要素など、「値」の部分を示す。
    enum class NodeType {
        // serialize() に入るとき。まだ型は未確定。
        Ready,

        Object,         // Object confirmed
        Array,          // Array confirmed

        // Value confirmed (int, bool, null, string など、{} や [] ではないもの)
        // あるオブジェクトを string としてシリアライズしたいときは makeStringTag() でこの状態にできる。
        PrimitiveValue, 

        WrapperObject,  // Ref<>, Optional<> など。実際にコンテナを open するかは innter type による。なので、PrimitiveValue　へ遷移可能。
                        // save 時はダミーノードとして振る舞う。（出力される JSON は Ref<Class1> でひとつのノードだが、内部的には2つの Node で管理する。そうしないと Node 自体が「深さ」を管理する必要があり複雑になる）
                        //InnerObject,    // Ref<>, Optional<> などの
                        //ContainerOpend,    // Object または Array を開始した (Value の場合はこの状態にはならない)
    };

    // stack の要素。Container Node (Object or Array) の時に作られる。
    // Object::serialize() {} ブロックの情報と考えてよい。
    // - [Save] Object::serialize() 開始時点では、コンテナタイプは Object なのか Array なのかは確定していない。
    //          デフォルトは Object だが、makeArrayTag() などでカスタマイズできる。
    struct NodeInfo {
        NodeType headState = NodeType::Ready;
        int arrayIndex = 0;
        int classVersion = 0; //
        ln::String typeInfo;  //
        bool root = false;
        bool containerOpend = false; // m_store に、コンテナ開始タグの書き込み通知を行ったかどうか
        bool nextBaseCall = false;
        bool parentIsOpendWrapper = false;
        //bool innterType = false;

        // load 時に使う。
        ArchiveContainerType readingContainerType = ArchiveContainerType::Null;
    };

    //-----------------------------------------------------------------------------
    // Save

    template<typename TValue>
    bool processSave(NameValuePair<TValue>& nvp) {
        setCurrentNodeType(NodeType::Object);
        preWriteValue();
        m_store->setNextName(nvp.name);
        writeValue(*nvp.value);
        return true;
    }


    template<typename TValue>
    bool processSave(TValue& value) {
        //if (detail::ArchiveValueTraits<TValue>::isPrimitiveType()) {
        //    setParentNodeType(NodeType::PrimitiveValue);
        //}
        //setParentNodeType((detail::ArchiveValueTraits<TValue>::isPrimitiveType()) ? NodeType::PrimitiveValue : NodeType::Object);
        preWriteValue();
        writeValue(value);
        return true;
    }

    bool preWriteValue() {
        //if (LN_REQUIRE(!m_nodeInfoStack.empty())) return false;    // not root node opend.
        if (m_nodeInfoStack.empty()) {
            // ルートノードの場合は NVP や Tag が何もセットされていない。
            // (これ用のダミーノードを作っても Ready にしか遷移しないので無駄)
            return true;
        }

        NodeInfo* node = currentNode();

        switch (node->headState) {
            case NodeType::Ready:
                //if (m_nodeInfoStack.size() == 1) {
                //    // NVP や Tag が何もセットされていない。
                //    // これを許可するのはルートノードのみ。
                //    return true;
                //}
                //else {
                onError(); // NVP も Tag も事前にセットされていない。
                return false;
                //}

            case NodeType::Object:
                if (!node->containerOpend) {
                    //if (m_nodeInfoStack.size() >= 2 && m_nodeInfoStack[m_nodeInfoStack.size() - 2].headState == NodeType::WrapperObject) {
                    //    // 親が WrapperObject ならコンテナを共有するので何もしない
                    //}
                    //else {
                    m_store->writeObject();
                    node->containerOpend = true;
                    writeClassVersion(node);
                    writeTypeInfo();
                    //}
                }
                return true;

            case NodeType::Array:
                if (!node->containerOpend) {
                    m_store->writeArray();
                    node->containerOpend = true;
                }
                return true;

            case NodeType::PrimitiveValue:
                return true;

            case NodeType::WrapperObject:
                // ここではまだコンテナを開けることはできない。
                // Optional<List<>> の時、コンテナが List であるかは List の serialize に入らなければわからない。
                // read ではこの時点で open しないと TypeInfo などのメタ情報が読み取れないので、write と read でちょっとタイミングが違う点に注意。

                //if (!m_nodeInfoStack.back().containerOpend) {
                //    m_store->writeObject();
                //    m_nodeInfoStack.back().containerOpend = true;
                //    writeClassVersion();
                //    writeTypeInfo();
                //}
                return true;

            default:
                LN_UNREACHABLE();
                return false;
        }
    }

    // 検証しつつ、current の NodeType を変更する
    void setParentNodeType(NodeType type) {
        NodeInfo* parent = parentNode();
        if (parent->headState == NodeType::Ready) {
            LN_DCHECK(type != NodeType::Ready);
            parent->headState = type;
        }
        else if (parent->headState != type) {
            LN_UNREACHABLE();
        }
    }

    void setCurrentNodeType(NodeType type) {
        NodeInfo* current = currentNode();
        if (current->headState == NodeType::Ready) {
            LN_DCHECK(type != NodeType::Ready);
            current->headState = type;
        }
        else if (current->headState != type) {
            LN_UNREACHABLE();
        }
    }

    template<
        typename TValue,
        typename std::enable_if<detail::has_static_member_class_version<TValue>::value, std::nullptr_t>::type = nullptr>
    int getClassVersion() {
        return TValue::lumino_class_version;
    }

    template<
        typename TValue,
        typename std::enable_if<!detail::has_static_member_class_version<TValue>::value, std::nullptr_t>::type = nullptr>
    int getClassVersion() {
        return ln::detail::SerializeClassVersionInfo<TValue>::value;
    }

    void writeValueNull() { m_store->writeValueNull(); }
    void writeValue(bool value) { m_store->writeValue(value); }
    void writeValue(int8_t value) { m_store->writeValue(static_cast<int64_t>(value)); }
    void writeValue(int16_t value) { m_store->writeValue(static_cast<int64_t>(value)); }
    void writeValue(int32_t value) { m_store->writeValue(static_cast<int64_t>(value)); }
    void writeValue(int64_t value) { m_store->writeValue(static_cast<int64_t>(value)); }
    void writeValue(uint8_t value) { m_store->writeValue(static_cast<int64_t>(value)); }
    void writeValue(uint16_t value) { m_store->writeValue(static_cast<int64_t>(value)); }
    void writeValue(uint32_t value) { m_store->writeValue(static_cast<int64_t>(value)); }
    void writeValue(uint64_t value) { m_store->writeValue(static_cast<int64_t>(value)); }
    void writeValue(float value) { m_store->writeValue(static_cast<double>(value)); }
    void writeValue(double value) { m_store->writeValue(static_cast<double>(value)); }
    void writeValue(String& value) { m_store->writeValue(value); }

    template<class TValue>
    void writeValue(Ref<TValue>& value) {
        if (value) {
            pushNodeWrite<TValue>();
            setCurrentNodeType(NodeType::Object);

            const TypeInfo* typeInfo = TypeInfo::getTypeInfo(value);
            if (typeInfo && typeInfo != TypeInfo::getTypeInfo<Object>()) {
                // TODO: markXX は外側で serialize を拡張するためのものなので、内側でやるときはメモリ効率よくしたい (save 時は const 参照で)
                String typeName = typeInfo->name();
                currentNode()->typeInfo = typeName;
            }

            value->serialize(*this);
            popNodeWrite();
        }
        else {
            writeValueNull();
        }
    }

    template<>
    void writeValue(Ref<Variant>& value) {
        if (value) {
            pushNodeWrite<Variant>();
            setCurrentNodeType(NodeType::WrapperObject);
            value->serializeInternal3(*this, ArchiveNodeType::Null);
            popNodeWrite();
        }
        else {
            writeValueNull();
        }
    }
     
    // メンバ関数として serialize を持つ型の writeValue()
    template<
        typename TValue,
        typename std::enable_if<detail::has_member_serialize_function<TValue>::value, std::nullptr_t>::type = nullptr>
    void writeValue(TValue& value) // メンバ serialize() が const 関数とは限らないのでここは非 const 参照
    {
        pushNodeWrite<TValue>();
        value.serialize(*this);
        popNodeWrite();
    }

    // メンバ関数として serialize を持たない型の writeValue()
    template<
        typename TValue,
        typename std::enable_if<detail::non_member_serialize_function<TValue>::value, std::nullptr_t>::type = nullptr>
    void writeValue(TValue& value) {
        pushNodeWrite<TValue>();
        ::ln::serialize(*this, value);
        popNodeWrite();
    }

    template<typename TValue> // for class version
    void pushNodeWrite() {
        m_nodeInfoStack.push_back(m_current);
        m_current = NodeInfo{};
        m_current.classVersion = getClassVersion<TValue>();
    }

    void popNodeWrite();

    void writeClassVersion(NodeInfo* node) {
        if (node->classVersion > 0) {
            m_store->setNextName(s_defaultVersionKey);
            writeValue(node->classVersion);
        }
    }

    void writeTypeInfo() {
        NodeInfo* parentNode = (m_nodeInfoStack.size() >= 2) ? &m_nodeInfoStack[m_nodeInfoStack.size() - 2] : nullptr;
        if (parentNode && !parentNode->typeInfo.isEmpty()) {
            m_store->setNextName(_TT("_type"));
            writeValue(parentNode->typeInfo);
        }
    }

    //-----------------------------------------------------------------------------
    // Load

    template<typename TValue>
    void processLoad(NameValuePair<TValue>& nvp) {
        processLoadNVP(nvp);
    }

    template<typename TValue>
    void processLoadNVP(NameValuePair<TValue>& nvp) {
        setCurrentNodeType(NodeType::Object); // Current node は Object confirmed.
        tryOpenContainer();


        if (m_store->hasKey(nvp.name) || nvp.hasDefaultValue()) {
            preReadValue();
            m_store->moveToNamedMember(nvp.name);
            m_nextReadValueDefault = &nvp;
            readValue(*nvp.value);
            postReadValue();
        }
    }

    template<typename TValue>
    void processLoad(TValue& value) {
        //if (detail::ArchiveValueTraits<TValue>::isPrimitiveType()) {
        //    setParentNodeType(NodeType::PrimitiveValue);
        //}
        //confirmObjectContainerAndOpen();

        // ここでの try open は不要。object か array を確定づけるものは NVP か makeArrayTag

        preReadValue();
        readValue(value);
        postReadValue();
    }

    bool preReadValue() {
        NodeInfo* node = currentNode();
        if (node->headState == NodeType::Array) {
            m_store->moveToIndexedMember(node->arrayIndex);
        }
        return true;
    }

    // after pop value node. stack top refers to parent container.
    void postReadValue() {
        NodeInfo* node = currentNode();
        if (node->headState == NodeType::Array) {
            node->arrayIndex++;
        }
    }

    template<typename TValue>
    bool setDefaultValueHelper(TValue& outValue) {
        auto* nvp = static_cast<NameValuePair<TValue>*>(m_nextReadValueDefault);
        if (nvp &&
            nvp->defaultValue) {
            outValue = *nvp->defaultValue;
            return true;
        }
        return false;
    }

    void readValue(bool& outValue) {
        if (!m_store->readValue(&outValue)) {
            if (!setDefaultValueHelper(outValue)) onError();
            return;
        }
    }
    void readValue(int8_t& outValue) {
        int64_t tmp;
        if (!m_store->readValue(&tmp)) {
            if (!setDefaultValueHelper(outValue)) onError();
            return;
        }
        outValue = static_cast<int8_t>(tmp);
    }
    void readValue(int16_t& outValue) {
        int64_t tmp;
        if (!m_store->readValue(&tmp)) {
            if (!setDefaultValueHelper(outValue)) onError();
            return;
        }
        outValue = static_cast<int16_t>(tmp);
    }
    void readValue(int32_t& outValue) {
        int64_t tmp;
        if (!m_store->readValue(&tmp)) {
            if (!setDefaultValueHelper(outValue)) onError();
            return;
        }
        outValue = static_cast<int32_t>(tmp);
    }
    void readValue(int64_t& outValue) {
        int64_t tmp;
        if (!m_store->readValue(&tmp)) {
            if (!setDefaultValueHelper(outValue)) onError();
            return;
        }
        outValue = static_cast<int64_t>(tmp);
    }
    void readValue(uint8_t& outValue) {
        int64_t tmp;
        if (!m_store->readValue(&tmp)) {
            if (!setDefaultValueHelper(outValue)) onError();
            return;
        }
        outValue = static_cast<uint8_t>(tmp);
    }
    void readValue(uint16_t& outValue) {
        int64_t tmp;
        if (!m_store->readValue(&tmp)) {
            if (!setDefaultValueHelper(outValue)) onError();
            return;
        }
        outValue = static_cast<uint16_t>(tmp);
    }
    void readValue(uint32_t& outValue) {
        int64_t tmp;
        if (!m_store->readValue(&tmp)) {
            if (!setDefaultValueHelper(outValue)) onError();
            return;
        }
        outValue = static_cast<uint32_t>(tmp);
    }
    void readValue(uint64_t& outValue) {
        int64_t tmp;
        if (!m_store->readValue(&tmp)) {
            if (!setDefaultValueHelper(outValue)) onError();
            return;
        }
        outValue = static_cast<uint64_t>(tmp);
    }
    void readValue(float& outValue) {
        double tmp;
        if (!m_store->readValue(&tmp)) {
            if (!setDefaultValueHelper(outValue)) onError();
            return;
        }
        outValue = static_cast<float>(tmp);
    }
    void readValue(double& outValue) {
        double tmp;
        if (!m_store->readValue(&tmp)) {
            if (!setDefaultValueHelper(outValue)) onError();
            return;
        }
        outValue = static_cast<double>(tmp);
    }
    void readValue(String& outValue) {
        if (!m_store->readValue(&outValue)) {
            if (!setDefaultValueHelper(outValue)) onError();
            return;
        }
    }

    template<class TValue>
    void readValue(Ref<TValue>& outValue) {
        if (m_store->getReadingValueType() == ArchiveNodeType::Null) {
            outValue = nullptr;
        }
        else {
            //LN_DCHECK(!m_current.containerOpend);
            //m_store->openContainer();
            //m_current.containerOpend = true;

            if (!outValue) {
                outValue = detail::makeObjectHelper<TValue>();
            }

            if (outValue) {
                preLoadSerialize();

                m_nodeInfoStack.push_back(m_current);
                m_current = NodeInfo{};

                outValue->serialize(*this);

                postLoadSerialize();
            }
        }
    }

    template<>
    void readValue(Ref<Variant>& outValue) {
        ArchiveNodeType type;
        makeVariantTag(&type);
        
        //if (isSaving()) {
        //    if (!outValue) {
        //        processNull();
        //    }
        //    else {
        //        outValue->serializeInternal3(*this, type);
        //    }
        //}
        //else {
            if (type != ArchiveNodeType::Null) {
        
                // TODO: いまのところ Variant 用
                if (!outValue) {
                    outValue = makeObject<Variant>();
                }
                outValue->serializeInternal3(*this, type);
            }
            else {
                outValue = nullptr;
            }
        //}
    }

    // メンバ関数として serialize を持つ型の readValue()
    template<
        typename TValue,
        typename std::enable_if<detail::has_member_serialize_function<TValue>::value, std::nullptr_t>::type = nullptr>
    void readValue(TValue& outValue) {
        preLoadSerialize();

        // TValue が ValueType であっても、serialize の階層を測りたいので、serialize を呼ぶ前に必ず push
        if (!m_nodeInfoStack.empty() && m_nodeInfoStack.back().headState == NodeType::WrapperObject && m_nodeInfoStack.back().containerOpend) {
            m_current.parentIsOpendWrapper = true;
        }
        m_nodeInfoStack.push_back(m_current);
        m_current = NodeInfo{}; // Ready で始める

        outValue.serialize(*this);

        postLoadSerialize();
    }

    // メンバ関数として serialize を持たない型の readValue()
    template<
        typename TValue,
        typename std::enable_if<detail::non_member_serialize_function<TValue>::value, std::nullptr_t>::type = nullptr>
    void readValue(TValue& outValue) {
        preLoadSerialize();

        // TValue が ValueType であっても、serialize の階層を測りたいので、serialize を呼ぶ前に必ず push
        if (!m_nodeInfoStack.empty() && m_nodeInfoStack.back().headState == NodeType::WrapperObject && m_nodeInfoStack.back().containerOpend) {
            m_current.parentIsOpendWrapper = true;
        }
        m_nodeInfoStack.push_back(m_current);
        m_current = NodeInfo{};

        ::ln::serialize(*this, outValue);

        postLoadSerialize();
    }

    bool preLoadSerialize() {
        tryOpenContainer();
        //NodeType parentState = NodeType::Ready;
        //if (!m_nodeInfoStack.empty()) {
        //    parentState = m_nodeInfoStack.back().headState;
        //}
        /*
        ArchiveContainerType type = m_store->getOpendContainerType();

        NodeInfo node;
        node.readingContainerType = type;
        m_nodeInfoStack.push_back(node);*/

        ////if (type == ArchiveNodeType::Array || type == ArchiveNodeType::Object) {
        //if (parentState == NodeType::WrapperObject) {
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

    void postLoadSerialize() {
        // 空の serialize を呼び出した場合、state は変わっていない。
        // 空の Object として扱いたいので、ここで Object 状態にしておく。
        NodeInfo* node = currentNode();
        if (node->headState == NodeType::Ready) {
            setCurrentNodeType(NodeType::Object);
        }

        if (node->containerOpend) {
            m_store->closeContainer();
            node->containerOpend = false;
        }

        m_current = m_nodeInfoStack.back();
        m_nodeInfoStack.pop_back();
    }

    //bool confirmObjectContainerAndOpen()
    //{
    //    setParentNodeType(NodeType::Object);

    //    NodeType parentState = NodeType::Ready;
    //    if (m_nodeInfoStack.size() >= 2) {
    //        parentState = m_nodeInfoStack.back().headState;
    //    }

    //    //if (type == ArchiveNodeType::Array || type == ArchiveNodeType::Object) {
    //    if (parentState == NodeType::WrapperObject) {
    //        // ひとつ前の process 直前で open された状態を維持する。
    //    }
    //    else if (m_nodeInfoStack.back().headState == NodeType::PrimitiveValue) {
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

    bool tryOpenContainer() {
        LN_DCHECK(m_mode == ArchiveMode::Load);
        
        NodeInfo* node = currentNode();

        if (node->headState == NodeType::PrimitiveValue ||
            node->headState == NodeType::WrapperObject ||
            node->parentIsOpendWrapper) {
        }
        else if (node->headState == NodeType::Array || node->headState == NodeType::Object) {
            if (!node->containerOpend) {
                if (!m_store->openContainer()) {
                    return false;
                }
                readClassVersion(node);
                node->containerOpend = true;
            }
        }

        return true;
    }

    void readClassVersion(NodeInfo* containerNode) {
        if (currentNode()->headState == NodeType::Object &&
            m_store->moveToNamedMember(s_defaultVersionKey)) {
            int64_t version;
            m_store->readValue(&version);
            containerNode->classVersion = static_cast<int>(version);
        }
    }

    const String& readTypeInfo();


    NodeInfo* currentNode() { return &m_current; }
    const NodeInfo* currentNode() const { return &m_current; }
    NodeInfo* parentNode() { return existsParentNode() ? &m_nodeInfoStack.back() : nullptr; }
    bool existsParentNode() const { return !m_nodeInfoStack.empty(); }

    ArchiveStore* m_store;
    ArchiveMode m_mode;

    // State stack.
    // current は含まない。初期状態では empty.
    // - [Load] m_store ではなく serialize の呼び出し階層に対応する。
    // - ルートにはダミーの Node をひとつ作る。JSON では不要だが、YAML や TOML の Store を作るときに使うかも。
    std::vector<NodeInfo> m_nodeInfoStack; // 
    
    NodeInfo m_current;                    // (Load) Value も含む。serialize の直前、current は m_nodeInfoStack に積まれ、新しい Node を current とする。
    int64_t m_archiveVersion;
    detail::NameValuePairBase* m_nextReadValueDefault = nullptr;
    String m_basePath;

	bool m_hasError;
    String m_errorMessage;
	
    static String s_defaultVersionKey;
};


class JsonTextOutputSerializer
    : public Archive {
public:
    JsonTextOutputSerializer();
    virtual ~JsonTextOutputSerializer();

    template<typename TValue>
    void save(TValue&& value) {
        if (LN_REQUIRE(!m_processing)) return;
        m_processing = true;
        try {
            Archive::process(std::forward<TValue>(value));
        }
        catch (SerializeException& e) {
            onError(e.message());
        }
        m_processing = false;
    }

    String toString(JsonFormatting formatting = JsonFormatting::Indented);

private:
    std::unique_ptr<detail::JsonArchiveStore3> m_store;
    bool m_processing;
};

class JsonTextInputSerializer
    : public Archive {
public:
    JsonTextInputSerializer();
    virtual ~JsonTextInputSerializer();

    template<typename TValue>
    Result load(const String& jsonText, TValue&& value) {
        if (LN_REQUIRE(!m_processing)) return err();
        auto r = setup(jsonText);
        if (!r) return r;
		
        m_processing = true;
        try {
            Archive::process(std::forward<TValue>(value));
        }
        catch (SerializeException& e) {
            onError(e.message());
        }
        m_processing = false;

        return ok();
    }

private:
    Result setup(const String& jsonText);

    std::unique_ptr<detail::JsonArchiveStore3> m_store;
    bool m_processing;
};

template<class T = void>
using ArchiveResult = BasicResult<T, String>;

/**
 * オブジェクトと JSON 文字列間のシリアライズ/デシリアライズ行うユーティリティです。
 */
class JsonSerializer {
public:
    /**
     * オブジェクトを JSON 文字列へシリアライズします。
     * @param[in]     value         : データが格納されたオブジェクトへの参照
     * @param[in]     formatting    : JSON 文字列の整形方法
     * @return        JSON 文字列
     */
    template<class TObject>
    static ArchiveResult<String> serialize(Ref<TObject>& value, JsonFormatting formatting = JsonFormatting::Indented) {
        if (LN_ASSERT(value)) return err();
        JsonTextOutputSerializer ar;
        ar.save(value);
        String str = ar.toString(formatting);
        if (ar.hasError()) return err(ar.errorMessage());
        return ok(str);
    }

    template<class TValue>
    static ArchiveResult<String> serialize(TValue& value, JsonFormatting formatting = JsonFormatting::Indented) {
        JsonTextOutputSerializer ar;
        ar.save(value);
        String str = ar.toString(formatting);
        if (ar.hasError()) return err(ar.errorMessage());
        return ok(str);
    }

    /**
     * JSON 文字列をオブジェクトへデシリアライズします。
     * @param[in]     jsonText    : JSON 文字列
     * @param[in]     value         : データを格納するオブジェクトへの参照
     */
    template<typename TObject>
    static ArchiveResult<Ref<TObject>> deserialize(const StringView& jsonText) {
        JsonTextInputSerializer ar;
        Ref<TObject> value;
        auto r = ar.load(jsonText, value);
		if (!r) return r;
        if (ar.hasError()) return err(ar.errorMessage());
        return ok(value);
    }

    template<typename TObject>
    static ArchiveResult<> deserialize(const StringView& jsonText, TObject* value) {
        JsonTextInputSerializer ar;
        auto r = ar.load(jsonText, *value);
        if (!r) return r;
        if (ar.hasError()) return err(ar.errorMessage());
        return ok();
    }
};

template<class TItem>
inline void serialize(Archive& ar, Array<TItem>& value) {
    int size = 0;
    ar.makeArrayTag(&size);

    if (ar.isLoading()) {
        value.resize(size);
    }

    for (TItem& v : value) {
        ar.process(v);
    }
}

template<typename TValue>
inline void serialize(Archive& ar, Optional<TValue>& value) {
    bool hasValue = value.hasValue();
    ar.makeOptionalTag(&hasValue);

    if (ar.isSaving()) {
        if (hasValue) {
            ar.process(value.value());
        }
    }
    else {
        if (hasValue) {
            TValue v;
            ar.process(v);
            value = v;
        }
        else {
            value.reset();
        }
    }
}

template<typename TKey, typename TValue>
void serialize(Archive& ar, std::unordered_map<TKey, TValue>& value) {
    int size = static_cast<int>(value.size());
    ar.makeMapTag(&size);
    if (ar.isSaving()) {
        for (auto& p : value) {
            ar.makeMapItem(p.first, p.second);
        }
    }
    else {
        value.clear();
        auto hint = value.begin();
        for (int i = 0; i < size; i++) {
            TKey k;
            TValue v;
            ar.makeMapItem(k, v);
            hint = value.emplace_hint(hint, std::move(k), std::move(v));
        }
    }
}

inline void serialize(Archive& ar, Path& value) {
    if (ar.isSaving()) {
        String str = value.str();
        ar.makeStringTag(&str);
    }
    else {
        String str;
        ar.makeStringTag(&str);
        value = Path(str);
    }
}

inline void serialize(Archive& ar, Uuid& value) {
    if (ar.isSaving()) {
        String str = value.toString();
        ar.makeStringTag(&str);
    }
    else {
        String str;
        ar.makeStringTag(&str);
        value = Uuid(str);
    }
}

} // namespace ln

#define LN_SERIALIZE_VERSION(version)                         \
    template<typename T>                                      \
    friend class ::ln::detail::has_member_serialize_function; \
    template<typename T>                                      \
    friend class ::ln::detail::non_member_serialize_function; \
    friend class ::ln::Archive;                               \
    static const int lumino_class_version = version;

// non‐intrusive
#define LN_SERIALIZE_VERSION_NI(type, version)       \
    namespace ln {                                   \
    namespace detail {                               \
    template<>                                       \
    struct SerializeClassVersionInfo<type> {         \
        static const int value = version;            \
    };                                               \
    }                                                \
    }