#include "LuminoRuby.h"

VALUE g_luminoModule;
VALUE g_luminoError;

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

// 関数名は create_makefile で指定した名前と合わせておかなければならない
extern "C" void Init_Lumino()
{
	// ルートモジュール
	g_luminoModule = rb_define_module("Lumino");
	
    // 例外定義
    g_luminoError = rb_define_class_under(g_luminoModule, "LuminoError", rb_eRuntimeError);
	
	Manager::Initialize();
}


