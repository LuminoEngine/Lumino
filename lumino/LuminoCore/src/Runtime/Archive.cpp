#include <rapidjson/encodings.h>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <rapidjson/error/en.h>
#include <LuminoCore/Serialization/ArchiveStore.hpp>
#include <LuminoCore/Runtime/Object.hpp>
#include <LuminoCore/Runtime/Archive.hpp>

namespace ln {

namespace detail {
using RapidJsonStringRef = rapidjson::GenericStringRef<char32_t>;
using RapidJsonValue = rapidjson::GenericValue<rapidjson::UTF32LE<char32_t>>;
using RapidJsonDocument = rapidjson::GenericDocument<rapidjson::UTF32LE<char32_t>>;
using RapidJsonStringBuffer = rapidjson::GenericStringBuffer<rapidjson::UTF32LE<char32_t>>;

class JsonArchiveStore3 : public ArchiveStore {
public:
    JsonArchiveStore3();
    ~JsonArchiveStore3();
    void setupSave();
    ArchiveResult<> setupLoad(const String& text);
    String toString(JsonFormatting formatting) const;

protected:
    ArchiveContainerType onGetContainerType() const override;

    void onWriteObject() override;
    void onWriteArray() override;
    void onWriteObjectEnd() override;
    void onWriteArrayEnd() override;
    void onWriteValueNull() override;
    void onWriteValueBool(bool value) override;
    void onWriteValueInt64(int64_t value) override;
    void onWriteValueDouble(double value) override;
    void onWriteValueString(const String& value) override;

    bool onOpenContainer() override;
    bool onCloseContainer() override;
    int onReadContainerElementCount() const override;
    bool onMoveToNamedMember(const StringView& name) override;
    bool onMoveToIndexedMember(int index) override;
    bool onHasKey(const StringView& name) const override;
    StringView onGetMemberKey(int index) const override;
    ArchiveNodeType onGetReadingValueType() override;
    bool onReadValueBool(bool* outValue) override;
    bool onReadValueInt64(int64_t* outValue) override;
    bool onReadValueDouble(double* outValue) override;
    bool onReadValueString(String* outValue) override;

private:
    RapidJsonStringRef toStringRef(const String& s) const { return RapidJsonStringRef(s.c_str(), s.length()); }
    RapidJsonValue* savingContainer();

    struct StackItem {
        RapidJsonValue key;
        RapidJsonValue container;
    };

    RapidJsonDocument m_document;
    std::stack<StackItem> m_stack;
    RapidJsonValue* m_current; // Load の時だけ使う
    std::stack<RapidJsonValue*> m_loadingStack;

};
} // namespace detail

//==============================================================================
// Archive

String Archive::s_defaultVersionKey = U"_ln_version_";

const String Archive::ClassNameKey = _TT("lumino_class_name");
const String Archive::ClassBaseKey = _TT("lumino_base_class");

Archive::Archive(ArchiveStore* store, ArchiveMode mode)
    : Archive() {
    setup(store, mode);
}

Archive::~Archive() {
}

void Archive::processNull() {
    setCurrentNodeType(NodeType::PrimitiveValue);
    preWriteValue();
    writeValueNull();
}

void Archive::makePrimitiveValue() {
    setCurrentNodeType(NodeType::PrimitiveValue);
}

void Archive::makeStringTag(String* str) {
    if (isSaving()) {
        setCurrentNodeType(NodeType::PrimitiveValue);
        process(*str);
    }
    else {

        //  process は、いま open しているコンテナに対して行いたい。
        // ここで閉じて、次に使えるようにする。current は閉じたコンテナになる。
        // m_store->closeContainer();
        // m_nodeInfoStack.back().containerOpend = false;

        // Value 確定。次の process で open container しない。
        setCurrentNodeType(NodeType::PrimitiveValue);
        process(*str);
    }
}

void Archive::makeArrayTag(int* outSize) {
    //
    if (isSaving()) {
        setCurrentNodeType(NodeType::Array);
    }
    else if (isLoading()) {
        setCurrentNodeType(NodeType::Array);
        // ArrayContainer としてデシリアライズしている場合、この時点で size を返したいので、store を ArrayContainer まで移動して size を得る必要がある
        //preReadValue();
        tryOpenContainer();
        if (outSize) *outSize = m_store->getContainerElementCount();

        // makeArrayTag() を抜けた次の process は 0 インデックスを使う
        //m_store->moveToIndexedMember(0);
    }
}

void Archive::makeMapTag(int* outSize) {
    if (isSaving()) {
        setCurrentNodeType(NodeType::Object);
    }
    else if (isLoading()) {
        setCurrentNodeType(NodeType::Object);
        tryOpenContainer();
        if (outSize) *outSize = m_store->getContainerElementCount();
    }
}

void Archive::makeObjectTag(int* outSize) {
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

void Archive::makeSmartPtrTag(bool* outIsNull) {
    if (isSaving()) {
        if ((*outIsNull)) {
            processNull();
        }
        else {
            setParentNodeType(NodeType::WrapperObject);
        }
    }
    else if (isLoading()) {

        // setParentNodeType(NodeType::Object);
        tryOpenContainer();
        *outIsNull = m_store->getOpendContainerType() == ArchiveContainerType::Null;
        setParentNodeType(NodeType::WrapperObject);
        // if ((*outIsNull)) {
        //     setParentNodeType(NodeType::PrimitiveValue);
        // }
        // else {
        //     setParentNodeType(NodeType::WrapperObject);
        // }
    }
}

void Archive::makeOptionalTag(bool* outHasValue) {
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

        // tryOpenContainer();
        //*outHasValue = m_store->getOpendContainerType() != ArchiveContainerType::Null;

        // makeOptionalTag を抜けた後の process は、いま open しているコンテナに対して行いたい。
        // ここで閉じて、次に使えるようにする。current は閉じたコンテナになる。
        // m_store->closeContainer();
        // m_nodeInfoStack.back().containerOpend = false;

        setCurrentNodeType(NodeType::WrapperObject);

        //*outHasValue = m_store->getReadingValueType() != ArchiveNodeType::Null;
        // if (!(*outHasValue)) {
        //    setParentNodeType(NodeType::PrimitiveValue);
        //}
        // else {
        //    setParentNodeType(NodeType::WrapperObject);
        //}
    }
}

void Archive::makeVariantTag(ArchiveNodeType* type) {
#if LN_EXPERIMENTAL_VARIANT_SERIALIZATION
    if (isSaving()) {
        //if (*type == ArchiveNodeType::Object || *type == ArchiveNodeType::Array) {
        //    setParentNodeType(NodeType::WrapperObject);
        //}
        //else {
            setParentNodeType(NodeType::PrimitiveValue);
        //}
    }
    else if (isLoading()) {

        //m_store->closeContainer();
        //m_nodeInfoStack.back().containerOpend = false;

        *type = m_store->getReadingValueType();

        //*outIsNull = m_store->getOpendContainerType() == ArchiveContainerType::Null;
        //setParentNodeType(NodeType::WrapperObject);
        setParentNodeType(NodeType::WrapperObject);

        // makeVariantTag の次は何らかの値の process をする。
        // いまのところその process
        //setParentNodeType(NodeType::Ready);

        //*type = m_store->getReadingValueType();
    }

    //m_store->getContainerType()
#else
    // TODO: Type に応じて PrimitiveValue にしたり Object にしたり、細かにコントロールする必要がある。
    LN_NOTIMPLEMENTED();
#endif
}

void Archive::makeTypeInfo(String* value) {
    if (isSaving()) {
        // この時点では Ref<> の serialize、つまり WrapperObject の serialize 中。
        // ここではまだ m_store に書き出すことはできない (コンテナの書き出しがまだ) ので、メタデータを Node に覚えておく。
        m_nodeInfoStack.back().typeInfo = *value;
    }
    else {
        //setParentNodeType(NodeType::Object);
        if (LN_REQUIRE(m_nodeInfoStack.back().headState == NodeType::WrapperObject)) return; // 事前に makeSmartPtrTag 必須
        preReadValue();
        *value = readTypeInfo();
        //process(NameValuePair<String>(u"_type", value));
    }

    //setParentNodeType(NodeType::Object);	// TypeInfo を読みたいなら Object confirmed.

    //if (isSaving()) {
    //}
    //else {
    //	preReadValue();
    //	*value = m_nodeInfoStack.top().typeInfo;
    //}
}

bool Archive::preWriteValue() {
    if (m_nodeInfoStack.empty()) {
        // ルートノードの場合は NVP や Tag が何もセットされていない。
        // (これ用のダミーノードを作っても Ready にしか遷移しないので無駄)
        return true;
    }

    NodeInfo* node = currentNode();

    switch (node->headState) {
        case NodeType::Ready:
            onError(); // NVP も Tag も事前にセットされていない。
            return false;

        case NodeType::Object:
            if (!node->containerOpend) {
                m_store->writeObject();
                node->containerOpend = true;
                writeClassVersion(node);
                writeTypeInfo();
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
            return true;

        default:
            LN_UNREACHABLE();
            return false;
    }
}

// 検証しつつ、current の NodeType を変更する
void Archive::setParentNodeType(NodeType type) {
    NodeInfo* parent = parentNode();
    if (parent->headState == NodeType::Ready) {
        LN_DCHECK(type != NodeType::Ready);
        parent->headState = type;
    }
    else if (parent->headState != type) {
        LN_UNREACHABLE();
    }
}

void Archive::setCurrentNodeType(NodeType type) {
    NodeInfo* current = currentNode();
    if (current->headState == NodeType::Ready) {
        LN_DCHECK(type != NodeType::Ready);
        current->headState = type;
    }
    else if (current->headState != type) {
        LN_UNREACHABLE();
    }
}

void Archive::popNodeWrite() {
    NodeInfo* current = currentNode();
    NodeInfo* parent = parentNode();

    // 空の serialize を呼び出した場合、state は変わっていない。
    // 空の Object として扱いたいので、ここで Object 状態にしておく。
    if (current->headState == NodeType::Ready) {
        current->headState = NodeType::Object;
    }

    // serialize が空実装ではないが、makeArrayTag など Tag 設定だけして子値の process を行わなかった場合はコンテナ開始タグが書き込まれていないため、ここで書き込む。
    if (current->headState == NodeType::Object ||
        current->headState == NodeType::Array) {
        if (!current->containerOpend) {
            if (current->headState == NodeType::Object) {
                m_store->writeObject();
            }
            else {
                m_store->writeArray();
            }
            current->containerOpend = true;
        }
    }

    if (!current->typeInfo.isEmpty()) {
        m_store->setNextName(_TT("_type"));
        writeValue(current->typeInfo);
    }

    bool containerOpend = current->containerOpend;
    NodeType nodeType = current->headState;

    // Pop
    m_current = m_nodeInfoStack.back();
    m_nodeInfoStack.pop_back();

    // Close containers
    if (containerOpend) {
        if (nodeType == NodeType::Object || nodeType == NodeType::WrapperObject) {
            m_store->writeObjectEnd();
        }
        else if (nodeType == NodeType::Array) {
            m_store->writeArrayEnd();
        }
        else {
            LN_UNREACHABLE();
        }
    }
}

void Archive::writeClassVersion(NodeInfo* node) {
    if (node->classVersion > 0) {
        m_store->setNextName(s_defaultVersionKey);
        writeValue(node->classVersion);
    }
}

void Archive::writeTypeInfo() {
    NodeInfo* parentNode = (m_nodeInfoStack.size() >= 2) ? &m_nodeInfoStack[m_nodeInfoStack.size() - 2] : nullptr;
    if (parentNode && !parentNode->typeInfo.isEmpty()) {
        m_store->setNextName(_TT("_type"));
        writeValue(parentNode->typeInfo);
    }
}

bool Archive::preReadValue() {
    NodeInfo* node = currentNode();
    if (node->headState == NodeType::Array) {
        m_store->moveToIndexedMember(node->arrayIndex);
    }
    return true;
}

// after pop value node. stack top refers to parent container.
void Archive::postReadValue() {
    NodeInfo* node = currentNode();
    if (node->headState == NodeType::Array) {
        node->arrayIndex++;
    }
}

bool Archive::preLoadSerialize() {
    tryOpenContainer();
    return true;
}

void Archive::postLoadSerialize() {
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

bool Archive::tryOpenContainer() {
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

void Archive::readClassVersion(NodeInfo* containerNode) {
    if (currentNode()->headState == NodeType::Object &&
        m_store->moveToNamedMember(s_defaultVersionKey)) {
        int64_t version;
        m_store->readValue(&version);
        containerNode->classVersion = static_cast<int>(version);
    }
}

const String& Archive::readTypeInfo() {
    m_store->moveToNamedMember(_TT("_type"));
    ln::String type;
    if (m_store->readValue(&type)) {
        m_nodeInfoStack.back().typeInfo = type;
    }
    return m_nodeInfoStack.back().typeInfo;
}

//==============================================================================
// JsonArchiveStore3
namespace detail {

JsonArchiveStore3::JsonArchiveStore3() {
}

JsonArchiveStore3::~JsonArchiveStore3() {

}

void JsonArchiveStore3::setupSave() {
    m_current = nullptr;
}

ArchiveResult<> JsonArchiveStore3::setupLoad(const String& text) {
    rapidjson::ParseResult result = m_document.Parse<rapidjson::kParseDefaultFlags | rapidjson::kParseNanAndInfFlag>(text.c_str(), text.size());
    if (!result) {
        const auto message = fmt::format("JSON parse error: {} (offset:{})", GetParseError_En(result.Code()), result.Offset());
        return err(ln::String::fromUtf8(message));
    }
    m_current = &m_document;
    return ok();
}

String JsonArchiveStore3::toString(JsonFormatting formatting) const {
    RapidJsonStringBuffer buffer;
    rapidjson::Writer<RapidJsonStringBuffer, rapidjson::UTF32LE<char32_t>, rapidjson::UTF32LE<char32_t>, rapidjson::CrtAllocator, rapidjson::kWriteDefaultFlags | rapidjson::kWriteNanAndInfFlag> writer(buffer);
    if (!m_document.Accept(writer)) {
        LN_ERROR();
        return String::Empty;
    }
    return String(buffer.GetString(), static_cast<int>(buffer.GetLength()));
}

RapidJsonValue* JsonArchiveStore3::savingContainer() {
    return (m_stack.empty()) ? &m_document : &m_stack.top().container;
}

ArchiveContainerType JsonArchiveStore3::onGetContainerType() const {
    LN_NOTIMPLEMENTED();
    return ArchiveContainerType::Null;
}

void JsonArchiveStore3::onWriteObject() {
    if (m_document.IsNull()) { // Root element. (First time)
        assert(m_stack.empty());
        m_document.SetObject();
    }
    else {
        const String& key = getNextName();
        RapidJsonStringRef k(key.c_str(), key.length());
        m_stack.push({ RapidJsonValue(k, m_document.GetAllocator()), RapidJsonValue(rapidjson::kObjectType) });
    }
}

void JsonArchiveStore3::onWriteObjectEnd() {
    if (m_stack.empty()) { // Root element.
    }
    else {
        StackItem item = std::move(m_stack.top());
        m_stack.pop();

        RapidJsonValue* container = savingContainer();
        if (container->IsObject()) {
            container->AddMember(item.key, item.container, m_document.GetAllocator());
        }
        else if (container->IsArray()) {
            container->PushBack(item.container, m_document.GetAllocator());
        }
        else {
            LN_UNREACHABLE();
        }
    }
}

void JsonArchiveStore3::onWriteArray() {
    if (m_document.IsNull()) { // Root element. (First time)
        assert(m_stack.empty());
        m_document.SetArray();
    }
    else {
        const String& key = getNextName();
        RapidJsonStringRef k(key.c_str(), key.length());
        m_stack.push({ RapidJsonValue(k, m_document.GetAllocator()), RapidJsonValue(rapidjson::kArrayType) });
    }
}

void JsonArchiveStore3::onWriteArrayEnd() {
    if (m_stack.empty()) { // Root element.
    }
    else {
        StackItem item = std::move(m_stack.top());
        m_stack.pop();

        RapidJsonValue* container = savingContainer();
        if (container->IsObject()) {
            container->AddMember(item.key, item.container, m_document.GetAllocator());
        }
        else if (container->IsArray()) {
            container->PushBack(item.container, m_document.GetAllocator());
        }
        else {
            LN_UNREACHABLE();
        }
    }
}

void JsonArchiveStore3::onWriteValueNull() {
    RapidJsonValue* container = savingContainer();
    if (container->IsObject()) {
        RapidJsonStringRef k = toStringRef(getNextName());
        container->AddMember(RapidJsonValue(k, m_document.GetAllocator()), RapidJsonValue(rapidjson::kNullType), m_document.GetAllocator());
    }
    else if (container->IsArray()) {
        container->PushBack(RapidJsonValue(rapidjson::kNullType), m_document.GetAllocator());
    }
    else {
        LN_UNREACHABLE();
    }
}

void JsonArchiveStore3::onWriteValueBool(bool value) {
    RapidJsonValue* container = savingContainer();
    if (container->IsObject()) {
        RapidJsonStringRef k = toStringRef(getNextName());
        container->AddMember(RapidJsonValue(k, m_document.GetAllocator()), value, m_document.GetAllocator());
    }
    else if (container->IsArray()) {
        container->PushBack(value, m_document.GetAllocator());
    }
    else {
        LN_UNREACHABLE();
    }
}

void JsonArchiveStore3::onWriteValueInt64(int64_t value) {
    RapidJsonValue* container = savingContainer();
    if (container->IsObject()) {
        RapidJsonStringRef k = toStringRef(getNextName());
        container->AddMember(RapidJsonValue(k, m_document.GetAllocator()), value, m_document.GetAllocator());
    }
    else if (container->IsArray()) {
        container->PushBack(value, m_document.GetAllocator());
    }
    else {
        LN_UNREACHABLE();
    }
}

void JsonArchiveStore3::onWriteValueDouble(double value) {
    RapidJsonValue* container = savingContainer();
    if (container->IsObject()) {
        RapidJsonStringRef k = toStringRef(getNextName());
        container->AddMember(RapidJsonValue(k, m_document.GetAllocator()), value, m_document.GetAllocator());
    }
    else if (container->IsArray()) {
        container->PushBack(value, m_document.GetAllocator());
    }
    else {
        LN_UNREACHABLE();
    }
}

void JsonArchiveStore3::onWriteValueString(const String& value) {
    RapidJsonValue v(value.c_str(), value.length(), m_document.GetAllocator());
    RapidJsonValue* container = savingContainer();
    if (container->IsObject()) {
        RapidJsonStringRef k = toStringRef(getNextName());
        container->AddMember(RapidJsonValue(k, m_document.GetAllocator()), v, m_document.GetAllocator());
    }
    else if (container->IsArray()) {
        container->PushBack(v, m_document.GetAllocator());
    }
    else {
        LN_UNREACHABLE();
    }
}

bool JsonArchiveStore3::onOpenContainer() {
    if (LN_REQUIRE(m_current)) return false;
    if (LN_ASSERT(m_current->GetType() == rapidjson::kObjectType || m_current->GetType() == rapidjson::kArrayType)) return false;
    m_loadingStack.push(m_current);
    m_current = nullptr;
    return true;
}

bool JsonArchiveStore3::onCloseContainer() {
    if (LN_REQUIRE(!m_loadingStack.empty())) return false;
    m_current = m_loadingStack.top();
    m_loadingStack.pop();
    return true;
}

int JsonArchiveStore3::onReadContainerElementCount() const {
    RapidJsonValue* container = m_loadingStack.top();
    if (container->IsArray()) {
        return container->Size();
    }
    else if(container->IsObject()) {
        return container->MemberCount();
    }
    else {
        LN_NOTIMPLEMENTED();
        return 0;
    }
}

bool JsonArchiveStore3::onMoveToNamedMember(const StringView& name) {
    if (LN_REQUIRE(!m_loadingStack.empty())) return false;

    RapidJsonValue* container = m_loadingStack.top();
    if (LN_REQUIRE(container->IsObject())) return false;

    RapidJsonValue k(name.data(), name.length());
    const auto itr = container->FindMember(k);
    if (itr != container->MemberEnd()) {
        m_current = &(itr->value);
        return true;
    }
    else {
        m_current = nullptr;
        return false;
    }
}

bool JsonArchiveStore3::onMoveToIndexedMember(int index) {
    if (LN_REQUIRE(!m_loadingStack.empty())) return false;

    RapidJsonValue* container = m_loadingStack.top();
    if (LN_REQUIRE(container->IsArray())) return false;

    m_current = &(*container)[index];

    return false;
}

bool JsonArchiveStore3::onHasKey(const StringView& name) const {
    if (LN_REQUIRE(!m_loadingStack.empty())) return false;

    RapidJsonValue* value = m_loadingStack.top();
    if (LN_REQUIRE(value->IsObject())) return false;

    RapidJsonValue k(name.data(), name.length());
    return value->HasMember(k);
}

StringView JsonArchiveStore3::onGetMemberKey(int index) const {
    RapidJsonValue* value = m_loadingStack.top();
    if (LN_REQUIRE(value->IsObject())) return String::Empty;

    const auto itr = value->MemberBegin() + index;
    const auto& name = itr->name;
    return StringView(name.GetString(), name.GetStringLength());
}

ArchiveNodeType JsonArchiveStore3::onGetReadingValueType() {
    switch (m_current->GetType()) {
        case rapidjson::kNullType:
            return ArchiveNodeType::Null;
        case rapidjson::kFalseType:
            return ArchiveNodeType::Bool;
        case rapidjson::kTrueType:
            return ArchiveNodeType::Bool;
        case rapidjson::kObjectType:
            return ArchiveNodeType::Object;
        case rapidjson::kArrayType:
            return ArchiveNodeType::Array;
        case rapidjson::kStringType:
            return ArchiveNodeType::String;
        case rapidjson::kNumberType:
            if (m_current->IsFloat() || m_current->IsDouble() || m_current->IsLosslessFloat() || m_current->IsLosslessDouble()) {
                return ArchiveNodeType::Double;
            }
            else {
                return ArchiveNodeType::Int64;
            }
        default:
            LN_UNREACHABLE();
            return ArchiveNodeType::Null;
    }
}

bool JsonArchiveStore3::onReadValueBool(bool* outValue) {
    if (!m_current) return false;

    if (m_current->IsBool()) {
        *outValue = m_current->GetBool();
    }
    else {
        LN_UNREACHABLE();
        return false;
    }
    return true;
}

bool JsonArchiveStore3::onReadValueInt64(int64_t* outValue) {
    if (!m_current) return false;

    if (m_current->IsInt()) {
        *outValue = static_cast<int64_t>(m_current->GetInt());
    }
    else if (m_current->IsInt64()) {
        *outValue = static_cast<int64_t>(m_current->GetInt64());
    }
    else if (m_current->IsUint()) {
        *outValue = static_cast<int64_t>(m_current->GetUint());
    }
    else if (m_current->IsUint64()) {
        *outValue = static_cast<int64_t>(m_current->GetUint64());
    }
    else {
        LN_UNREACHABLE();
        return false;
    }
    return true;
}

bool JsonArchiveStore3::onReadValueDouble(double* outValue) {
    if (!m_current) return false;

    if (m_current->IsFloat()) {
        *outValue = static_cast<double>(m_current->GetFloat());
    }
    else if (m_current->IsDouble()) {
        *outValue = static_cast<double>(m_current->GetDouble());
    }
    else if (m_current->IsLosslessFloat()) {
        *outValue = static_cast<double>(m_current->GetFloat());
    }
    else if (m_current->IsLosslessDouble()) {
        *outValue = static_cast<double>(m_current->GetDouble());
    }
    else {
        LN_UNREACHABLE();
        return false;
    }
    return true;
}

bool JsonArchiveStore3::onReadValueString(String* outValue) {
    if (!m_current) return false;

    if (m_current->IsString()) {
        *outValue = m_current->GetString();
    }
    else {
        LN_UNREACHABLE();
        return false;
    }
    return true;
}

} // namespace detail

//==============================================================================
// JsonTextOutputSerializer

JsonTextOutputSerializer::JsonTextOutputSerializer()
    : m_store(std::make_unique<detail::JsonArchiveStore3>())
    , m_processing(false) {
    m_store->setupSave();
    setup(m_store.get(), ArchiveMode::Save);
}

JsonTextOutputSerializer::~JsonTextOutputSerializer() {
}

String JsonTextOutputSerializer::toString(JsonFormatting formatting) {
    return m_store->toString(formatting);
}

//==============================================================================
// JsonTextInputSerializer

JsonTextInputSerializer::JsonTextInputSerializer()
    : m_store(std::make_unique<detail::JsonArchiveStore3>())
    , m_processing(false) {
}

JsonTextInputSerializer::~JsonTextInputSerializer() {
}

Result JsonTextInputSerializer::setup(const String& jsonText) {
    auto result = m_store->setupLoad(jsonText);
    if (!result) return result;
    Archive::setup(m_store.get(), ArchiveMode::Load);
    return ok();
}

} // namespace ln

