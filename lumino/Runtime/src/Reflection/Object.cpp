
#include "Internal.hpp"
#include <LuminoEngine/Reflection/Object.hpp>
#include <LuminoEngine/Reflection/Property.hpp>
#include <LuminoEngine/Reflection/VMProperty.hpp>
#include <LuminoEngine/Engine/EngineContext2.hpp>

namespace ln {

//==============================================================================
// Object

Ref<ObjectInitializeContext> ObjectInitializeContext::Default = makeRef<ObjectInitializeContext>();
//Ref<ObjectInitializeContext> NoAutoAdd;

Object::Object()
    : m_weakRefInfo(nullptr)
    , m_weakRefInfoMutex()
    , m_runtimeData(nullptr)
    , m_objectFlags(detail::ObjectFlags::None) {
}

Object::~Object() {
    std::lock_guard<std::mutex> lock(m_weakRefInfoMutex);
    if (m_weakRefInfo) {
        m_weakRefInfo->owner = nullptr;
        m_weakRefInfo->release();
        m_weakRefInfo = nullptr;
    }

    if (m_runtimeData) {
        auto c = EngineContext2::instance();
        if (c->objectEventListener) {
            c->objectEventListener->onDestructObject(this);
        }
    }
}

bool Object::init() {
    TypeInfo::initializeObjectProperties(this);
    return true;
}

void Object::finalize() {
    onDispose(false);
}

void Object::dispose() {
    onDispose(true);
}

void Object::onDispose(bool explicitDisposing) {
}

void Object::serialize(Serializer2& ar) {
    onSerialize(&ar);
}

void Object::onSerialize(Serializer2* ar) {
}

void Object::serialize3(Serializer3& ar) {
}

bool Object::traverseRefrection(ReflectionObjectVisitor* visitor) {
    const List<Ref<PropertyInfo>>& props = TypeInfo::getTypeInfo(this)->properties();
    for (auto& prop : props) {
        if (visitor->visitProperty(this, prop)) {
            return true;
        }
    }
    return false;
}

void Object::setTypeInfoOverride(TypeInfo* value) {
    LN_UNREACHABLE();
}

//void Object::reloadAsset()
//{
//	if (!m_assetPath.isNull()) {
//		detail::EngineDomain::assetManager()->loadAssetModelFromAssetPathToInstance(this, m_assetPath);
//	}
//}

//void Object::onSetAssetFilePath(const Path& filePath)
//{
//}

void Object::onRetained() {
    if (m_runtimeData) {
        auto c = EngineContext2::instance();
        if (c->objectEventListener) {
            c->objectEventListener->onRetainedObject(this);
        }
    }
}

void Object::onReleased() {
    if (m_runtimeData) {
        auto c = EngineContext2::instance();
        if (c->objectEventListener) {
            c->objectEventListener->onReleasedObject(this);
        }
    }
}

detail::WeakRefInfo* Object::requestWeakRefInfo() {
    std::lock_guard<std::mutex> lock(m_weakRefInfoMutex);
    if (!m_weakRefInfo) {
        m_weakRefInfo = LN_NEW detail::WeakRefInfo();
        m_weakRefInfo->owner = this;
    }
    return m_weakRefInfo;
}

TypeInfo* Object::_lnref_getTypeInfo() {
    return EngineContext2::instance()->objectTypeInfo();
}

TypeInfo* Object::_lnref_getThisTypeInfo() const {
    return _lnref_getTypeInfo();
}

//==============================================================================
// WeakRefInfo
namespace detail {

WeakRefInfo::WeakRefInfo()
    : owner(nullptr)
    , weakRefCount(1) {
}

void WeakRefInfo::addRef() {
    weakRefCount.fetch_add(1, std::memory_order_relaxed);
}

void WeakRefInfo::release() {
    int before = weakRefCount.fetch_sub(1, std::memory_order_relaxed);
    if (before <= 1) {
        delete this;
    }
}

} // namespace detail

} // namespace ln
