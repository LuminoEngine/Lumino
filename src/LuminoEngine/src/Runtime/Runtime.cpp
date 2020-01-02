#include "Internal.hpp"
#include <LuminoEngine/Engine/Application.hpp>
#include <LuminoEngine/Runtime/Runtime.hpp>
#include "../Engine/EngineManager.hpp"
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

const Char* Runtime::getUTF16StringPtr(const String& str)
{
    return str.c_str();
}

const char* Runtime::getUTF8StringPtr(const String& str)
{
    auto* sb = detail::EngineDomain::runtimeManager()->requestCommonStringBuffer();
    sb->reset(str);
    return sb->getUtf8();
}


//==============================================================================
// ShapesRendererCommandList

namespace detail {

RuntimeStringBuffer::RuntimeStringBuffer()
    : m_translated(false)
{
}

//void RuntimeStringBuffer::init(const String& str)
//{
//    Object::init();
//    m_str = str;
//}

void RuntimeStringBuffer::reset(const String& str)
{
    m_str = str;
    m_translated = false;
}

const char* RuntimeStringBuffer::getUtf8()
{
    if (m_translated) {
        m_str8 = m_str.toStdString();   // TODO: encoding
        m_translated = true;
    }
    return m_str8.c_str();
}

} // namespace detail
} // namespace ln

extern "C" {

void LnRuntime_Initialize()
{
    auto manager = ln::detail::EngineDomain::engineManager();
    manager->initializeRuntimeManager();
}

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

void LnRuntime_SetReferenceCountTracker(LnReferenceCountTrackerCallback callback)
{
	return ln::detail::RuntimeManager::setReferenceCountTracker(callback);
}

void LnRuntime_SetReferenceTrackEnabled(LnHandle handle)
{
	return ln::detail::EngineDomain::runtimeManager()->setReferenceTrackEnabled(handle);
}

void LnRuntime_SetRuntimeFinalizedCallback(LnRuntimeFinalizedCallback callback)
{
    return ln::detail::RuntimeManager::setRuntimeFinalizedCallback(callback);
}

void LnRuntime_RunAppInternal(LnHandle app)
{
    ln::detail::ApplicationHelper::run(
        static_cast<ln::Application*>(ln::detail::EngineDomain::runtimeManager()->getObjectFromHandle(app)));
}

LN_FLAT_API LnResult LnObject_Release(LnHandle obj)
{
    if (auto m = ln::detail::EngineDomain::runtimeManager()) {
        m->releaseObjectExplicitly(obj);
    }
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

#if 0
//==============================================================================

/**
    @brief	文字列をラップします。
*/

/**
    @brief		文字列を UTF16 エンコーディングとして取得します。
    @param[in]	stringBuffer : StringBuffer
    @param[out]	outReturn    : string
*/
LN_FLAT_API LnResult LnStringBuffer_GetUTF16String(LnHandle stringBuffer, const LnChar** outReturn);

/**
    @brief		文字列を UTF8 エンコーディングとして取得します。
    @param[in]	stringBuffer : StringBuffer
    @param[out]	outReturn    : string
*/
LN_FLAT_API LnResult LnStringBuffer_GetUTF8String(LnHandle stringBuffer, const char** outReturn);

LN_FLAT_API LnResult LnStringBuffer_GetUTF16String(LnHandle stringBuffer, const LnChar** outReturn)
{
    *outReturn = LNI_HANDLE_TO_OBJECT(ln::RuntimeStringBuffer, stringBuffer)->getUtf16();
}

LN_FLAT_API LnResult LnStringBuffer_GetUTF8String(LnHandle stringBuffer, const char** outReturn)
{
    *outReturn = LNI_HANDLE_TO_OBJECT(ln::RuntimeStringBuffer, stringBuffer)->getUtf8();
}
#endif

} // extern "C"