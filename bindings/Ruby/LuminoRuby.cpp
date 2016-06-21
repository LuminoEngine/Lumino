#include "LuminoRuby.h"

VALUE g_luminoModule;
VALUE g_luminoError;
VALUE g_class_RefObject;
VALUE g_class_ObjectList;

//==============================================================================
// Utils
//==============================================================================

bool checkEqualHandle(VALUE obj, LNHandle handle)
{
	if (obj == Qnil)
		return handle == NULL;
	if (((wrapRefObject*)DATA_PTR(obj))->Handle != handle)
		return false;
	return true;
}

//==============================================================================
// Manager
//==============================================================================

std::vector<TypeInfo>	Manager::m_typeInfoList;
std::vector<VALUE>		Manager::m_objectList;
std::stack<int>			Manager::m_objectListIndexStack;

//------------------------------------------------------------------------------
void Manager::Initialize()
{
	// 管理リストの空き番号を詰める
	for (int i = 0; i < InitialListSize; i++)
	{
		m_objectList.push_back(Qnil);
		m_objectListIndexStack.push(i);
	}
	
	// Ruby 系の初期化
	InitEnums();
	InitStructs();
	InitClasses();
	
	// 型情報の登録
	m_typeInfoList.push_back(TypeInfo());	// ダミーを1つ詰めておく
	RegisterTypeInfo();
}

//------------------------------------------------------------------------------
void Manager::Finalize()
{
	LNEngine_Terminate();
}

//------------------------------------------------------------------------------
VALUE Manager::GetWrapperObjectFromHandle(LNHandle handle)
{
	int objectIndex = (int)LNObject_GetUserData(handle);
	if (objectIndex == 0)
	{
		int typeinfoIndex = (int)LNObject_GetBindingTypeData(handle);
		VALUE obj = m_typeInfoList[typeinfoIndex].factory(m_typeInfoList[typeinfoIndex].klass, handle);
		RegisterWrapperObject(obj);
		return obj;
	}
	else
	{
		return m_objectList[objectIndex];
	}
}

//------------------------------------------------------------------------------
LNHandle Manager::GetHandleFromtWrapperObject(VALUE v)
{
	if (v == Qnil) {
		return NULL;    
	}
	wrapRefObject* obj;
	Data_Get_Struct(v, wrapRefObject, obj);
	return obj->Handle;
}

//------------------------------------------------------------------------------
void Manager::RegisterWrapperObject(VALUE obj)
{
	// 管理リストが一杯の時は拡張する
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
	LNObject_SetUserData(GetHandleFromtWrapperObject(obj), (void*)index);
}

//------------------------------------------------------------------------------
void Manager::UnregisterWrapperObject(LNHandle handle)
{
	int index = (int)LNObject_GetUserData(handle);
	m_objectList[index] = Qnil;
	m_objectListIndexStack.push(index);
}

//------------------------------------------------------------------------------
const char* LNGetLastErrorMessage()
{
	const LNChar* str;
	LNError_GetLastErrorMessage(&str);
	return LNInternal_ConvertToUTF8String(str, -1);
}

//------------------------------------------------------------------------------
static VALUE Lumino_terminate(VALUE obj)
{
	Manager::Finalize();
}


//==============================================================================
// LNObjectList

struct wrapObjectList
    : public wrapRefObject
{
    std::vector<VALUE> m_cacheList;
};

static void lnrbLNObjectList_delete(wrapObjectList* obj)
{
    if (obj->Handle != 0) LNObject_Release(obj->Handle);
    Manager::UnregisterWrapperObject(obj->Handle);
    delete obj;
}

static void lnrbLNObjectList_mark(wrapObjectList* obj) 
{
	// 配列に保持しているオブジェクトを mark する
	std::vector<VALUE>::iterator itr = obj->m_cacheList.begin();
	std::vector<VALUE>::iterator end = obj->m_cacheList.end();
	for (; itr != end; ++itr) {
		rb_gc_mark(*itr);
	}
}

static VALUE lnrbLNObjectList_allocate( VALUE klass )
{
    VALUE obj;
    wrapObjectList* internalObj;

    internalObj = new wrapObjectList();
    if (internalObj == NULL) rb_raise( g_luminoModule, "Faild alloc - LNObjectList_allocate" );
    obj = Data_Wrap_Struct(klass, lnrbLNObjectList_mark, lnrbLNObjectList_delete, internalObj);

    return obj;
}

static VALUE lnrbLNObjectList_allocateForGetRefObject(VALUE klass, LNHandle handle)
{
    VALUE obj;
    wrapObjectList* internalObj;

    internalObj = new wrapObjectList();
    if (internalObj == NULL) rb_raise( g_luminoModule, "Faild alloc - LNObjectList_allocateForGetRefObject" );
    obj = Data_Wrap_Struct(klass, lnrbLNObjectList_mark, lnrbLNObjectList_delete, internalObj);
    
    internalObj->Handle = handle;
    return obj;
}

static void lnrbLNObjectList_SyncItems(wrapObjectList* selfWrap) 
{
	std::vector<VALUE>& list = selfWrap->m_cacheList;
	int count;
	LNObjectList_GetCount(selfWrap->Handle, &count);

	if (((int)list.size()) < count) 
	{
		// 足りない分を詰める
		list.resize(count);

		// リスト内容を同期する
		for (int i = 0; i < count; ++i)
		{
			LNHandle item;
			LNObjectList_GetAt(selfWrap->Handle, i, &item);
			if (list[i] == Qnil || (!checkEqualHandle(list[i], item)))
			{
				list[i] = Manager::GetWrapperObjectFromHandle(item);
			}
		}
	}
}

static VALUE lnrbLNObjectList_GetCount(VALUE self) 
{
    int count;
    LNObjectList_GetCount(Manager::GetHandleFromtWrapperObject(self), &count);
	return INT2FIX(count);
}

static VALUE lnrbLNObjectList_GetAt(VALUE self, VALUE index) 
{
    wrapObjectList* selfWrap;
    Data_Get_Struct(self, wrapObjectList, selfWrap);
    lnrbLNObjectList_SyncItems(selfWrap);

    int idx = INT2FIX(index);
    return selfWrap->m_cacheList[idx];
}

static VALUE lnrbLNObjectList_SetAt(VALUE self, VALUE index, VALUE item) 
{
    wrapObjectList* selfWrap;
    Data_Get_Struct(self, wrapObjectList, selfWrap);
    lnrbLNObjectList_SyncItems(selfWrap);
	
    int idx = INT2FIX(index);
    LNObjectList_SetAt(Manager::GetHandleFromtWrapperObject(self), idx, Manager::GetHandleFromtWrapperObject(item));
	selfWrap->m_cacheList[idx] = item;
    return self;
}

static VALUE lnrbLNObjectList_Add(VALUE self, VALUE item) 
{
    wrapObjectList* selfWrap;
    Data_Get_Struct(self, wrapObjectList, selfWrap);
    lnrbLNObjectList_SyncItems(selfWrap);
	
    LNObjectList_Add(Manager::GetHandleFromtWrapperObject(self), Manager::GetHandleFromtWrapperObject(item));
	selfWrap->m_cacheList.push_back(item);
	return self;
}

static VALUE lnrbLNObjectList_Clear(VALUE self) 
{
    wrapObjectList* selfWrap;
    Data_Get_Struct(self, wrapObjectList, selfWrap);
    LNObjectList_Clear(Manager::GetHandleFromtWrapperObject(self));
	selfWrap->m_cacheList.clear();
	return self;
}

static VALUE lnrbLNObjectList_Insert(VALUE self, VALUE index, VALUE item) 
{
    wrapObjectList* selfWrap;
    Data_Get_Struct(self, wrapObjectList, selfWrap);
    lnrbLNObjectList_SyncItems(selfWrap);
	
    int idx = INT2FIX(index);
    LNObjectList_Insert(Manager::GetHandleFromtWrapperObject(self), idx, Manager::GetHandleFromtWrapperObject(item));
	selfWrap->m_cacheList.insert(selfWrap->m_cacheList.begin() + idx, item);
	return self;
}

static VALUE lnrbLNObjectList_Remove(VALUE self, VALUE item) 
{
    wrapObjectList* selfWrap;
    Data_Get_Struct(self, wrapObjectList, selfWrap);
    lnrbLNObjectList_SyncItems(selfWrap);
	
	LNBool removed;
    LNObjectList_Remove(Manager::GetHandleFromtWrapperObject(self), Manager::GetHandleFromtWrapperObject(item), &removed);
	std::vector<VALUE>::iterator itr = selfWrap->m_cacheList.begin();
	std::vector<VALUE>::iterator end = selfWrap->m_cacheList.end();
	for (; itr != end; ++itr)
	{
		if (*itr == item) {
			selfWrap->m_cacheList.erase(itr);
			break;
		}
	}
	return toVALUE(removed);
}

/*
static VALUE lnrbLNObjectList_remove_all(VALUE self, VALUE item) 
{
    LNObjectList_SyncItems();

    wrapObjectList* selfWrap;
    Data_Get_Struct(self, wrapObjectList, selfWrap);
	std::vector<VALUE>::iterator itr = selfWrap->m_cacheList.begin();
	std::vector<VALUE>::iterator end = selfWrap->m_cacheList.end();
	for (; itr != end;)
	{
		if (*itr == item) {
			itr = selfWrap->m_cacheList.erase(itr);
		}
		else {
			++itr;
		}
	}
    LNObjectList_RemoveAll(Manager::GetHandleFromtWrapperObject(self), Manager::GetHandleFromtWrapperObject(item));
	return self;
}
*/

static VALUE lnrbLNObjectList_RemoveAt(VALUE self, VALUE index) 
{
    wrapObjectList* selfWrap;
    Data_Get_Struct(self, wrapObjectList, selfWrap);
    lnrbLNObjectList_SyncItems(selfWrap);
	
	int idx = INT2FIX(index);
	selfWrap->m_cacheList.erase(selfWrap->m_cacheList.begin() + idx);
    LNObjectList_RemoveAt(Manager::GetHandleFromtWrapperObject(self), idx);
	return self;
}

// 関数名は create_makefile で指定した名前と合わせておかなければならない
extern "C" void Init_Lumino()
{
	// ルートモジュール
	g_luminoModule = rb_define_module("Lumino");
	
    // 例外定義
    g_luminoError = rb_define_class_under(g_luminoModule, "LuminoError", rb_eRuntimeError);
	
	// 終了時に実行される処理の登録
	rb_set_end_proc((void (*)(VALUE))Lumino_terminate, Qnil);
	
	// RefObject
    g_class_RefObject = rb_define_class_under(g_luminoModule, "RefObject", rb_cObject);
    
	// ObjectList
	g_class_ObjectList = rb_define_class_under(g_luminoModule, "ObjectList", g_class_RefObject);
	rb_define_method(g_class_ObjectList, "[]", LN_TO_RUBY_FUNC(lnrbLNObjectList_GetAt), 1);
	rb_define_method(g_class_ObjectList, "[]=", LN_TO_RUBY_FUNC(lnrbLNObjectList_SetAt), 2);
	rb_define_method(g_class_ObjectList, "count", LN_TO_RUBY_FUNC(lnrbLNObjectList_GetCount), 0);
	rb_define_method(g_class_ObjectList, "size", LN_TO_RUBY_FUNC(lnrbLNObjectList_GetCount), 0);
	rb_define_method(g_class_ObjectList, "add", LN_TO_RUBY_FUNC(lnrbLNObjectList_Add), 1);
	rb_define_method(g_class_ObjectList, "push", LN_TO_RUBY_FUNC(lnrbLNObjectList_Add), 1);
	rb_define_method(g_class_ObjectList, "clear", LN_TO_RUBY_FUNC(lnrbLNObjectList_Clear), 0);
	rb_define_method(g_class_ObjectList, "insert", LN_TO_RUBY_FUNC(lnrbLNObjectList_Insert), 2);
	rb_define_method(g_class_ObjectList, "remove", LN_TO_RUBY_FUNC(lnrbLNObjectList_Remove), 1);
	//rb_define_method(g_class_ObjectList, "remove_all", LN_TO_RUBY_FUNC(lnrbLNObjectList_RemoveAll), 1);
	rb_define_method(g_class_ObjectList, "remove_at", LN_TO_RUBY_FUNC(lnrbLNObjectList_RemoveAt), 1);
	
	Manager::Initialize();
}


