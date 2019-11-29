
#include "Internal.hpp"
#include <LuminoEngine/Engine/Object.hpp>
#include <LuminoEngine/Engine/Property.hpp>
#include "../Runtime/RuntimeManager.hpp"

namespace ln {

//==============================================================================
// Object


Object::Object()
    : m_weakRefInfo(nullptr)
    , m_weakRefInfoMutex()
	, m_runtimeData(nullptr)
{
}

Object::~Object()
{
	std::lock_guard<std::mutex> lock(m_weakRefInfoMutex);
	if (m_weakRefInfo)
	{
		m_weakRefInfo->owner = nullptr;
		m_weakRefInfo->release();
		m_weakRefInfo = nullptr;
	}

	if (m_runtimeData) {
		detail::EngineDomain::runtimeManager()->onDestructObject(this);
	}
}

void Object::init()
{
    TypeInfo::initializeObjectProperties(this);
}

void Object::finalize()
{
	onDispose(false);
}

void Object::dispose()
{
	onDispose(true);
}

void Object::onDispose(bool explicitDisposing)
{
}

void Object::serialize(Archive& ar)
{
    const List<Ref<PropertyInfo>>& props = TypeInfo::getTypeInfo(this)->properties();
    for (auto& prop : props) {
        prop->accessor()->serializeMember(this, ar, prop->name());
    }
}

bool Object::traverseRefrection(ReflectionObjectVisitor* visitor)
{
	const List<Ref<PropertyInfo>>& props = TypeInfo::getTypeInfo(this)->properties();
	for (auto& prop : props) {
		if (visitor->visitProperty(this, prop)) {
			return true;
		}
	}
	return false;
}

void Object::onSetAssetFilePath(const Path& filePath)
{
}

void Object::onRetained()
{
	if (m_runtimeData) {
		detail::EngineDomain::runtimeManager()->onRetainedObject(this);
	}
}

void Object::onReleased()
{
	if (m_runtimeData) {
		detail::EngineDomain::runtimeManager()->onReleasedObject(this);
	}
}

detail::WeakRefInfo* Object::requestWeakRefInfo()
{
    std::lock_guard<std::mutex> lock(m_weakRefInfoMutex);
    if (!m_weakRefInfo)
    {
        m_weakRefInfo = LN_NEW detail::WeakRefInfo();
        m_weakRefInfo->owner = this;
    }
    return m_weakRefInfo;
}

TypeInfo* Object::_lnref_getTypeInfo()
{
    static TypeInfo typeInfo("Object", nullptr);
    return &typeInfo;
}

TypeInfo* Object::_lnref_getThisTypeInfo() const
{
    return _lnref_getTypeInfo();
}

//==============================================================================
// WeakRefInfo
namespace detail {

WeakRefInfo::WeakRefInfo()
    : owner(nullptr)
    , weakRefCount(1)
{}

void WeakRefInfo::addRef()
{
    weakRefCount.fetch_add(1, std::memory_order_relaxed);
}

void WeakRefInfo::release()
{
    int before = weakRefCount.fetch_sub(1, std::memory_order_relaxed);
    if (before <= 1)
    {
        delete this;
    }
}

} // namespace detail

} // namespace ln

