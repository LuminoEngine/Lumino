#define RAPIDJSON_NAMESPACE ln_rapidjson
#include "rapidjson/rapidjson.h"
#include "rapidjson/encodings.h"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include "rapidjson/error/en.h"
#include <LuminoCore/Serialization/JsonArchiveStore.hpp>

namespace ln {
namespace detail {
using RapidJsonStringRef = RAPIDJSON_NAMESPACE::GenericStringRef<char32_t>;
using RapidJsonValue = RAPIDJSON_NAMESPACE::GenericValue<RAPIDJSON_NAMESPACE::UTF32LE<char32_t>>;
using RapidJsonDocument = RAPIDJSON_NAMESPACE::GenericDocument<RAPIDJSON_NAMESPACE::UTF32LE<char32_t>>;
using RapidJsonStringBuffer = RAPIDJSON_NAMESPACE::GenericStringBuffer<RAPIDJSON_NAMESPACE::UTF32LE<char32_t>>;

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
    RAPIDJSON_NAMESPACE::ParseResult result =
        m_document.Parse<RAPIDJSON_NAMESPACE::kParseDefaultFlags | RAPIDJSON_NAMESPACE::kParseNanAndInfFlag>(
            text.c_str(), text.size());
    if (!result) {
        const auto message = ::LN_FMT_NAMESPACE::format(
            "JSON parse error: {} (offset:{})", GetParseError_En(result.Code()), result.Offset());
        return err(ln::String::fromUtf8(message));
    }
    m_current = &m_document;
    return ok();
}

String JsonArchiveStore3::toString(JsonFormatting formatting) const {
    RapidJsonStringBuffer buffer;
    RAPIDJSON_NAMESPACE::Writer<
        RapidJsonStringBuffer,
        RAPIDJSON_NAMESPACE::UTF32LE<char32_t>,
        RAPIDJSON_NAMESPACE::UTF32LE<char32_t>,
        RAPIDJSON_NAMESPACE::CrtAllocator,
        RAPIDJSON_NAMESPACE::kWriteDefaultFlags | RAPIDJSON_NAMESPACE::kWriteNanAndInfFlag>
        writer(buffer);
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
        m_stack.push(
            { RapidJsonValue(k, m_document.GetAllocator()), RapidJsonValue(RAPIDJSON_NAMESPACE::kObjectType) });
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
        m_stack.push({ RapidJsonValue(k, m_document.GetAllocator()), RapidJsonValue(RAPIDJSON_NAMESPACE::kArrayType) });
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
        container->AddMember(
            RapidJsonValue(k, m_document.GetAllocator()),
            RapidJsonValue(RAPIDJSON_NAMESPACE::kNullType),
            m_document.GetAllocator());
    }
    else if (container->IsArray()) {
        container->PushBack(RapidJsonValue(RAPIDJSON_NAMESPACE::kNullType), m_document.GetAllocator());
    }
    else {
        LN_UNREACHABLE();
    }
}

void JsonArchiveStore3::onWriteValueBool(bool value) {
    RapidJsonValue* container = savingContainer();
    if (container->IsObject()) {
        RapidJsonValue k(toStringRef(getNextName()), m_document.GetAllocator());
        container->AddMember(k, value, m_document.GetAllocator());
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
        RapidJsonValue k(toStringRef(getNextName()), m_document.GetAllocator());
        container->AddMember(k, value, m_document.GetAllocator());
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
        RapidJsonValue k(toStringRef(getNextName()), m_document.GetAllocator());
        container->AddMember(k, value, m_document.GetAllocator());
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
        RapidJsonValue k(toStringRef(getNextName()), m_document.GetAllocator());
        container->AddMember(k, v, m_document.GetAllocator());
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
    if (LN_ASSERT(
            m_current->GetType() == RAPIDJSON_NAMESPACE::kObjectType ||
            m_current->GetType() == RAPIDJSON_NAMESPACE::kArrayType))
        return false;
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
    else if (container->IsObject()) {
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
        case RAPIDJSON_NAMESPACE::kNullType:
            return ArchiveNodeType::Null;
        case RAPIDJSON_NAMESPACE::kFalseType:
            return ArchiveNodeType::Bool;
        case RAPIDJSON_NAMESPACE::kTrueType:
            return ArchiveNodeType::Bool;
        case RAPIDJSON_NAMESPACE::kObjectType:
            return ArchiveNodeType::Object;
        case RAPIDJSON_NAMESPACE::kArrayType:
            return ArchiveNodeType::Array;
        case RAPIDJSON_NAMESPACE::kStringType:
            return ArchiveNodeType::String;
        case RAPIDJSON_NAMESPACE::kNumberType:
            if (m_current->IsFloat() || m_current->IsDouble() || m_current->IsLosslessFloat() ||
                m_current->IsLosslessDouble()) {
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

Result_deprecated<> JsonTextInputSerializer::setup(const String& jsonText) {
    auto result = m_store->setupLoad(jsonText);
    if (!result) return result;
    Archive::setup(m_store.get(), ArchiveMode::Load);
    return ok();
}

} // namespace ln

