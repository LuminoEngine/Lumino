#include "Internal.hpp"
#include <cstdarg>
#include <LuminoEngine/Engine/Application.hpp>
#include <LuminoFFI/FFI.hpp>
#include <LuminoFFI/Lumino.FlatC.generated.h>
#include "../../LuminoEngine/src/Engine/EngineManager.hpp"
#include <LuminoEngine/Runtime/detail/RuntimeManager.hpp>

namespace ln {

LNResult FFI::processException(Exception* e) {
    return LN_ERROR_UNKNOWN;
}

LNHandle FFI::makeObjectWrap(Object* obj, bool fromCreate)
{
    return detail::RuntimeManager::instance()->makeObjectWrap(obj, fromCreate);
}

Object* FFI::getObject(LNHandle handle)
{
	return detail::RuntimeManager::instance()->getObjectEntry(handle)->object;
}

void FFI::setManagedObjectId(LNHandle handle, int64_t id)
{
	detail::RuntimeManager::instance()->setManagedObjectId(handle, id);
}

int64_t FFI::getManagedObjectId(LNHandle handle)
{
	return detail::RuntimeManager::instance()->getManagedObjectId(handle);
}

//const Char* FFI::getUTF16StringPtr(String str)
//{
//    return str.c_str();
//}

const Char* FFI::getUTF16StringPtr(const String& str)
{
    // TODO: Assets::readAllText() のように、String の実態を返す関数の対策のため、必ず reset している。
    // 参照で返してくるものはふつうに return str.c_str(); すればよいだけなので、そのように最適化したい。
    auto* sb = detail::RuntimeManager::instance()->requestCommonStringBuffer();
    sb->reset(str);
    return sb->getUtf16();

    //return str.c_str();
}

const char* FFI::getAStringPtr(const String& str)
{
    auto* sb = detail::RuntimeManager::instance()->requestCommonStringBuffer();
    sb->reset(str);
    return sb->getAscii();
}

void FFI::setAStringEncoding(TextEncoding* value)
{
    detail::RuntimeManager::instance()->setAStringEncoding(value);
}

TextEncoding* FFI::getAStringEncoding()
{
    return detail::RuntimeManager::instance()->getAStringEncoding();
}

//==============================================================================
// ShapesRendererCommandList

namespace detail {

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

void registerTypes_Graphics(EngineContext2* context);
void registerTypes_Engine(EngineContext2* context);

} // namespace ln

extern "C" {

void LNRuntime_Initialize(const tagLNRuntimeSettings* settings)
{
    if (settings) {
        auto& s = ln::detail::RuntimeManager::s_globalSettings;
        s.runtimeFinalizedCallback = settings->runtimeFinalizedCallback;
        s.referenceCountTrackerCallback = settings->referenceCountTrackerCallback;
        s.runtimeGetTypeInfoIdCallback = settings->runtimeGetTypeInfoIdCallback;
    }

    ln::EngineContext2::initialize();
    auto* context = ln::EngineContext2::instance();
	ln::detail::RuntimeManager::initialize(ln::detail::RuntimeManager::s_globalSettings);
    ln::registerTypes_Graphics(context);
    ln::registerTypes_Engine(context);
}

void LNRuntime_Finalize()
{
    ln::detail::RuntimeManager::terminate();
    ln::EngineContext2::terminate();
}

void LNRuntime_SetManagedObjectId(LNHandle handle, int64_t id)
{
	ln::detail::RuntimeManager::instance()->setManagedObjectId(handle, id);
}

int64_t LNRuntime_GetManagedObjectId(LNHandle handle)
{
	return ln::detail::RuntimeManager::instance()->getManagedObjectId(handle);
}

int64_t LNRuntime_GetManagedTypeInfoId(LNHandle handle)
{
	return ln::detail::RuntimeManager::instance()->getManagedTypeInfoId(handle);
}

//void LNRuntime_SetReferenceCountTracker(LNReferenceCountTrackerCallback callback)
//{
//	return ln::detail::RuntimeManager::setReferenceCountTracker(callback);
//}

void LNRuntime_SetReferenceTrackEnabled(LNHandle handle)
{
	return ln::detail::RuntimeManager::instance()->setReferenceTrackEnabled(handle);
}

//void LNRuntime_SetRuntimeFinalizedCallback(LNRuntimeFinalizedCallback callback)
//{
//    return ln::detail::RuntimeManager::setRuntimeFinalizedCallback(callback);
//}

//void LNRuntime_SetRuntimeCreateInstanceCallback(LNRuntimeCreateInstanceCallback callback)
//{
//    return ln::detail::RuntimeManager::setRuntimeCreateInstanceCallback(callback);
//}

//void LNRuntime_SetRuntimeGetTypeInfoIdCallback(LNRuntimeGetTypeInfoIdCallback callback)
//{
//    return ln::detail::RuntimeManager::setRuntimeGetTypeInfoIdCallback(callback);
//}

void LNRuntime_RunAppInternal(LNHandle app)
{
    ln::detail::ApplicationHelper::run(
        static_cast<ln::Application*>(ln::detail::RuntimeManager::instance()->getObjectFromHandle(app)));
}

void LNRuntime_DumpInfo()
{
    ln::detail::RuntimeManager::instance()->dumpInfo();
}

void LNInternalEngineSettings_SetEngineResourcesPathA(const char* path)
{
	ln::detail::EngineManager::s_settings.engineResourcesPath = ln::String::fromCString(path);
}

//==============================================================================
// LNTypeInfo

LNResult LNTypeInfo_Acquire(const LNChar* typeName, int32_t* outTypeInfoId)
{
    LNI_FUNC_TRY_BEGIN;
    if (LN_REQUIRE(outTypeInfoId)) return LN_ERROR_UNKNOWN;
    if (ln::TypeInfo* t = ln::EngineContext2::instance()->acquireTypeInfo(typeName)) {
        *outTypeInfoId = t->id();
        return LN_OK;
    }
    else {
        return LN_ERROR_UNKNOWN;
    }
    LNI_FUNC_TRY_END_RETURN;
}

LNResult LNTypeInfo_AcquireA(const char* typeName, int32_t* outTypeInfoId)
{
    return LNTypeInfo_Acquire(ln::String::fromCString(typeName).c_str(), outTypeInfoId);
}

LNResult LNTypeInfo_Find(const LNChar* typeName, int32_t* outTypeInfoId)
{
    LNI_FUNC_TRY_BEGIN;
    if (LN_REQUIRE(outTypeInfoId)) return LN_ERROR_UNKNOWN;
    if (ln::TypeInfo* t = ln::EngineContext2::instance()->findTypeInfo(typeName)) {
        *outTypeInfoId = t->id();
        return LN_OK;
    }
    else {
        return LN_ERROR_UNKNOWN;
    }
    LNI_FUNC_TRY_END_RETURN;
}

LNResult LNTypeInfo_FindA(const char* typeName, int32_t* outTypeInfoId)
{
    return LNTypeInfo_Find(ln::String::fromCString(typeName).c_str(), outTypeInfoId);
}

LNResult LNTypeInfo_SetBaseClass(int32_t typeInfoId, int32_t baseClassTypeInfoId)
{
    LNI_FUNC_TRY_BEGIN;
    ln::TypeInfo* t = ln::EngineContext2::instance()->findTypeInfo(typeInfoId);
    ln::TypeInfo* b = ln::EngineContext2::instance()->findTypeInfo(baseClassTypeInfoId);
    if (t && b) {
        t->m_baseType = b;
        return LN_OK;
    }
    else {
        return LN_ERROR_UNKNOWN;
    }
    LNI_FUNC_TRY_END_RETURN;
}

LNResult LNTypeInfo_SetCreateInstanceCallback(int32_t typeInfoId, LNTypeInfoCreateInstanceCallback callback)
{
    LNI_FUNC_TRY_BEGIN;
    if (ln::TypeInfo* t = ln::EngineContext2::instance()->findTypeInfo(typeInfoId)) {
        t->m_factory = [callback](const ln::TypeInfo* typeInfo) -> ln::Ref<ln::Object> {
            LNHandle handle = LN_NULL_HANDLE;
            callback(typeInfo->id(), &handle);
            if (handle == LN_NULL_HANDLE) {
                LN_ERROR("Faild creation managed instance.");
                return nullptr;
            }
            // TODO: 参照カウントこれでよい？
            auto ref = ln::detail::RuntimeManager::instance()->getObjectFromHandle(handle);
            printf("[Engine] [%p] RefCount: %d\n", ref, ln::RefObjectHelper::getReferenceCount(ref));
            return ref;
        };
        return LN_OK;
    }
    else {
        return LN_ERROR_UNKNOWN;
    }
    LNI_FUNC_TRY_END_RETURN;
}

LNResult LNTypeInfo_SetManagedTypeInfoId(int32_t typeInfoId, int32_t managedTypeInfoId)
{
    LNI_FUNC_TRY_BEGIN;
    if (ln::TypeInfo* t = ln::EngineContext2::instance()->findTypeInfo(typeInfoId)) {
        ln::detail::TypeInfoInternal::setManagedTypeInfoId(t, managedTypeInfoId);
        return LN_OK;
    }
    else {
        return LN_ERROR_UNKNOWN;
    }
    LNI_FUNC_TRY_END_RETURN;
}

LNResult LNTypeInfo_GetManagedTypeInfoId(int32_t typeInfoId, int32_t* outManagedTypeInfoId)
{
    LNI_FUNC_TRY_BEGIN;
    if (ln::TypeInfo* t = ln::EngineContext2::instance()->findTypeInfo(typeInfoId)) {
        *outManagedTypeInfoId = static_cast<int32_t>(ln::detail::TypeInfoInternal::getManagedTypeInfoId(t));
        return LN_OK;
    }
    else {
        return LN_ERROR_UNKNOWN;
    }
    LNI_FUNC_TRY_END_RETURN;
}

//==============================================================================
// LNObject

LNResult LNObject_Release(LNHandle obj)
{
    if (auto m = ln::detail::RuntimeManager::instance()) {
        m->releaseObjectExplicitly(obj);
		return LN_OK;
    }
	else {
		return LN_RUNTIME_UNINITIALIZED;
	}
}

LNResult LNObject_Retain(LNHandle obj)
{
	if (auto m = ln::detail::RuntimeManager::instance()) {
		m->retainObjectExplicitly(obj);
		return LN_OK;
	}
	else {
		return LN_RUNTIME_UNINITIALIZED;
	}
}

LNResult LNObject_GetReferenceCount(LNHandle obj, int* outReturn)
{
    if (!outReturn) return LN_ERROR_INVALID_ARGUMENT;

    if (auto t = LNI_HANDLE_TO_OBJECT(ln::Object, obj)) {
        *outReturn = ln::RefObjectHelper::getReferenceCount(t);
        return LN_OK;
    }
	else
		return LN_ERROR_INVALID_ARGUMENT;
}

int32_t _LNObject_GetReferenceCount(LNHandle obj)
{
    if (auto t = LNI_HANDLE_TO_OBJECT(ln::Object, obj)) {
        return ln::RefObjectHelper::getReferenceCount(t);
    }
    else
        return 0;
}

LNResult LNObject_SetTypeInfoId(LNHandle obj, int typeInfoId)
{
    LNI_FUNC_TRY_BEGIN;
    if (ln::TypeInfo* t = ln::EngineContext2::instance()->findTypeInfo(typeInfoId)) {
        LNI_HANDLE_TO_OBJECT(ln::Object, obj)->setTypeInfoOverride(t);
        return LN_OK;
    }
    else {
        return LN_ERROR_UNKNOWN;
    }
    LNI_FUNC_TRY_END_RETURN;
}

//==============================================================================

//void LNLog_SetLevel(LNLogLevel level)
//{
//	ln::Logger::addStdErrAdapter();	// TODO: とりいそぎ
//    ln::Logger::setLevel(static_cast<ln::LogLevel>(level));
//}
//
//void LNLog_AllocConsole()
//{
//    ln::Console::allocate();
//}
//
//void LNLog_Write(LNLogLevel level, const LNChar* tag, const LNChar* text)
//{
//    LN_LOG(static_cast<ln::LogLevel>(level), ln::String(tag).toStdString().c_str()) << ln::String(text);
//}
//
//void LNLog_WriteA(LNLogLevel level, const char* tag, const char* text)
//{
//    LN_LOG(static_cast<ln::LogLevel>(level), tag) << ln::String::fromCString(text);
//}
//

LN_FLAT_API void LNLog_PrintA(LNLogLevel level, const char* tag, const char* format, ...);
void LNLog_PrintA(LNLogLevel level, const char* tag, const char* format, ...)
{
    if (::ln::detail::LoggerInterface::getInstance()->checkLevel(static_cast<ln::LogLevel>(level))) {
        std::va_list arg;
        va_start(arg, format);
        auto str = ln::detail::std_string_vprintf(format, arg);
        va_end(arg);
        LNLog_WriteA(level, tag, str.c_str());
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
LN_FLAT_API LNResult LNStringBuffer_GetUTF16String(LNHandle stringBuffer, const LNChar** outReturn);

/**
    @brief		文字列を UTF8 エンコーディングとして取得します。
    @param[in]	stringBuffer : StringBuffer
    @param[out]	outReturn    : string
*/
LN_FLAT_API LNResult LNStringBuffer_GetUTF8String(LNHandle stringBuffer, const char** outReturn);

LN_FLAT_API LNResult LNStringBuffer_GetUTF16String(LNHandle stringBuffer, const LNChar** outReturn)
{
    *outReturn = LNI_HANDLE_TO_OBJECT(ln::RuntimeStringBuffer, stringBuffer)->getUtf16();
}

LN_FLAT_API LNResult LNStringBuffer_GetUTF8String(LNHandle stringBuffer, const char** outReturn)
{
    *outReturn = LNI_HANDLE_TO_OBJECT(ln::RuntimeStringBuffer, stringBuffer)->getUtf8();
}
#endif

} // extern "C"