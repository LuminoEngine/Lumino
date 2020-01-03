#pragma once
#include <ruby.h>
#include <string>
#include <vector>
#include <stack>
#include "FlatCommon.h"

//#define LNRB_TRACE(...) printf(__VA_ARGS__)
#define LNRB_TRACE(...)

typedef VALUE(*ObjectFactoryFunc)(VALUE klass, LnHandle handle);

struct TypeInfo
{
    VALUE               klass;
    ObjectFactoryFunc   factory;
};

struct Wrap_RubyObject
{
    LnHandle handle;
    Wrap_RubyObject() : handle(0) {}
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
    VALUE wrapObjectForGetting(LnHandle handle, bool retain = true);
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
    VALUE m_objectClass;
    std::vector<TypeInfo> m_typeInfoList;
    std::vector<ObjectReferenceItem> m_objectList;
    std::stack<int> m_objectListIndexStack;
    bool m_runtimeAliving;
};

inline VALUE LNRB_HANDLE_WRAP_TO_VALUE(LnHandle handle) { return LuminoRubyRuntimeManager::instance->wrapObjectForGetting(handle); }
inline VALUE LNRB_HANDLE_WRAP_TO_VALUE(LnHandle handle, VALUE& accessorCache) { return LuminoRubyRuntimeManager::instance->wrapObjectForGetting(handle); }
inline VALUE LNRB_HANDLE_WRAP_TO_VALUE(LnHandle handle, std::vector<VALUE>& accessorCache, int index) { return LuminoRubyRuntimeManager::instance->wrapObjectForGetting(handle); }
inline void LNRB_SAFE_UNREGISTER_WRAPPER_OBJECT(LnHandle handle) { if ( LuminoRubyRuntimeManager::instance) LuminoRubyRuntimeManager::instance->unregisterWrapperObject(handle); }

inline VALUE LNRB_HANDLE_WRAP_TO_VALUE_NO_RETAIN(LnHandle handle) { return LuminoRubyRuntimeManager::instance->wrapObjectForGetting(handle, false); }


