
#pragma once
#include "LNInternal.h"
#include "LNManager.h"


//=============================================================================
// LFManager
//=============================================================================

ApplicationConfigData	LFManager::ConfigData;
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
		Application = Lumino::Application::Create(ConfigData);
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

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LNResult LFManager::ProcException(Exception* e)
{
	if (e == NULL) {
		return LN_ERROR_UNKNOWN;
	}

#define ERROR_DEF(exceptionType, code) \
	{ \
		exceptionType* t = dynamic_cast<exceptionType*>(e); \
		if (t != NULL) { return code; } \
	}

	ERROR_DEF(OutOfMemoryException, LN_ERROR_OUT_OF_MEMORY);
	ERROR_DEF(IOException, LN_ERROR_IO);
	ERROR_DEF(EndOfStreamException, LN_ERROR_END_OF_STREAM);
	ERROR_DEF(ArgumentException, LN_ERROR_ARGUMENT);
	ERROR_DEF(InvalidOperationException, LN_ERROR_INVALID_OPERATION);
	ERROR_DEF(NotSupportedException, LN_ERROR_NOT_SUPPORTED);
	ERROR_DEF(FileNotFoundException, LN_ERROR_FILE_NOT_FOUND);
	ERROR_DEF(DirectoryNotFoundException, LN_ERROR_DIRECTORY_NOT_FOUND);
	ERROR_DEF(InvalidFormatException, LN_ERROR_INVALID_FORMAT);
	ERROR_DEF(NotImplementedException, LN_ERROR_NOT_IMPLEMENTED);
	ERROR_DEF(RuntimeException, LN_ERROR_RUNTIME);
	ERROR_DEF(EncodingFallbackException, LN_ERROR_ENCODING_FALLBACK);
	ERROR_DEF(Win32Exception, LN_ERROR_WIN32);
	ERROR_DEF(COMException, LN_ERROR_COM);

#undef ERROR_DEF

	return LN_ERROR_UNKNOWN;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void LFManager::ReleaseObject(LNHandle handle)
{
	if (!IsSystemInitialized)
		return;		// すべて解放済み

	int index = TO_INDEX(handle);
	ObjectEntry& e = m_objectEntryList[index];

	if (e.Object != NULL) {
		e.RefCount--;
		if (e.RefCount <= 0) {
			LN_SAFE_RELEASE(e.Object);
		}
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void LFManager::AddRefObject(LNHandle handle)
{
	int index = TO_INDEX(handle);
	ObjectEntry& e = m_objectEntryList[index];
	if (e.Object != NULL) {
		e.RefCount++;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ObjectEntry* LFManager::GetObjectEntry(LNHandle handle)
{
	return &m_objectEntryList[TO_INDEX(handle)];
}