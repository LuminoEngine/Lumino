
#include "Internal.hpp"
#include <LuminoCore/Base/Assertion.hpp>
#include <LuminoCore/Base/RefObject.hpp>

namespace ln {

//==============================================================================
// RefObject

RefObject::RefObject()
    : m_referenceCount(1)
    , m_internalReferenceCount(0)
    , m_ojectFlags(1)
{
}

RefObject::~RefObject()
{
    if (m_referenceCount > 1) {
        LN_ERROR("Forcibly delete the instance with the remaining reference. (0x{}, refCount:{})", LN_FMT_NAMESPACE::ptr(this), m_referenceCount.load());
        LN_CHECK(m_referenceCount <= 1);
    }
}

void RefObject::finalize()
{
	LN_CHECK(m_referenceCount <= 1);
}

int32_t RefObject::getReferenceCount() const
{
    return m_referenceCount;
}

int32_t RefObject::retain()
{
    m_referenceCount++;

	if (getObjectFlag(detail::RefObjectFlags_ReferenceTracking)) {
		onRetained();
	}

	return m_referenceCount;
}

int32_t RefObject::release()
{
    const int32_t count = (--m_referenceCount);
    const int32_t count2 = m_internalReferenceCount;

	if (getObjectFlag(detail::RefObjectFlags_ReferenceTracking)) {
		onReleased();
	}

    if (count <= 0 && count2 <= 0) {
        finalize();
        delete this;
    }
    return count;
}

void RefObject::setObjectFlag(uint32_t flag, bool value)
{
	if (value) {
		m_ojectFlags |= flag;
	}
	else {
		m_ojectFlags &= ~flag;
	}
}

uint32_t RefObject::getObjectFlag(uint32_t flag) const
{
	return (m_ojectFlags & flag) == flag;
}

void RefObject::releaseInternal()
{
    int32_t count = m_referenceCount;
    int32_t count2 = (--m_internalReferenceCount);
    if (count <= 0 && count2 <= 0) {
        finalize();
        delete this;
    }
}

void RefObject::onRetained()
{
}

void RefObject::onReleased()
{
}

//==============================================================================
// RefObjectHelper

int32_t RefObjectHelper::getReferenceCount(RefObject* obj)
{
    if (LN_REQUIRE(obj)) return 0;
    return obj->getReferenceCount();
}

int32_t RefObjectHelper::retain(RefObject* obj)
{
    if (LN_REQUIRE(obj)) return 0;
    return obj->retain();
}

int32_t RefObjectHelper::release(RefObject* obj)
{
    if (LN_REQUIRE(obj)) return 0;
    return obj->release();
}

} // namespace ln
