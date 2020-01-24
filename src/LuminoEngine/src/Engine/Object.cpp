
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

bool Object::init()
{
    TypeInfo::initializeObjectProperties(this);
	return true;
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

    onSerialize(ar.m_serializer);

    printf("[Engine] end onSerialize\n");
}

void Object::onSerialize(Serializer* ar)
{

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

void Object::setTypeInfoOverride(TypeInfo* value)
{
    LN_UNREACHABLE();
}

//void Object::onSetAssetFilePath(const Path& filePath)
//{
//}

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
	return EngineContext::current()->objectTypeInfo();
    //static TypeInfo typeInfo();
    //return &typeInfo;
}

TypeInfo* Object::_lnref_getThisTypeInfo() const
{
    return _lnref_getTypeInfo();
}

//==============================================================================
// TypeInfo

TypeInfo::TypeInfo(const char* className, TypeInfo* baseType)
	: m_name(className)
	, m_baseType(baseType)
	, m_managedTypeInfoId(-1)
{
}

TypeInfo::TypeInfo(const String& className)
	: m_name(className)
	, m_baseType(nullptr)
	, m_managedTypeInfoId(-1)
{
}

TypeInfo::~TypeInfo()
{
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

