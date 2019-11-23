#include "Internal.hpp"
#include <LuminoEngine/Runtime/Runtime.hpp>
#include "RuntimeManager.hpp"

namespace ln {

LnHandle Runtime::makeObjectWrap(Object* obj, bool fromCreate)
{
	return detail::EngineDomain::runtimeManager()->makeObjectWrap(obj, fromCreate);
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

int64_t LnRuntime_GetManagedTypeInfoId(LnHandle handle)
{
	return ln::detail::EngineDomain::runtimeManager()->getManagedTypeInfoId(handle);
}

//void LnRuntime_UTF8ToNativeString(const char* src, std::u16string* dst)
//{
//	*dst = ln::String::fromCString(src, -1, ln::TextEncoding::utf8Encoding()).c_str();
//}
//
LN_FLAT_API LnResult LnObject_Release(LnHandle obj)
{
	ln::detail::EngineDomain::runtimeManager()->releaseObjectExplicitly(obj);
	return LN_SUCCESS;
}

LN_FLAT_API LnResult LnObject_Retain(LnHandle obj)
{
	ln::detail::EngineDomain::runtimeManager()->retainObjectExplicitly(obj);
	return LN_SUCCESS;
}

LN_FLAT_API int32_t LnObject_GetReferenceCount(LnHandle obj)
{
	if (auto t = LNI_HANDLE_TO_OBJECT(ln::Object, obj))
		return ln::RefObjectHelper::getReferenceCount(t);
	else
		return 0;
}

} // extern "C"