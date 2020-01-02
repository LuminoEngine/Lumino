#pragma once
#include "Common.hpp"

namespace ln {
class Exception;
class Object;

class Runtime
{
public:
	static LnHandle makeObjectWrap(Object* obj, bool fromCreate);
	static Object* getObject(LnHandle handle);
	static void setManagedObjectId(LnHandle handle, int64_t id);
	static int64_t getManagedObjectId(LnHandle handle);
	static LnResult processException(Exception* e);
    static const Char* getUTF16StringPtr(const String& str);
    static const char* getUTF8StringPtr(const String& str);

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
    const char* getUtf8();
    const Char* getUtf16() { return m_str.c_str(); }

//LN_CONSTRUCT_ACCESS:
    RuntimeStringBuffer();
//    void init(const String& str);

private:
    String m_str;
    std::string m_str8;
    bool m_translated;
};

} // namespace detail
} // namespace ln
