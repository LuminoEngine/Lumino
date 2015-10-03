
#pragma once
#include "LNInternal.h"
#include "LNManager.h"


//=============================================================================
// LFManager
//=============================================================================

ApplicationSettings			LFManager::ConfigData;
Lumino::ApplicationImpl*	LFManager::Application = NULL;
bool						LFManager::IsSystemInitialized = false;
Exception*					LFManager::LastException = NULL;
LNResult					LFManager::LastErrorCode = LN_OK;
EncodingConverter*			LFManager::TCharToUTF8Converter = NULL;

LFManager::ObjectEntryList	LFManager::m_objectEntryList;
Stack<int>					LFManager::m_objectIndexStack;

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void LFManager::PreInitialize()
{
	if (Application == NULL)
	{
		LNTypeDef_CheckCommonDefinition();
		Application = Lumino::ApplicationImpl::Create(ConfigData);
		IsSystemInitialized = true;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void LFManager::PostInitialize()
{
	// 文字コード変換器
	TCharToUTF8Converter = LN_NEW EncodingConverter();
	TCharToUTF8Converter->SetSourceEncoding(Encoding::GetTCharEncoding());
	TCharToUTF8Converter->SetDestinationEncoding(Encoding::GetUTF8Encoding());

	// オブジェクト管理配列
	for (int i = 511; i >= 0; --i)
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
void LFManager::Terminate()
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

	LN_SAFE_DELETE(LastException);
	LN_SAFE_DELETE(TCharToUTF8Converter);

	IsSystemInitialized = false;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LNHandle LFManager::CheckRegisterObject(CoreObject* obj)
{
	LN_CHECK_ARGS_RETURNV(obj != NULL, NULL);

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

	// 最後に発生した例外として覚えておく
	LN_SAFE_DELETE(LastException);
	LastException = e->Copy();

#define ERROR_DEF(exceptionType, code) \
	{ \
		exceptionType* t = dynamic_cast<exceptionType*>(e); \
		if (t != NULL) { LastErrorCode = code; return code; } \
	}

	ERROR_DEF(VerifyException, LN_ERROR_VERIFY);
	ERROR_DEF(ArgumentException, LN_ERROR_ARGUMENT);
	ERROR_DEF(InvalidOperationException, LN_ERROR_INVALID_OPERATION);
	ERROR_DEF(NotImplementedException, LN_ERROR_NOT_IMPLEMENTED);
	ERROR_DEF(OutOfMemoryException, LN_ERROR_OUT_OF_MEMORY);
	ERROR_DEF(OutOfRangeException, LN_ERROR_OUT_OF_RANGE);
	ERROR_DEF(KeyNotFoundException, LN_ERROR_KEY_NOT_FOUND);
	ERROR_DEF(OverflowException, LN_ERROR_OVERFLOW);
	ERROR_DEF(IOException, LN_ERROR_IO);
	ERROR_DEF(FileNotFoundException, LN_ERROR_FILE_NOT_FOUND);
	ERROR_DEF(DirectoryNotFoundException, LN_ERROR_DIRECTORY_NOT_FOUND);
	ERROR_DEF(InvalidFormatException, LN_ERROR_INVALID_FORMAT);
	ERROR_DEF(EndOfStreamException, LN_ERROR_END_OF_STREAM);
	ERROR_DEF(EncodingException, LN_ERROR_ENCODING);
	ERROR_DEF(Win32Exception, LN_ERROR_WIN32);
	ERROR_DEF(COMException, LN_ERROR_COM);

#undef ERROR_DEF

	LastErrorCode = LN_ERROR_UNKNOWN;
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