#include "Internal.hpp"
#include <LuminoEngine/Runtime/Runtime.hpp>
#include "RuntimeManager.hpp"

namespace ln {

LnHandle Runtime::makeObjectWrap(Object* obj)
{
	return detail::EngineDomain::runtimeManager()->makeObjectWrap(obj);
}

LnResult Runtime::processException(Exception* e)
{
	return detail::EngineDomain::runtimeManager()->processException(e);
}

} // namespace ln

extern "C" {

LnResult LnObject_Release(LnHandle obj)
{
	if (obj) {
		ln::detail::EngineDomain::runtimeManager()->releaseObject(obj);
	}
	return LN_SUCCESS;
}

} // extern "C"