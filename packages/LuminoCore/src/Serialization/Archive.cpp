#include <LuminoCore/Serialization/ArchiveStore.hpp>
#include <LuminoCore/Runtime/Object.hpp>
#include <LuminoCore/Serialization/Archive.hpp>

namespace ln {

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
            setCurrentNodeType(NodeType::WrapperObject);
        }
    }
    else if (isLoading()) {
        *outIsNull = m_store->getReadingValueType() == ArchiveNodeType::Null;
        setCurrentNodeType(NodeType::WrapperObject);
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
        setCurrentNodeType(NodeType::WrapperObject);
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

} // namespace ln

