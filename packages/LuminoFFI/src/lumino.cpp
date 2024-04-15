#include <stdio.h>
#include <LuminoEngine/RuntimeModule.hpp>
#include <LuminoEngine/RHIModule.hpp>
#include <LuminoEngine/GPU/OpenGLGraphicsContext.hpp>
#include <LuminoEngine/Runtime/detail/RuntimeManager.hpp>
#include <lumino.h>

namespace ln {

class FFI {
public:
    static LNResult processException(Exception* e);
    static LNHandle wrapObject(Object* obj, bool fromCreate);
    static Object* getObject(LNHandle handle);
    //static void setManagedObjectId(LNHandle handle, int64_t id);
    //static int64_t getManagedObjectId(LNHandle handle);
    //static const Char* getUTF16StringPtr(const String& str);
    //static const char* getAStringPtr(const String& str);
    //static void setAStringEncoding(TextEncoding* value);
    //static TextEncoding* getAStringEncoding();

private:
};

LNResult FFI::processException(Exception* e) {
    return LN_ERROR_UNKNOWN;
}

LNHandle FFI::wrapObject(Object* obj, bool fromCreate) {
    return detail::RuntimeManager::instance()->makeObjectWrap(obj, fromCreate);
}

Object* FFI::getObject(LNHandle handle) {
    return detail::RuntimeManager::instance()->getObjectEntry(handle)->object;
}

} // namespace ln







#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#define LN_FFI_TRY_BEGIN try {

#define LN_FFI_TRY_END_RETURN               \
    }                                         \
    catch (ln::Exception & e) {               \
        return ln::FFI::processException(&e); \
    }                                         \
    return LN_OK;

#define LNI_HANDLE_TO_OBJECT(type, h) static_cast<type*>((h) ? ::ln::FFI::getObject(h) : nullptr)

LUMINO_API LNResult LNRuntime_Initialize() {
    ln::RuntimeModule::initialize();
    ln::GraphicsModule::initialize({ ln::GraphicsAPI::OpenGL });
    return LN_OK;
}

LUMINO_API void LNRuntime_Terminate() {
    ln::GraphicsModule::terminate();
    ln::RuntimeModule::terminate();
}

LUMINO_API LNResult LNGraphicsContext_CreateFromOpenGL(LNHandle* outHandle) {
    LN_FFI_TRY_BEGIN;
    ln::OpenGLGraphicsContext::Settings s;
    s.window = nullptr;
    *outHandle = ::ln::FFI::wrapObject(ln::OpenGLGraphicsContext::create(s), true);
    LN_FFI_TRY_END_RETURN;
}

//LUMINO_API LNResult LNGraphicsContext_Release(LNHandle handle) {
//    LN_SAFE_RELEASE(handle)
//
//}




//==============================================================================
// LNObject

LUMINO_API LNResult LNObject_Release(LNHandle obj) {
    if (auto m = ln::detail::RuntimeManager::instance()) {
        m->releaseObjectExplicitly(obj);
        return LN_OK;
    }
    else {
        return LN_RUNTIME_UNINITIALIZED;
    }
}

LUMINO_API LNResult LNObject_Retain(LNHandle obj) {
    if (auto m = ln::detail::RuntimeManager::instance()) {
        m->retainObjectExplicitly(obj);
        return LN_OK;
    }
    else {
        return LN_RUNTIME_UNINITIALIZED;
    }
}

LUMINO_API LNResult LNObject_GetReferenceCount(LNHandle obj, int32_t* outReturn) {
    if (!outReturn) return LN_ERROR_INVALID_ARGUMENT;

    if (auto t = LNI_HANDLE_TO_OBJECT(ln::Object, obj)) {
        *outReturn = ln::RefObjectHelper::getReferenceCount(t);
        return LN_OK;
    }
    else {
        return LN_ERROR_INVALID_ARGUMENT;
    }
}



#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus
