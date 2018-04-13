
#include "Internal.hpp"
#include <Lumino/Base/Assertion.hpp>
#include <Lumino/Base/RefObject.hpp>

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
	LN_FATAL(m_referenceCount <= 1, "Object is still referenced.");
}

void RefObject::finalize()
{
	LN_FATAL(m_referenceCount <= 1, "Object is still referenced.");
}

int32_t RefObject::getReferenceCount() const
{ 
	return m_referenceCount;
}

int32_t RefObject::addRef()
{
	return m_referenceCount++;
}

int32_t RefObject::release()
{
    int32_t count = (--m_referenceCount);
	int32_t count2 = m_internalReferenceCount;
	if (count <= 0 && count2 <= 0)
	{
		finalize();
		delete this;
	}
    return count;
}

void RefObject::releaseInternal()
{
	int32_t count = m_referenceCount;
	int32_t count2 = (--m_internalReferenceCount);
	if (count <= 0 && count2 <= 0)
	{
		finalize();
		delete this;
	}
}

} // namespace ln
