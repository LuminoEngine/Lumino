
#include "../Internal.h"
#include <Lumino/Reflection/ReflectionEventArgs.h>

LN_NAMESPACE_BEGIN
namespace tr
{

//==============================================================================
// ReflectionEventArgs
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(ReflectionEventArgs, ReflectionObject);

//------------------------------------------------------------------------------
ReflectionEventArgs::ReflectionEventArgs()
	: handled(false)
{
}

//------------------------------------------------------------------------------
ReflectionEventArgs::~ReflectionEventArgs()
{
}

//==============================================================================
// PropertyChangedEventArgs
//==============================================================================
//LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(PropertyChangedEventArgs, ReflectionEventArgs);

//------------------------------------------------------------------------------
PropertyChangedEventArgs::PropertyChangedEventArgs(const PropertyInfo* prop, PropertySetSource cause_)
	: changedProperty(prop)
	, cause(cause_)
{
}

//------------------------------------------------------------------------------
PropertyChangedEventArgs::~PropertyChangedEventArgs()
{
}

} // namespace tr
LN_NAMESPACE_END
