
#include <rapidjson/encodings.h>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <rapidjson/error/en.h>
#include "Internal.hpp"
#include <LuminoEngine/Reflection/Object.hpp>
#include <LuminoEngine/Reflection/Serializer3.hpp>

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
    Result setupLoad(const String& text);
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
    const String& onGetMemberKey(int index) const override;
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
// Serializer3

const String Serializer3::ClassNameKey = _TT("lumino_class_name");
const String Serializer3::ClassVersionKey = _TT("lumino_class_version");
const String Serializer3::ClassBaseKey = _TT("lumino_base_class");

void Serializer3::makeArrayTag(int* outSize) {
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

void Serializer3::makeMapTag(int* outSize) {
    if (isSaving()) {
        setParentNodeType(NodeType::Object);
    }
    else if (isLoading()) {
        setParentNodeType(NodeType::Object);
        tryOpenContainer();
        if (outSize) *outSize = m_store->getContainerElementCount();
    }
}

void Serializer3::makeVariantTag(ArchiveNodeType* type) {

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
}

void Serializer3::makeTypeInfo(String* value) {
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

const String& Serializer3::readTypeInfo() {
    m_store->moveToNamedMember(_TT("_type"));
    ln::String type;
    if (m_store->readValue(&type)) {
        m_nodeInfoStack.back().typeInfo = type;
    }
    return m_nodeInfoStack.back().typeInfo;
}

void Serializer3::popNodeWrite() {
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

Result JsonArchiveStore3::setupLoad(const String& text) {
    rapidjson::ParseResult result = m_document.Parse(text.c_str(), text.size());
    if (!result) {
        LN_ERROR("JSON parse error: {} ({})", GetParseError_En(result.Code()), result.Offset());
        return err();
    }
    m_current = &m_document;
    return ok();
}

String JsonArchiveStore3::toString(JsonFormatting formatting) const {
    RapidJsonStringBuffer buffer;
    rapidjson::Writer<RapidJsonStringBuffer, rapidjson::UTF32LE<char32_t>, rapidjson::UTF32LE<char32_t>> writer(buffer);
    if (!m_document.Accept(writer)) {
        LN_ERROR();
        return String::Empty;
    }
    return String(buffer.GetString(), buffer.GetLength());
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
    const String& key = getNextName();
    RapidJsonStringRef k(key.c_str(), key.length());
    m_stack.push({ RapidJsonValue(k, m_document.GetAllocator()), RapidJsonValue(rapidjson::kArrayType) });
}

void JsonArchiveStore3::onWriteArrayEnd() {
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
    const auto itr = value->FindMember(k);
    return itr != value->MemberEnd();
}

const String& JsonArchiveStore3::onGetMemberKey(int index) const {
    LN_NOTIMPLEMENTED();
    return String::Empty;
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
// JsonTextOutputSerializer3

JsonTextOutputSerializer3::JsonTextOutputSerializer3()
    : m_store(std::make_unique<detail::JsonArchiveStore3>())
    , m_processing(false) {
    m_store->setupSave();
    setup(m_store.get(), ArchiveMode::Save);
}

JsonTextOutputSerializer3::~JsonTextOutputSerializer3() {
}

String JsonTextOutputSerializer3::toString(JsonFormatting formatting) {
    return m_store->toString(formatting);
}

//==============================================================================
// JsonTextInputSerializer3

JsonTextInputSerializer3::JsonTextInputSerializer3()
    : m_store(std::make_unique<detail::JsonArchiveStore3>())
    , m_processing(false) {
}

JsonTextInputSerializer3::~JsonTextInputSerializer3() {
}

Result JsonTextInputSerializer3::setup(const String& jsonText) {
    LN_TRY(m_store->setupLoad(jsonText));
    Serializer3::setup(m_store.get(), ArchiveMode::Load);
    return ok();
}

} // namespace ln

