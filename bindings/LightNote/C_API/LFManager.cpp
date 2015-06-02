
#pragma once
#include "LFInternal.h"
#include "LFManager.h"

namespace Lumino
{

//=============================================================================
// LFManager
//=============================================================================

Lumino::Application*	LFManager::Application = NULL;
bool					LFManager::IsSystemInitialized = false;

LFManager::ObjectEntryList	LFManager::m_objectEntryList;
Stack<int>					LFManager::m_objectIndexStack;

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void LFManager::PreInitialize()
{
	if (Application == NULL)
	{
		//CheckCommonDefinition();
		Application = Lumino::Application::Create();
		IsSystemInitialized = true;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void LFManager::PostInitialize()
{
	// オブジェクト管理配列
	for (int i = 0; i < 512; ++i)
	{
		ObjectEntry e;
		e.Object = NULL;
		e.Index = i;
		m_objectEntryList.Add(e);

		if (i > 0) {  // 0 は NULL 扱い。欠番にする。
			m_objectIndexStack.Push(i);
		}
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void LFManager::Finalize()
{
	// 残っているオブジェクトを全て削除
	LN_FOREACH(ObjectEntry& e, m_objectEntryList) {
		LN_SAFE_RELEASE(e.Object);
	}

	if (Application != NULL)
	{
		//Application->Finalize();
		LN_SAFE_RELEASE(Application);
	}

	IsSystemInitialized = false;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LNHandle LFManager::CheckRegisterObject(CoreObject* obj)
{
	LN_VERIFY(obj != NULL) { return NULL; }

	// 登録済みならハンドル (管理配列上のインデックス) を返す
	if (obj->GetUserData() != NULL) {
		return ((ObjectRegisterData*)obj->GetUserData())->Index;
	}

	// 管理配列がすべて埋まっている場合
	if (m_objectIndexStack.IsEmpty())
	{
		// 末尾に追加する
		ObjectEntry e;
		e.Object = obj;
		//e.Interface = obj;
		e.Index = m_objectEntryList.GetCount();
		e.RefCount = 1;
		m_objectEntryList.Add(e);

		// ユーザーデータ登録
		ObjectRegisterData* data = LN_NEW ObjectRegisterData();
		data->Index = e.Index;
		obj->SetUserData(data);

		return e.Index;
	}
	else
	{
		// 空き場所を取得
		int newPos = m_objectIndexStack.GetTop();
		m_objectIndexStack.Pop();

		// 格納
		ObjectEntry& e = m_objectEntryList[newPos];

		e.Object = obj;
		e.Index = newPos;
		e.RefCount = 1;

		// ユーザーデータ登録
		ObjectRegisterData* data = LN_NEW ObjectRegisterData();
		data->Index = e.Index;
		obj->SetUserData(data);

		return e.Index;
	}
}

} // namespace Lumino
