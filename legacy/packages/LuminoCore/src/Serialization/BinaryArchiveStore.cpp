#include <LuminoCore/IO/Stream.hpp>
#include <LuminoCore/IO/BinaryWriter.hpp>
#include <LuminoCore/IO/BinaryReader.hpp>
#include <LuminoCore/Serialization/BinaryArchiveStore.hpp>

#if 0 // prototype

namespace ln {
namespace detail {

class BinaryArchiveStore3 : public ArchiveStore {
public:
    BinaryArchiveStore3();
    ~BinaryArchiveStore3();
    void setupSave(Stream* stream);
    void setupLoad(Stream* stream);

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
    void writeCString(const char* value0, int length);
    std::string readCString();

    struct WriteStack {
        int64_t pos;
    };
    struct ReadStack {
        int64_t length; // current document length.
    };

    Stream* m_stream;
    Ref<BinaryWriter> m_writer;
    Ref<BinaryReader> m_reader;
    std::stack<WriteStack> m_writeStack;
    std::stack<ReadStack> m_readStack;

};
} // namespace detail


//==============================================================================
// BinaryArchiveStore3
namespace detail {

// BSON Specification Version 1.1
// https://bsonspec.org/spec.html
enum BSONType {
    BSONType_EOD = 0x00,
	BSONType_Double = 0x01,
	BSONType_String = 0x02,
	BSONType_Object = 0x03,
	BSONType_Array = 0x04,
	BSONType_Binary = 0x05,
	BSONType_Undefined = 0x06,
	BSONType_ObjectId = 0x07,
	BSONType_Boolean = 0x08,
	BSONType_UTCDateTime = 0x09,
	BSONType_Null = 0x0A,
	BSONType_Regex = 0x0B,
	BSONType_DBPointer = 0x0C,
	BSONType_JavaScript = 0x0D,
	BSONType_Symbol = 0x0E,
	BSONType_JavaScriptWithScope = 0x0F,
	BSONType_Int32 = 0x10,
	BSONType_Timestamp = 0x11,
	BSONType_Int64 = 0x12,
	BSONType_MinKey = 0xFF,
	BSONType_MaxKey = 0x7F,
};

BinaryArchiveStore3::BinaryArchiveStore3()
    : m_stream(nullptr)
    , m_writer(nullptr) {
}

BinaryArchiveStore3::~BinaryArchiveStore3() {
}

void BinaryArchiveStore3::setupSave(Stream* stream) {
    m_stream = stream;
    m_writer = makeRef<BinaryWriter>(stream);
}

void BinaryArchiveStore3::setupLoad(Stream* stream) {
    m_stream = stream;
	m_reader = makeRef<BinaryReader>(stream);
}

//ArchiveResult<> BinaryArchiveStore3::setupLoad(const String& text) {
//    RAPIDJSON_NAMESPACE::ParseResult result =
//        m_document.Parse<RAPIDJSON_NAMESPACE::kParseDefaultFlags | RAPIDJSON_NAMESPACE::kParseNanAndInfFlag>(
//            text.c_str(), text.size());
//    if (!result) {
//        const auto message = ::LN_FMT_NAMESPACE::format(
//            "JSON parse error: {} (offset:{})", GetParseError_En(result.Code()), result.Offset());
//        return err(ln::String::fromUtf8(message));
//    }
//    m_current = &m_document;
//    return ok();
//}

void BinaryArchiveStore3::writeCString(const char* value0, int length) {
    m_writer->write(value0, length + 1);
}

std::string BinaryArchiveStore3::readCString() {
    std::string s;
	while (true) {
        char c = m_reader->readInt8();
		if (c == '\0') {
			break;
		}
		s.push_back(c);
	}
	return s;
}

ArchiveContainerType BinaryArchiveStore3::onGetContainerType() const {
    LN_NOTIMPLEMENTED();
    return ArchiveContainerType::Null;
}

void BinaryArchiveStore3::onWriteObject() {
    if (!hasNextName()) {
        // Root document.
        m_writeStack.push(WriteStack{ m_stream->position() });
        m_writer->writeInt32(0); // dummy length
        m_writer->writeUInt8(BSONType_Object);
        writeCString("", 0);
    }
    else {
        LN_NOTIMPLEMENTED();
    }
}

void BinaryArchiveStore3::onWriteObjectEnd() {
	if (m_writeStack.size() >= 1) {
		WriteStack stack = m_writeStack.top();
		m_writeStack.pop();
		int64_t endPos = m_stream->position();
		m_stream->seek(stack.pos, SeekOrigin::Begin);
		m_writer->writeInt32(endPos - stack.pos);
		m_stream->seek(endPos, SeekOrigin::Begin);
	}
	else {
		LN_NOTIMPLEMENTED();
	}
}

void BinaryArchiveStore3::onWriteArray() {
    LN_NOTIMPLEMENTED();
}

void BinaryArchiveStore3::onWriteArrayEnd() {
    LN_NOTIMPLEMENTED();
}

void BinaryArchiveStore3::onWriteValueNull() {
    LN_NOTIMPLEMENTED();
}

void BinaryArchiveStore3::onWriteValueBool(bool value) {
    LN_NOTIMPLEMENTED();
}

void BinaryArchiveStore3::onWriteValueInt64(int64_t value) {
    m_writer->writeUInt8(BSONType_Int64);
    m_writer->writeInt64(value);
}

void BinaryArchiveStore3::onWriteValueDouble(double value) {
    LN_NOTIMPLEMENTED();
}

void BinaryArchiveStore3::onWriteValueString(const String& value) {
    LN_NOTIMPLEMENTED();
}

bool BinaryArchiveStore3::onOpenContainer() {

    if (m_readStack.size() == 0) {
        // Root document.
        ReadStack item;
        item.length = m_reader->readInt32();
        m_readStack.push(item);
    }

    uint8_t type = m_reader->readUInt8();
    if (type != BSONType_Object) {
        return false;
    }
    return true;
}

bool BinaryArchiveStore3::onCloseContainer() {
    return true;
}

int BinaryArchiveStore3::onReadContainerElementCount() const {
    LN_NOTIMPLEMENTED();
    return 0;
}

bool BinaryArchiveStore3::onMoveToNamedMember(const StringView& name) {
    LN_NOTIMPLEMENTED();
    return false;
}

bool BinaryArchiveStore3::onMoveToIndexedMember(int index) {
    LN_NOTIMPLEMENTED();
    return false;
}

bool BinaryArchiveStore3::onHasKey(const StringView& name) const {
    LN_NOTIMPLEMENTED();
    return false;
}

StringView BinaryArchiveStore3::onGetMemberKey(int index) const {
    LN_NOTIMPLEMENTED();
    return false;
}

ArchiveNodeType BinaryArchiveStore3::onGetReadingValueType() {
    LN_NOTIMPLEMENTED();
    return ArchiveNodeType::Null;
}

bool BinaryArchiveStore3::onReadValueBool(bool* outValue) {
    LN_NOTIMPLEMENTED();
    return false;
}

bool BinaryArchiveStore3::onReadValueInt64(int64_t* outValue) {
    LN_NOTIMPLEMENTED();
    return false;
}

bool BinaryArchiveStore3::onReadValueDouble(double* outValue) {
    LN_NOTIMPLEMENTED();
    return false;
}

bool BinaryArchiveStore3::onReadValueString(String* outValue) {
    LN_NOTIMPLEMENTED();
    return false;
}

} // namespace detail

//==============================================================================
// BinaryTextOutputSerializer

BinaryTextOutputSerializer::BinaryTextOutputSerializer(Stream* stream)
    : m_store(std::make_unique<detail::BinaryArchiveStore3>())
    , m_processing(false) {
    m_store->setupSave(stream);
    setup(m_store.get(), ArchiveMode::Save);
}

BinaryTextOutputSerializer::~BinaryTextOutputSerializer() {
}

//==============================================================================
// BinaryTextInputSerializer

BinaryTextInputSerializer::BinaryTextInputSerializer(Stream* stream)
    : m_store(std::make_unique<detail::BinaryArchiveStore3>())
    , m_processing(false) {
    m_store->setupLoad(stream);
    setup(m_store.get(), ArchiveMode::Load);
}

BinaryTextInputSerializer::~BinaryTextInputSerializer() {
}

//Result_deprecated<> BinaryTextInputSerializer::setup(const String& jsonText) {
//    auto result = m_store->setupLoad(jsonText);
//    if (!result) return result;
//    Archive::setup(m_store.get(), ArchiveMode::Load);
//    return ok();
//}

} // namespace ln


#endif
