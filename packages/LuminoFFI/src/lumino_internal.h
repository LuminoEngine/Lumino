#include <stdio.h>
#include <LuminoEngine.hpp>
#include <lumino.h>
#include <lumino_types.h>
using namespace ln;

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#define LN_FFI_TRY_BEGIN try {

#define LN_FFI_TRY_END_RETURN             \
    }                                     \
    catch (Exception & e) {               \
        return FFI::processException(&e); \
    }                                     \
    return LN_OK;

#define LN_FFI_TRY_END             \
    }                              \
    catch (Exception & e) {        \
        FFI::processException(&e); \
    }

#define LN_HANDLE_TO_OBJECT(type, h) static_cast<type*>((h) ? ::Runtime::getObject(h) : nullptr)
#define LN_RELEASE_OBJECT(h) LNObject_Release(reinterpret_cast<LNHandle>(h))

#define TO_FFI_ERROR(result) LN_ERROR_UNKNOWN // TODO:

#define TRY_FFI_RESULT(expr)             \
    {                                    \
        auto result = (expr);            \
        if (!result) {                   \
            return TO_FFI_ERROR(result); \
        }                                \
    }

// #define LN_DEFINE_HANDLE(object) typedef struct object##_T* object
// NOTE: ↑こういうタイプセーフなハンドル定義は行わない。
//   - 継承されたクラスを使いづらくなるため。
//     例えば LNTexture2D を LNTextre として LNMaterial にセットしたいときなど。
//     キャストして使ってもよいが、そうすると LNHandle のまま使うのと大差なくなるかも。

namespace ln {

class FFI {
public:
    static LNResult processException(Exception* e);

private:
};

/**
 * Provides operations and utilities for the entire Runtime module.
 */
class Runtime {
public:
    static LNHandle wrapObject(Object* obj, bool fromCreate);
    static Object* getObject(LNHandle handle);
    //static void setManagedObjectId(LNHandle handle, int64_t id);
    //static int64_t getManagedObjectId(LNHandle handle);
    //static const Char* getUTF16StringPtr(const String& str);
    //static const char* getAStringPtr(const String& str);
    //static void setAStringEncoding(TextEncoding* value);
    //static TextEncoding* getAStringEncoding();
};

} // namespace ln

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus
