
#include "../Internal.h"
#include <Lumino/Base/Resource.h>
#include <Lumino/Xml/XmlException.h>

LN_NAMESPACE_BEGIN
namespace tr {

#ifdef LN_EXCEPTION2
#else
//==============================================================================
// XmlException
//==============================================================================
LN_EXCEPTION_BASIC_CONSTRUCTOR_IMPLEMENT(XmlException, InternalResource::XmlException);
#endif

namespace detail
{

} // namespace detail

} // namespace tr
LN_NAMESPACE_END
