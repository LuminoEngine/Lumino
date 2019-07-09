#include "Internal.hpp"
#include <LuminoEngine/Runtime/Runtime.hpp>
#include "RuntimeManager.hpp"

namespace ln {

LnHandle Runtime::makeObjectWrap(Object* obj)
{
	return detail::EngineDomain::runtimeManager()->makeObjectWrap(obj);
}

Object* Runtime::getObject(LnHandle handle)
{
	return detail::EngineDomain::runtimeManager()->getObjectEntry(handle)->object;
}

void Runtime::setManagedObjectId(LnHandle handle, int64_t id)
{
	detail::EngineDomain::runtimeManager()->setManagedObjectId(handle, id);
}

int64_t Runtime::getManagedObjectId(LnHandle handle)
{
	return detail::EngineDomain::runtimeManager()->getManagedObjectId(handle);
}

LnResult Runtime::processException(Exception* e)
{
	return detail::EngineDomain::runtimeManager()->processException(e);
}

} // namespace ln

extern "C" {

void LnRuntime_SetManagedObjectId(LnHandle handle, int64_t id)
{
	ln::detail::EngineDomain::runtimeManager()->setManagedObjectId(handle, id);
}

int64_t LnRuntime_GetManagedObjectId(LnHandle handle)
{
	return ln::detail::EngineDomain::runtimeManager()->getManagedObjectId(handle);
}

LnResult LnObject_Release(LnHandle obj)
{
	if (obj) {
		ln::detail::EngineDomain::runtimeManager()->releaseObject(obj);
	}
	return LN_SUCCESS;
}

} // extern "C"