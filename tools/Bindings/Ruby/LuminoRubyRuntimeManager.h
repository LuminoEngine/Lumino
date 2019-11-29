#pragma once
#include <ruby.h>
#include <string>
#include <vector>
#include <stack>
#include <LuminoEngine/Runtime/FlatCommon.h>

//#define LNRB_TRACE(...) printf(__VA_ARGS__)
#define LNRB_TRACE(...)

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
    static const int InitialListSize = 512;

    static LuminoRubyRuntimeManager* instance;
    void init();
    VALUE luminoModule() const { return m_luminoModule; }
    VALUE eventSignalClass() const { return m_eventSignalClass; }
    VALUE wrapObjectForGetting(LnHandle handle);
    LnHandle getHandle(VALUE value) const;
    int registerTypeInfo(VALUE klass, ObjectFactoryFunc factory);
    void registerWrapperObject(VALUE obj, bool forNativeGetting);
    void unregisterWrapperObject(LnHandle handle);

    // for generator interface
    static LuminoRubyRuntimeManager* getInstance(VALUE managerInstance);
    static void gc_mark(LuminoRubyRuntimeManager* obj);
    static void handleReferenceChangedStatic(LnHandle handle, int method, int count);
    void handleReferenceChanged(LnHandle handle, int method, int count);

    static void handleRuntimeFinalized();

private:
    struct ObjectReferenceItem
    {
        VALUE weakRef;
        VALUE strongRef;
    };

    VALUE m_luminoModule;
    VALUE m_eventSignalClass;
    std::vector<TypeInfo> m_typeInfoList;
    std::vector<ObjectReferenceItem> m_objectList;
    std::stack<int> m_objectListIndexStack;
    bool m_runtimeAliving;
};

inline VALUE LNRB_HANDLE_WRAP_TO_VALUE(LnHandle handle) { return LuminoRubyRuntimeManager::instance->wrapObjectForGetting(handle); }
inline VALUE LNRB_HANDLE_WRAP_TO_VALUE(LnHandle handle, VALUE& accessorCache) { return LuminoRubyRuntimeManager::instance->wrapObjectForGetting(handle); }
inline VALUE LNRB_HANDLE_WRAP_TO_VALUE(LnHandle handle, std::vector<VALUE>& accessorCache, int index) { return LuminoRubyRuntimeManager::instance->wrapObjectForGetting(handle); }
inline void LNRB_SAFE_UNREGISTER_WRAPPER_OBJECT(LnHandle handle) { if ( LuminoRubyRuntimeManager::instance) LuminoRubyRuntimeManager::instance->unregisterWrapperObject(handle); }


#endif

