#pragma once
#include <ruby.h>
#include <string>
#include <vector>
#include <stack>
#include <LuminoEngine/Runtime/FlatCommon.h>

#if 1
/*
extern "C" int64_t LnRuntime_GetManagedObjectId(LnHandle handle);
typedef intptr_t LnHandle;
extern "C" void LnRuntime_SetManagedObjectId(LnHandle handle, int64_t id);
extern "C" void LnRuntime_UTF8ToNativeString(const char* src, std::u16string* dst);
extern "C" LnResult LnObject_Release(LnHandle obj);
extern "C" void InitLuminoRubyRuntimeManager();
 */

typedef VALUE(*ObjectFactoryFunc)(VALUE klass, LnHandle handle);

struct TypeInfo
{
    VALUE               klass;
    ObjectFactoryFunc   factory;
};

struct Wrap_Object
{
    LnHandle handle;
    Wrap_Object() : handle(0) {}
};

// 複数の RubyBinding(.so) をまたいで唯一のインスタンス。
// Lumino.RubyExt.generated.cpp とは別の so にするのがイメージしやすいが、
// そうすると require が面倒になるのでひとつの so にまとめている。
class LuminoRubyRuntimeManager
{
public:
    static const int InitialListSize = 1024;

    static LuminoRubyRuntimeManager* instance;
    void init();
    VALUE luminoModule() const { return m_luminoModule; }
    VALUE wrapObject(LnHandle handle);
    LnHandle getHandle(VALUE value) const;
    int registerTypeInfo(VALUE klass, ObjectFactoryFunc factory);
    void registerWrapperObject(VALUE obj);
    void unregisterWrapperObject(LnHandle handle);

    // for generator interface
    static LuminoRubyRuntimeManager* getInstance(VALUE managerInstance);
    static void gc_mark(LuminoRubyRuntimeManager* obj);
    //static std::u16string toNativeString(VALUE value);

private:
    VALUE m_luminoModule;
    std::vector<TypeInfo> m_typeInfoList;
    std::vector<VALUE> m_objectList;
    std::stack<int> m_objectListIndexStack;
};

#endif

