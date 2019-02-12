
#include "Internal.hpp"
#include <LuminoEngine/Engine/Object.hpp>
#include <LuminoEngine/Engine/Property.hpp>

namespace ln {

//==============================================================================
// Object


Object::Object()
    : m_weakRefInfo(nullptr)
    , m_weakRefInfoMutex()
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

