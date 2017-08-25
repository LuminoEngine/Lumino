
#include "../Internal.h"
#include <Lumino/Reflection/TypeInfo.h>
#include <Lumino/Reflection/Notify.h>

LN_NAMESPACE_BEGIN
namespace tr
{

//==============================================================================
// ReflectionEventInfo
//==============================================================================

//------------------------------------------------------------------------------
ReflectionEventInfo::ReflectionEventInfo(TypeInfo* ownerClass, const Char* name, RaiseEventFunc raiseEvent)
	: m_name(name)
	, m_raiseEvent(raiseEvent)
	, m_registerd(false)
{
	ownerClass->registerReflectionEvent(this);
}

//------------------------------------------------------------------------------
ReflectionEventInfo::~ReflectionEventInfo()
{

}

} // namespace tr
LN_NAMESPACE_END
