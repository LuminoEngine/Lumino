
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
	LN_FATAL(m_referenceCount.Get() <= 1, "Object is still referenced.");
}

//------------------------------------------------------------------------------
void RefObject::Finalize_()
{
	LN_FATAL(m_referenceCount.Get() <= 1, "Object is still referenced.");
}

//------------------------------------------------------------------------------
int32_t RefObject::getReferenceCount() const
{ 
	return m_referenceCount.Get();
}

//------------------------------------------------------------------------------
int32_t RefObject::addRef()
{
	return m_referenceCount.Increment();
}

//------------------------------------------------------------------------------
int32_t RefObject::release()
{
    int32_t count = m_referenceCount.Decrement();
	int32_t count2 = m_internalReferenceCount;
	if (count <= 0 && count2 <= 0)
	{
		Finalize_();
		delete this;
	}
    return count;
}

//------------------------------------------------------------------------------
void RefObject::releaseInternal()
{
	int32_t count = m_referenceCount.Get();
	int32_t count2 = (--m_internalReferenceCount);
	if (count <= 0 && count2 <= 0)
	{
		Finalize_();
		delete this;
	}
}

LN_NAMESPACE_END
