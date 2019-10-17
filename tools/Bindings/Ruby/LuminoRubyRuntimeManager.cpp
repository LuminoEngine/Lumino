#if 1
#include "LuminoRubyRuntimeManager.h"

LuminoRubyRuntimeManager* LuminoRubyRuntimeManager::getInstance(VALUE managerInstance)
{
    LuminoRubyRuntimeManager* instance;
    Data_Get_Struct(managerInstance, LuminoRubyRuntimeManager, instance);
    return instance;
}

void LuminoRubyRuntimeManager::gc_mark(LuminoRubyRuntimeManager* obj)
{
    rb_gc_mark(obj->m_luminoModule);
}

/*
std::u16string LuminoRubyRuntimeManager::toNativeString(VALUE value)
{
    std::u16string str; 
    LnRuntime_UTF8ToNativeString(StringValuePtr(value), &strcat);
    return str;
}
*/

LuminoRubyRuntimeManager* LuminoRubyRuntimeManager::instance = nullptr;

void LuminoRubyRuntimeManager::init()
{
    instance = this;
    m_luminoModule = rb_define_module("Lumino");
    m_typeInfoList.push_back({});   // [0] is dummy

    for (int i = 0; i < InitialListSize; i++) {
        m_objectList.push_back(Qnil);
        m_objectListIndexStack.push(i);
    }
}

VALUE LuminoRubyRuntimeManager::wrapObject(LnHandle handle)
{
    int objectIndex = (int)LnRuntime_GetManagedObjectId(handle);
    if (objectIndex <= 0) {
        int typeinfoIndex = (int)LnRuntime_GetManagedTypeInfoId(handle);

        VALUE obj = m_typeInfoList[typeinfoIndex].factory(m_typeInfoList[typeinfoIndex].klass, handle);
        registerWrapperObject(obj);
        return obj;
    }
    else {
        return m_objectList[objectIndex];
    }
    return Qnil;    // TODO:
}

LnHandle LuminoRubyRuntimeManager::getHandle(VALUE value) const
{
	if (value == Qnil) {
		return LN_NULL_HANDLE;
	}
	Wrap_Object* obj;
	Data_Get_Struct(value, Wrap_Object, obj);
	return obj->handle;
}

int LuminoRubyRuntimeManager::registerTypeInfo(VALUE klass, ObjectFactoryFunc factory)
{
    TypeInfo t;
    t.klass = klass;
    t.factory = factory;
    m_typeInfoList.push_back(t);
    return m_typeInfoList.size() - 1;
}

void LuminoRubyRuntimeManager::registerWrapperObject(VALUE obj)
{
    // grow
	if (m_objectListIndexStack.size() == 0)
	{
		int growCount = m_objectList.size();
		for (int i = 0; i < growCount; i++)
		{
			m_objectList.push_back(Qnil);
			m_objectListIndexStack.push(growCount + i);
		}
	}

	int index = m_objectListIndexStack.top();
	m_objectListIndexStack.pop();
	m_objectList[index] = obj;
	LnRuntime_SetManagedObjectId(getHandle(obj), index);
}

void LuminoRubyRuntimeManager::unregisterWrapperObject(LnHandle handle)
{
    LnObject_Release(handle);
	int index = (int)LnRuntime_GetManagedObjectId(handle);
	m_objectList[index] = Qnil;
	m_objectListIndexStack.push(index);
}

static VALUE g_LuminoRubyRuntimeManagerClass;
static VALUE g_LuminoRubyRuntimeManager;

static void LuminoRubyRuntimeManager_delete(LuminoRubyRuntimeManager* obj)
{
    if (obj) {
        delete obj;
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

extern "C" void InitLuminoRubyRuntimeManager()
{
    VALUE manager = rb_eval_string("$lumino_ruby_runtime_manager");
    if (manager == Qnil) {
        // Create new manager.
        g_LuminoRubyRuntimeManagerClass = rb_define_class("LuminoRubyRuntimeManager", rb_cObject);
        g_LuminoRubyRuntimeManager = LuminoRubyRuntimeManager_allocate(g_LuminoRubyRuntimeManagerClass);
        rb_define_readonly_variable("$lumino_ruby_runtime_manager", &g_LuminoRubyRuntimeManager);
    }
    else {
        // Use defined manager.
        LuminoRubyRuntimeManager::instance = LuminoRubyRuntimeManager::getInstance(manager);
    }
}

#endif
