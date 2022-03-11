#pragma once
#include <ruby.h>
#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <unordered_map>
#include "FlatCommon.h"
#include "FlatC.generated.h"

extern "C" LN_FLAT_API void LNLog_PrintA(LNLogLevel level, const char* tag, const char* format, ...);
#define LNRB_LOG_D(...) LNLog_PrintA(LN_LOG_LEVEL_DEBUG, "RubyRuntime", __VA_ARGS__)
#define LNRB_LOG_I(...) LNLog_PrintA(LN_LOG_LEVEL_INFO, "RubyRuntime", __VA_ARGS__)

typedef VALUE(*ObjectFactoryFunc)(VALUE klass, LNHandle handle);

struct TypeInfo
{
    VALUE               klass;
    ObjectFactoryFunc   factory;
};

struct Wrap_RubyObject
{
    LNHandle handle;
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
    static LNLogLevel s_logLevel;

    void init();
    VALUE luminoModule() const { return m_luminoModule; }
    VALUE eventSignalClass() const { return m_eventSignalClass; }
    VALUE wrapObjectForGetting(LNHandle handle, bool retain = true);
    LNHandle getHandle(VALUE value) const;
    int registerTypeInfo(VALUE klass, ObjectFactoryFunc factory);
    void registerWrapperObject(VALUE obj, bool forNativeGetting);
    void unregisterWrapperObject(LNHandle handle);
    void gc_mark();

    // for generator interface
    static LuminoRubyRuntimeManager* getInstance(VALUE managerInstance);
    static void handleReferenceChangedStatic(LNHandle handle, int method, int count);
    void handleReferenceChanged(LNHandle handle, int method, int count);

    static void handleRuntimeFinalized();
    static void handleCreateInstanceCallback(int typeInfoId, LNHandle* outHandle);

    void dumpInfo() const;

    // TODO: internal
    std::unordered_map<std::string, int> m_userDefinedClassTypeIdMap;

public: // TODO:
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

class RubyUtils
{
public:
    static std::string getClassNameFromClass(VALUE klass);
    static std::string getClassNameFromObject(VALUE klass);
    static std::string makeTypeInfoName(VALUE klass);
};

inline VALUE LNRB_HANDLE_WRAP_TO_VALUE(LNHandle handle) { return LuminoRubyRuntimeManager::instance->wrapObjectForGetting(handle); }
inline VALUE LNRB_HANDLE_WRAP_TO_VALUE(LNHandle handle, VALUE& accessorCache) { return LuminoRubyRuntimeManager::instance->wrapObjectForGetting(handle); }
inline VALUE LNRB_HANDLE_WRAP_TO_VALUE(LNHandle handle, std::vector<VALUE>& accessorCache, int index) { return LuminoRubyRuntimeManager::instance->wrapObjectForGetting(handle); }
inline void LNRB_SAFE_UNREGISTER_WRAPPER_OBJECT(LNHandle handle) { if ( LuminoRubyRuntimeManager::instance) LuminoRubyRuntimeManager::instance->unregisterWrapperObject(handle); }

inline VALUE LNRB_HANDLE_WRAP_TO_VALUE_NO_RETAIN(LNHandle handle) { return LuminoRubyRuntimeManager::instance->wrapObjectForGetting(handle, false); }


