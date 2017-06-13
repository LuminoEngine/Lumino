
#include "../Internal.h"
#include <Lumino/Base/Exception.h>
#include <Lumino/Base/RefObject.h>

LN_NAMESPACE_BEGIN

//==============================================================================
// RefObject
//==============================================================================

//------------------------------------------------------------------------------
RefObject::RefObject()
	: m_referenceCount(1)
	, m_internalReferenceCount(0)
{
}

//------------------------------------------------------------------------------
RefObject::~RefObject()
{
	LN_FATAL(m_referenceCount.get() <= 1, "Object is still referenced.");
}

//------------------------------------------------------------------------------
void RefObject::finalize_()
{
	LN_FATAL(m_referenceCount.get() <= 1, "Object is still referenced.");
}

//------------------------------------------------------------------------------
int32_t RefObject::getReferenceCount() const
{ 
	return m_referenceCount.get();
}

//------------------------------------------------------------------------------
int32_t RefObject::addRef()
{
	return m_referenceCount.increment();
}

//------------------------------------------------------------------------------
int32_t RefObject::release()
{
    int32_t count = m_referenceCount.decrement();
	int32_t count2 = m_internalReferenceCount;
	if (count <= 0 && count2 <= 0)
	{
		finalize_();
		delete this;
	}
    return count;
}

//------------------------------------------------------------------------------
void RefObject::releaseInternal()
{
	int32_t count = m_referenceCount.get();
	int32_t count2 = (--m_internalReferenceCount);
	if (count <= 0 && count2 <= 0)
	{
		finalize_();
		delete this;
	}
}

LN_NAMESPACE_END
