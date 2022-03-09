#pragma once
#include "Common.hpp"

namespace ln {
class Exception;
class Object;

class FFI
{
public:
	static LNHandle makeObjectWrap(Object* obj, bool fromCreate);
	static Object* getObject(LNHandle handle);
	static void setManagedObjectId(LNHandle handle, int64_t id);
	static int64_t getManagedObjectId(LNHandle handle);
	static LNResult processException(Exception* e);
    //static const Char* getUTF16StringPtr(String str);
    static const Char* getUTF16StringPtr(const String& str);
    static const char* getAStringPtr(const String& str);
    static void setAStringEncoding(TextEncoding* value);
    static TextEncoding* getAStringEncoding();


private:
};

namespace detail {

// C++ 側が String 実体を返す時に、Binding 側のアクセサ(getter)として使用する
// TODO: 寿命の短いオブジェクトなのでキャッシュ組みたい
class RuntimeStringBuffer
    //: public Object
{
public:
    void reset(const String& str);
    const char* getAscii();
    const Char* getUtf16() { return m_str.c_str(); }

    void setAStringEncoding(TextEncoding* value) { m_astringEncoding = value; }
    TextEncoding* getAStringEncoding() const { return m_astringEncoding; }

//LN_CONSTRUCT_ACCESS:
    RuntimeStringBuffer();
//    void init(const String& str);

private:
    String m_str;
    Ref<TextEncoding> m_astringEncoding;
    //String m_str16;
    std::string m_strAscii;
    bool m_translated;
};

template<class T1, class T2>
inline T1 convertStructForced(const T2& v)
{
	return reinterpret_cast<const T1&>(v);
}

} // namespace detail
} // namespace ln

#include <LuminoEngine/Runtime/RuntimeLog.hpp>
