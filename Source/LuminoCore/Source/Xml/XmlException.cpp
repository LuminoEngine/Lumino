
#include "../Internal.h"
#include <Lumino/Base/Resource.h>
#include <Lumino/Xml/XmlException.h>

LN_NAMESPACE_BEGIN
namespace tr {

//==============================================================================
// XmlException
//==============================================================================
#ifdef LN_EXCEPTION2
LN_EXCEPTION_FORMATTING_CONSTRUCTOR_IMPLEMENT(XmlException);

XmlException::XmlException()
{
	setCaption(InternalResource::getString(InternalResource::XmlException).c_str());
}

Exception* XmlException::copy() const
{
	return LN_NEW XmlException(*this);
}

#else
LN_EXCEPTION_BASIC_CONSTRUCTOR_IMPLEMENT(XmlException, InternalResource::XmlException);
#endif

namespace detail
{

} // namespace detail

} // namespace tr
LN_NAMESPACE_END
