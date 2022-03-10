#pragma once
#include "Common.hpp"

namespace ln {
class Exception;
class Object;

class FFI
{
public:
    static LNResult processException(Exception* e);
	static LNHandle makeObjectWrap(Object* obj, bool fromCreate);
	static Object* getObject(LNHandle handle);
	static void setManagedObjectId(LNHandle handle, int64_t id);
	static int64_t getManagedObjectId(LNHandle handle);
    static const Char* getUTF16StringPtr(const String& str);
    static const char* getAStringPtr(const String& str);
    static void setAStringEncoding(TextEncoding* value);
    static TextEncoding* getAStringEncoding();


private:
};

namespace detail {

template<class T1, class T2>
inline T1 convertStructForced(const T2& v)
{
	return reinterpret_cast<const T1&>(v);
}

} // namespace detail
} // namespace ln

#include <LuminoEngine/Runtime/RuntimeLog.hpp>
