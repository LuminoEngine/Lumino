#include "Internal.hpp"
#include <cstdarg>
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

static std::string std_string_vprintf(const char* format, std::va_list arg)
{
    std::string ret;
    ret.resize(32);
    auto n = static_cast<std::size_t>(vsnprintf(&ret[0], ret.size(), format, arg));

//#if defined _MSC_VER
//    if (n == static_cast<std::size_t>(-1))
//    {
//        n = _vscprintf(format, arg) + 1;
//#else
    if (n > ret.size())
    {
//#endif
        ret.resize(n + 1);
        n = vsnprintf(&ret[0], ret.size(), format, arg);
    }
    ret.resize(n);
    return ret;
}

//extern RuntimeManager::Settings g_globalRuntimeManagerSettings;

} // namespace detail
} // namespace ln

extern "C" {

void LnRuntime_Initialize(const tagLnRuntimeSettings* settings)
{
    if (settings) {
        auto& s = ln::detail::RuntimeManager::s_globalSettings;
        s.runtimeFinalizedCallback = settings->runtimeFinalizedCallback;
        s.referenceCountTrackerCallback = settings->referenceCountTrackerCallback;
        s.runtimeGetTypeInfoIdCallback = settings->runtimeGetTypeInfoIdCallback;
    }

	ln::EngineContext::current()->initializeRuntimeManager();
    //ln::detail::EngineDomain::runtimeManager();
    //auto manager = ln::detail::EngineDomain::engineManager();
    //manager->initializeRuntimeManager();
}

void LnRuntime_Finalize()
{
	ln::EngineContext::current()->disposeRuntimeManager();
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

//void LnRuntime_SetReferenceCountTracker(LnReferenceCountTrackerCallback callback)
//{
//	return ln::detail::RuntimeManager::setReferenceCountTracker(callback);
//}

void LnRuntime_SetReferenceTrackEnabled(LnHandle handle)
{
	return ln::detail::EngineDomain::runtimeManager()->setReferenceTrackEnabled(handle);
}

//void LnRuntime_SetRuntimeFinalizedCallback(LnRuntimeFinalizedCallback callback)
//{
//    return ln::detail::RuntimeManager::setRuntimeFinalizedCallback(callback);
//}

//void LnRuntime_SetRuntimeCreateInstanceCallback(LnRuntimeCreateInstanceCallback callback)
//{
//    return ln::detail::RuntimeManager::setRuntimeCreateInstanceCallback(callback);
//}

//void LnRuntime_SetRuntimeGetTypeInfoIdCallback(LnRuntimeGetTypeInfoIdCallback callback)
//{
//    return ln::detail::RuntimeManager::setRuntimeGetTypeInfoIdCallback(callback);
//}

void LnRuntime_RunAppInternal(LnHandle app)
{
    ln::detail::ApplicationHelper::run(
        static_cast<ln::Application*>(ln::detail::EngineDomain::runtimeManager()->getObjectFromHandle(app)));
}

void LnInternalEngineSettings_SetEngineResourcesPathA(const char* path)
{
	ln::detail::EngineManager::s_settings.engineResourcesPath = ln::String::fromCString(path);
}

//==============================================================================
// LnTypeInfo

LnResult LnTypeInfo_Acquire(const LnChar* typeName, int* outTypeInfoId)
{
    LNI_FUNC_TRY_BEGIN;
    if (LN_REQUIRE(outTypeInfoId)) return LN_ERROR_UNKNOWN;
    if (ln::TypeInfo* t = ln::EngineContext::current()->acquireTypeInfo(typeName)) {
        *outTypeInfoId = t->id();
        return LN_SUCCESS;
    }
    else {
        return LN_ERROR_UNKNOWN;
    }
    LNI_FUNC_TRY_END_RETURN;
}

LnResult LnTypeInfo_AcquireA(const char* typeName, int* outTypeInfoId)
{
    return LnTypeInfo_Acquire(ln::String::fromCString(typeName).c_str(), outTypeInfoId);
}

LnResult LnTypeInfo_Find(const LnChar* typeName, int* outTypeInfoId)
{
    LNI_FUNC_TRY_BEGIN;
    if (LN_REQUIRE(outTypeInfoId)) return LN_ERROR_UNKNOWN;
    if (ln::TypeInfo* t = ln::EngineContext::current()->findTypeInfo(typeName)) {
        *outTypeInfoId = t->id();
        return LN_SUCCESS;
    }
    else {
        return LN_ERROR_UNKNOWN;
    }
    LNI_FUNC_TRY_END_RETURN;
}

LnResult LnTypeInfo_FindA(const char* typeName, int* outTypeInfoId)
{
    return LnTypeInfo_Find(ln::String::fromCString(typeName).c_str(), outTypeInfoId);
}

LnResult LnTypeInfo_SetBaseClass(int typeInfoId, int baseClassTypeInfoId)
{
    LNI_FUNC_TRY_BEGIN;
    ln::TypeInfo* t = ln::EngineContext::current()->findTypeInfo(typeInfoId);
    ln::TypeInfo* b = ln::EngineContext::current()->findTypeInfo(baseClassTypeInfoId);
    if (t && b) {
        t->m_baseType = b;
        return LN_SUCCESS;
    }
    else {
        return LN_ERROR_UNKNOWN;
    }
    LNI_FUNC_TRY_END_RETURN;
}

LnResult LnTypeInfo_SetCreateInstanceCallback(int typeInfoId, LnTypeInfoCreateInstanceCallback callback)
{
    LNI_FUNC_TRY_BEGIN;
    if (ln::TypeInfo* t = ln::EngineContext::current()->findTypeInfo(typeInfoId)) {
        t->m_factory = [callback](const ln::TypeInfo* typeInfo) -> ln::Ref<ln::Object> {
            LnHandle handle = LN_NULL_HANDLE;
            callback(typeInfo->id(), &handle);
            if (handle == LN_NULL_HANDLE) {
                LN_ERROR("Faild creation managed instance.");
                return nullptr;
            }
            // TODO: 参照カウントこれでよい？
            auto ref = ln::detail::EngineDomain::runtimeManager()->getObjectFromHandle(handle);
            printf("[Engine] [%p] RefCount: %d\n", ref, ln::RefObjectHelper::getReferenceCount(ref));
            return ref;
        };
        return LN_SUCCESS;
    }
    else {
        return LN_ERROR_UNKNOWN;
    }
    LNI_FUNC_TRY_END_RETURN;
}

LnResult LnTypeInfo_SetManagedTypeInfoId(int typeInfoId, int managedTypeInfoId)
{
    LNI_FUNC_TRY_BEGIN;
    if (ln::TypeInfo* t = ln::EngineContext::current()->findTypeInfo(typeInfoId)) {
        ln::detail::TypeInfoInternal::setManagedTypeInfoId(t, managedTypeInfoId);
        return LN_SUCCESS;
    }
    else {
        return LN_ERROR_UNKNOWN;
    }
    LNI_FUNC_TRY_END_RETURN;
}

LnResult LnTypeInfo_GetManagedTypeInfoId(int typeInfoId, int* outManagedTypeInfoId)
{
    LNI_FUNC_TRY_BEGIN;
    if (ln::TypeInfo* t = ln::EngineContext::current()->findTypeInfo(typeInfoId)) {
        *outManagedTypeInfoId = ln::detail::TypeInfoInternal::getManagedTypeInfoId(t);
        return LN_SUCCESS;
    }
    else {
        return LN_ERROR_UNKNOWN;
    }
    LNI_FUNC_TRY_END_RETURN;
}

//==============================================================================
// LnObject

LnResult LnObject_Release(LnHandle obj)
{
    if (auto m = ln::detail::EngineDomain::runtimeManager()) {
        m->releaseObjectExplicitly(obj);
		return LN_SUCCESS;
    }
	else {
		return LN_RUNTIME_UNINITIALIZED;
	}
}

LnResult LnObject_Retain(LnHandle obj)
{
	if (auto m = ln::detail::EngineDomain::runtimeManager()) {
		m->retainObjectExplicitly(obj);
		return LN_SUCCESS;
	}
	else {
		return LN_RUNTIME_UNINITIALIZED;
	}
}

int32_t LnObject_GetReferenceCount(LnHandle obj)
{
	if (auto t = LNI_HANDLE_TO_OBJECT(ln::Object, obj))
		return ln::RefObjectHelper::getReferenceCount(t);
	else
		return 0;
}

LnResult LnObject_SetTypeInfoId(LnHandle obj, int typeInfoId)
{
    LNI_FUNC_TRY_BEGIN;
    if (ln::TypeInfo* t = ln::EngineContext::current()->findTypeInfo(typeInfoId)) {
        LNI_HANDLE_TO_OBJECT(ln::Object, obj)->setTypeInfoOverride(t);
        return LN_SUCCESS;
    }
    else {
        return LN_ERROR_UNKNOWN;
    }
    LNI_FUNC_TRY_END_RETURN;
}

//==============================================================================

void LnLog_SetLevel(LnLogLevel level)
{
	ln::GlobalLogger::addStdErrAdapter();	// TODO: とりいそぎ
    ln::GlobalLogger::setLevel(static_cast<ln::LogLevel>(level));
}

void LnLog_Write(LnLogLevel level, const LnChar* tag, const LnChar* text)
{
    LN_LOG(static_cast<ln::LogLevel>(level), ln::String(tag).toStdString().c_str()) << ln::String(text);
}

void LnLog_WriteA(LnLogLevel level, const char* tag, const char* text)
{
    LN_LOG(static_cast<ln::LogLevel>(level), tag) << ln::String::fromCString(text);
}

void LnLog_PrintA(LnLogLevel level, const char* tag, const char* format, ...)
{
    if (::ln::detail::LoggerInterface::getInstance()->checkLevel(static_cast<ln::LogLevel>(level))) {
        std::va_list arg;
        va_start(arg, format);
        auto str = ln::detail::std_string_vprintf(format, arg);
        va_end(arg);
        LnLog_WriteA(level, tag, str.c_str());
    }
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