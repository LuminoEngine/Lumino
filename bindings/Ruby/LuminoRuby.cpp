#include "LuminoRuby.h"

VALUE g_luminoModule;
VALUE g_luminoError;

ln::Array<TypeInfo>	Manager::m_typeInfoList;
ln::Array<VALUE>	Manager:m_objectList;
ln::Stack<int>		Manager:m_objectListIndexStack;

//------------------------------------------------------------------------------
void Manager::Initialize()
{
	// 管理リストの空き番号を詰める
	for (int i = 0; i < InitialListSize; i++)
	{
		m_objectList.Add(Qnil);
		m_objectListIndexStack.Push(i);
	}
	
	// Ruby 系の初期化
	InitEnums();
	InitStructs();
	InitClasses();
	
	// 型情報の登録
	m_typeInfoList.Add(TypeInfo());	// ダミーを1つ詰めておく
	RegisterTypeInfo();
}

//------------------------------------------------------------------------------
void Manager::Finalize()
{
}

//------------------------------------------------------------------------------
VALUE Manager::GetWrapperObjectFromHandle(LNHandle handle)
{
	int objectIndex = (int)LNObject_GetUserData(handle);
	if (objectIndex == 0)
	{
		int typeinfoIndex = (int)LNObject_GetTypeUserData(handle);
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
void Manager::RegisterWrapperObject(VALUE obj)
{
	// 管理リストが一杯の時は拡張する
	if (m_objectListIndexStack.GetCount() == 0)
	{
		int growCount = m_objectList.GetCount();
		for (int i = 0; i < growCount; i++)
		{
			m_objectList.Add(Qnil);
			m_objectListIndexStack.Push(growCount + i);
		}
	}
	
	int index = m_objectListIndexStack.Pop();
	m_objectList[index] = obj;
}

//------------------------------------------------------------------------------
void Manager::UnregisterWrapperObject(LNHandle handle)
{
	int index = (int)LNObject_GetUserData(handle);
	m_objectList[index] = Qnil;
	m_objectListIndexStack.Push(index);
}

//------------------------------------------------------------------------------
const char* LNGetLastErrorMessage()
{
	const LNChar* str;
	LNError_GetLastErrorMessage(&str);
	return LNInternal_ConvertToUTF8String(str, -1);
}

// 関数名は create_makefile で指定した名前と合わせておかなければならない
extern "C" void Init_Lumino()
{
	// ルートモジュール
	g_luminoModule = rb_define_module("Lumino");
	
    // 例外定義
    g_luminoError = rb_define_class_under(g_luminoModule, "LuminoError", rb_eRuntimeError);
	
	Manager::Initialize();
}


