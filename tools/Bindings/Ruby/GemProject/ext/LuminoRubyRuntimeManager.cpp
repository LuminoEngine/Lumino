#if 1
#include "LuminoRubyRuntimeManager.h"

extern "C" LN_FLAT_API LnResult LnEngine_Finalize();
extern "C" LN_FLAT_API void LnRuntime_RunAppInternal(LnHandle app);

//==============================================================================
// 

static VALUE Wrap_LnRuntime_RunAppInternal(VALUE self, VALUE app)
{
    LnHandle handle = LuminoRubyRuntimeManager::instance->getHandle(app);
    LnRuntime_RunAppInternal(handle);
    return Qnil;
}

VALUE Wrap_LnRuntime_RegisterType(VALUE self, VALUE type)
{
    std::string className;
    {
        VALUE v = rb_funcall(type, rb_intern("name"), 0, 0);
        className = StringValuePtr(v);
    }
    
    std::string superclassName;
    {
        VALUE v1  = rb_funcall(type, rb_intern("superclass"), 0, 0);
        superclassName = RubyUtils::makeTypeInfoName(v1);
    }

    LNRB_LOG_D("Usesr type registering. (class: %s, superclass: %s)", className.c_str(), superclassName.c_str());

    int typeInfoId;
    LnTypeInfo_AcquireA(className.c_str(), &typeInfoId);

    int baseTypeInfoId;
    LnTypeInfo_AcquireA(superclassName.c_str(), &baseTypeInfoId);

    LnTypeInfo_SetBaseClass(typeInfoId, baseTypeInfoId);
    LnTypeInfo_SetCreateInstanceCallback(typeInfoId, &LuminoRubyRuntimeManager::handleCreateInstanceCallback);

    // ユーザー定義型として、あとでインスタンス生成時に型情報を関連付けるときに高速に検索できるように覚えておく
    LuminoRubyRuntimeManager::instance->m_userDefinedClassTypeIdMap[className] = typeInfoId;

    int managedTypeInfoId = LuminoRubyRuntimeManager::instance->registerTypeInfo(type, nullptr);
    LnTypeInfo_SetManagedTypeInfoId(typeInfoId, managedTypeInfoId);
    
    LNRB_LOG_D("Usesr type registerd. (typeInfoId: %d, baseTypeInfoId: %s, managedTypeInfoId: %d)", typeInfoId, baseTypeInfoId, managedTypeInfoId);
    return Qnil;
}

static VALUE Wrap_LnRuntime_inherited(VALUE self, VALUE type)
{
    VALUE v  = rb_funcall(type, rb_intern("name"), 0, 0);
    std::string name2 = StringValuePtr(v);
    if (name2.find("Lumino::") == 0) return Qnil;   // TODO: Engine::init 前は EngineContext が null なので使えない。ので逃げる

    auto name = RubyUtils::makeTypeInfoName(type);
    LNRB_LOG_D("Usesr type registering from self.inherited. (%s)", name.c_str());

    int typeInfoId = 0;
    LnTypeInfo_FindA(name.c_str(), &typeInfoId);

    if (typeInfoId == 0) {
        Wrap_LnRuntime_RegisterType(self, type);
    }

    return Qnil;
}

//==============================================================================
// LuminoRubyRuntimeManager

LuminoRubyRuntimeManager* LuminoRubyRuntimeManager::instance = nullptr;
LnLogLevel LuminoRubyRuntimeManager::s_logLevel = LN_LOG_LEVEL_INFO;

LuminoRubyRuntimeManager* LuminoRubyRuntimeManager::getInstance(VALUE managerInstance)
{
    LuminoRubyRuntimeManager* instance;
    Data_Get_Struct(managerInstance, LuminoRubyRuntimeManager, instance);
    return instance;
}

void LuminoRubyRuntimeManager::init()
{
    instance = this;

    // Initialize runtime.
    {
        VALUE LUMINO_LOG_LEVEL = rb_eval_string_protect("$LUMINO_LOG_LEVEL", NULL);
        if (LUMINO_LOG_LEVEL != Qnil) {
            s_logLevel = (LnLogLevel)FIX2INT(LUMINO_LOG_LEVEL);
            LnLog_SetLevel(s_logLevel);
        }

        LnRuntimeSettings settings;
        settings.runtimeFinalizedCallback = handleRuntimeFinalized;
        settings.referenceCountTrackerCallback = handleReferenceChangedStatic;
        settings.runtimeGetTypeInfoIdCallback = nullptr;

        LnRuntime_Initialize(&settings);
    }

    // Define core functions.
    {
        m_luminoModule = rb_define_module("Lumino");
        
        int state = 0;
        rb_eval_string_protect(
            "module Lumino\n"
            "  class EventSignal\n"
            "    def initialize\n"
            "      @handlers = []\n"
            "    end\n"
            "    def add(handler=nil, &block)\n"
            "      if handler\n"
            "        @handlers.push(handler)\n"
            "      end\n"
            "      if block\n"
            "        @handlers.push(block)\n"
            "      end\n"
            "    end\n"
            "    def raise(*args)\n"
            "      @handlers.each do |h|\n"
            "        h.call(*args)\n"
            "      end\n"
            "    end\n"
            "  end\n"
            "end\n"
            ,
            &state);
        m_eventSignalClass = rb_eval_string("Lumino::EventSignal");

        m_objectClass = rb_define_class_under(m_luminoModule, "Object", rb_cObject);
        rb_define_singleton_method(m_objectClass, "inherited", reinterpret_cast<VALUE(__cdecl *)(...)>(Wrap_LnRuntime_inherited), 1);

        VALUE class_Application = rb_define_class_under(m_luminoModule, "Application", m_objectClass);
        rb_define_singleton_method(class_Application, "run_app_internal", reinterpret_cast<VALUE(__cdecl *)(...)>(Wrap_LnRuntime_RunAppInternal), 1);
        
        rb_define_module_function(m_luminoModule, "register_type", reinterpret_cast<VALUE(__cdecl *)(...)>(Wrap_LnRuntime_RegisterType), 1);
    }

    m_typeInfoList.push_back({});   // [0] is dummy

    for (int i = 0; i < InitialListSize; i++) {
        m_objectList.push_back({ Qnil, Qnil });
        m_objectListIndexStack.push(i);
    }

    m_runtimeAliving = true;

}

VALUE LuminoRubyRuntimeManager::wrapObjectForGetting(LnHandle handle, bool retain)
{
    int objectIndex = (int)LnRuntime_GetManagedObjectId(handle);
    int typeinfoIndex = (int)LnRuntime_GetManagedTypeInfoId(handle);

    if (objectIndex <= 0) {
        VALUE obj = m_typeInfoList[typeinfoIndex].factory(m_typeInfoList[typeinfoIndex].klass, handle);
        if (retain) {
            registerWrapperObject(obj, true);
        }
        LnObject_Retain(handle);
        return obj;
    }
    else {
        return m_objectList[objectIndex].weakRef;
    }
}

LnHandle LuminoRubyRuntimeManager::getHandle(VALUE value) const
{
	if (value == Qnil) {
		return LN_NULL_HANDLE;
	}
	Wrap_RubyObject* obj;
	Data_Get_Struct(value, Wrap_RubyObject, obj);
	return obj->handle;
}

int LuminoRubyRuntimeManager::registerTypeInfo(VALUE klass, ObjectFactoryFunc factory)
{
    TypeInfo t;
    t.klass = klass;
    t.factory = factory;
    m_typeInfoList.push_back(t);
    int id =  m_typeInfoList.size() - 1;

    if (s_logLevel == LN_LOG_LEVEL_DEBUG) {
        LNRB_LOG_D("Type registerd. (id: %d, displayName: %s)", id, RubyUtils::getClassName(klass).c_str());
    }

    return id;
}

void LuminoRubyRuntimeManager::registerWrapperObject(VALUE obj, bool forNativeGetting)
{
    LnHandle handle = getHandle(obj);

    int64_t id = LnRuntime_GetManagedObjectId(handle);
    if (id > 0) {
        // 登録済み
        LNRB_LOG_D("registerWrapperObject: [Registerd] ManagedObjectId: %d\n", id);
        return;
    }

    // grow
	if (m_objectListIndexStack.size() == 0)
	{
		int growCount = m_objectList.size();
		for (int i = 0; i < growCount; i++)
		{
			m_objectList.push_back({ Qnil, Qnil });
			m_objectListIndexStack.push(growCount + i);
		}
	}

	int index = m_objectListIndexStack.top();
	m_objectListIndexStack.pop();
	m_objectList[index].weakRef = obj;
    if (forNativeGetting) {
        m_objectList[index].strongRef = obj;
    }
    
	LnRuntime_SetManagedObjectId(handle, index);
    LNRB_LOG_D("registerWrapperObject (handle:%u, ManagedObjectId:%d)\n", handle, index);

    // ユーザー定義型の型情報をオブジェクトにセットする
    {
        VALUE klass = CLASS_OF(obj);
        VALUE v = rb_funcall(klass, rb_intern("name"), 0, 0);
        std::string name = StringValuePtr(v);

        // ユーザー定義型に限定する (Engine 内部型は登録する必要なし)
        auto itr = m_userDefinedClassTypeIdMap.find(name);
        if (itr != m_userDefinedClassTypeIdMap.end()) {
            LnObject_SetTypeInfoId(handle, itr->second);
            LNRB_LOG_D("LnObject_SetTypeInfoId (handle:%u, itr->second:%d)\n", handle, itr->second);
        }
    }

}

void LuminoRubyRuntimeManager::unregisterWrapperObject(LnHandle handle)
{
    LNRB_LOG_D("LuminoRubyRuntimeManager::unregisterWrapperObject: LnHandle=%u\n", handle);
    if (m_runtimeAliving) {
        LnObject_Release(handle);
        int index = (int)LnRuntime_GetManagedObjectId(handle);
        m_objectList[index].weakRef = Qnil;
        m_objectList[index].strongRef = Qnil;
        m_objectListIndexStack.push(index);
    }
}

static VALUE g_LuminoRubyRuntimeManagerClass;
static VALUE g_LuminoRubyRuntimeManager;

static void LuminoRubyRuntimeManager_delete(LuminoRubyRuntimeManager* obj)
{
    if (obj) {
        LnEngine_Finalize();
        delete obj;
        LuminoRubyRuntimeManager::instance = nullptr;
    }
}

static VALUE LuminoRubyRuntimeManager_allocate(VALUE klass)
{
    VALUE obj;
    LuminoRubyRuntimeManager* internalObj = new LuminoRubyRuntimeManager();
    if (!internalObj) rb_raise(rb_eRuntimeError, "Faild alloc - LuminoRubyRuntimeManager_allocate");
    internalObj->init();
    obj = Data_Wrap_Struct(klass, LuminoRubyRuntimeManager::gc_mark, LuminoRubyRuntimeManager_delete, internalObj);
    return obj;
}

void LuminoRubyRuntimeManager::gc_mark(LuminoRubyRuntimeManager* obj)
{
    LNRB_LOG_D("LuminoRubyRuntimeManager::gc_mark\n");
    rb_gc_mark(obj->m_luminoModule);
    rb_gc_mark(obj->m_eventSignalClass);
    for (auto& item : instance->m_objectList) {
        rb_gc_mark(item.strongRef);
    }
}

void LuminoRubyRuntimeManager::handleReferenceChangedStatic(LnHandle handle, int method, int count)
{
    instance->handleReferenceChanged(handle, method, count);
}

void LuminoRubyRuntimeManager::handleReferenceChanged(LnHandle handle, int method, int count)
{
    int objectIndex = (int)LnRuntime_GetManagedObjectId(handle);

    if (method == LNI_REFERENCE_RETAINED) {
        if (count >= 2) {
            m_objectList[objectIndex].strongRef = m_objectList[objectIndex].weakRef;
        }
    }
    else if (method == LNI_REFERENCE_RELEASED) {
        if (count <= 1) {
            m_objectList[objectIndex].strongRef = Qnil;
        }
    }
}

void LuminoRubyRuntimeManager::handleRuntimeFinalized()
{
    if (instance) {
        instance->m_runtimeAliving = false;
    }
}

void LuminoRubyRuntimeManager::handleCreateInstanceCallback(int typeInfoId, LnHandle* outHandle)
{
    LNRB_LOG_D("start: handleCreateInstanceCallback");
    auto* manager = LuminoRubyRuntimeManager::instance;

    int managedTypeInfoId = -1;
    LnTypeInfo_GetManagedTypeInfoId(typeInfoId, &managedTypeInfoId);

    
    printf("managedTypeInfoId: %d\n", managedTypeInfoId);

    const auto& typeInfo = manager->m_typeInfoList[managedTypeInfoId];
    printf("typeInfo.klass: %llu\n", typeInfo.klass);
    VALUE obj = rb_funcall(typeInfo.klass, rb_intern("new"), 0, 0);

    printf("obj: %llu\n", obj);
    manager->registerWrapperObject(obj, false);


    *outHandle = manager->getHandle(obj);
    printf("outHandle: %d\n", *outHandle);
    printf("e handleCreateInstanceCallback\n");

    
    printf("!!!! new Object. Handle:%d (%s)\n", *outHandle, RubyUtils::makeTypeInfoName(CLASS_OF(obj)).c_str());

    LNRB_LOG_D("end: handleCreateInstanceCallback");
}

extern "C" void InitLuminoRubyRuntimeManager()
{
    VALUE manager = rb_eval_string_protect("defined? $lumino_ruby_runtime_manager", NULL);
    if (manager == Qnil) {
        // Create new LuminoRubyRuntimeManager.
        g_LuminoRubyRuntimeManagerClass = rb_define_class("LuminoRubyRuntimeManager", rb_cObject);
        g_LuminoRubyRuntimeManager = LuminoRubyRuntimeManager_allocate(g_LuminoRubyRuntimeManagerClass);
        rb_define_readonly_variable("$lumino_ruby_runtime_manager", &g_LuminoRubyRuntimeManager);
    }
    else {
        // Use defined LuminoRubyRuntimeManager.
        LuminoRubyRuntimeManager::instance = LuminoRubyRuntimeManager::getInstance(manager);
    }
}

#endif



//==============================================================================
// RubyUtils

std::string RubyUtils::getClassName(VALUE klass)
{
    VALUE v = rb_funcall(klass, rb_intern("name"), 0, 0);
    return std::string(StringValuePtr(v));
}

std::string RubyUtils::makeTypeInfoName(VALUE klass)
{
    VALUE v = rb_funcall(klass, rb_intern("name"), 0, 0);
    //printf("superclassName: %s\n", StringValuePtr(v2));
    std::string name = StringValuePtr(v);

    // TODO: C++ 側が、LN_OBJECT_IMPLEMENT(Sprite) とか描いただけじゃ FullName で登録されない。
    // 対策方法が見つかるまでとりあえず頭消しておく。
    if (name.find("Lumino::") == 0) {
        name = name.substr(8);
    }

    return name;
}

