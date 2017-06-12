
#include "Internal.h"
#include "BindingManager.h"

//==============================================================================
// LFManager
//==============================================================================

detail::EngineSettings		LFManager::ConfigData;
EngineManager*				LFManager::Engine = NULL;
bool						LFManager::IsSystemInitialized = false;
Exception*					LFManager::LastException = NULL;
ResultCode					LFManager::LastErrorCode = ResultCode::OK;
EncodingConverter*			LFManager::TCharToUTF8Converter = NULL;

LFManager::ObjectEntryList	LFManager::m_objectEntryList;
Stack<int>					LFManager::m_objectIndexStack;

//------------------------------------------------------------------------------
void LFManager::PreInitialize()
{
	if (Engine == nullptr)
	{
		Engine = EngineManager::Create(ConfigData);
		IsSystemInitialized = true;
	}
}

//------------------------------------------------------------------------------
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

//------------------------------------------------------------------------------
void LFManager::Terminate()
{
	// 残っているオブジェクトを全て削除
	for (ObjectEntry& e : m_objectEntryList) {
		LN_SAFE_RELEASE(e.Object);
	}

	if (Engine != NULL)
	{
		//Engine->Finalize();
		LN_SAFE_RELEASE(Engine);
	}

	LN_SAFE_DELETE(LastException);
	LN_SAFE_DELETE(TCharToUTF8Converter);

	IsSystemInitialized = false;
}

//------------------------------------------------------------------------------
LNHandle LFManager::CheckRegisterObject(tr::ReflectionObject* obj)
{
	if (obj == nullptr) return NULL;

	// 登録済みならハンドル (管理配列上のインデックス) を返す
	if (obj->GetUserData() != NULL)
	{
		ObjectRegisterData* data = (ObjectRegisterData*)obj->GetUserData();
		//obj->AddRef();
		return data->Index;
	}

	// 管理配列がすべて埋まっている場合
	if (m_objectIndexStack.IsEmpty())
	{
		// 末尾に追加する
		ObjectEntry e;
		e.Object = obj;
		obj->addRef();
		//e.Interface = obj;
		e.Index = m_objectEntryList.GetCount();
		e.RefCount = 1;
		m_objectEntryList.Add(e);

		// ユーザーデータ登録
		ObjectRegisterData* data = LN_NEW ObjectRegisterData();
		data->Index = e.Index;
		obj->SetUserData(data);

		//obj->AddRef();
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
		obj->addRef();
		e.Index = newPos;
		e.RefCount = 1;

		// ユーザーデータ登録
		ObjectRegisterData* data = LN_NEW ObjectRegisterData();
		data->Index = e.Index;
		obj->SetUserData(data);

		//obj->AddRef();
		return e.Index;
	}
}

//------------------------------------------------------------------------------
ResultCode LFManager::ProcException(Exception* e)
{
	if (e == NULL) {
		return ResultCode::ErrorUnknown;
	}

	// 最後に発生した例外として覚えておく
	LN_SAFE_DELETE(LastException);
	LastException = e->copy();

#define ERROR_DEF(exceptionType, code) \
	{ \
		exceptionType* t = dynamic_cast<exceptionType*>(e); \
		if (t != NULL) { LastErrorCode = code; return code; } \
	}

	ERROR_DEF(VerifyException, ResultCode::ErrorVERIFY);
	ERROR_DEF(ArgumentException, ResultCode::ErrorARGUMENT);
	ERROR_DEF(InvalidOperationException, ResultCode::ErrorINVALID_OPERATION);
	ERROR_DEF(NotImplementedException, ResultCode::ErrorNOT_IMPLEMENTED);
	ERROR_DEF(OutOfMemoryException, ResultCode::ErrorOUT_OF_MEMORY);
	ERROR_DEF(OutOfRangeException, ResultCode::ErrorOUT_OF_RANGE);
	ERROR_DEF(KeyNotFoundException, ResultCode::ErrorKEY_NOT_FOUND);
	ERROR_DEF(OverflowException, ResultCode::ErrorOVERFLOW);
	ERROR_DEF(IOException, ResultCode::ErrorIO);
	ERROR_DEF(FileNotFoundException, ResultCode::ErrorFILE_NOT_FOUND);
	ERROR_DEF(DirectoryNotFoundException, ResultCode::ErrorDIRECTORY_NOT_FOUND);
	ERROR_DEF(InvalidFormatException, ResultCode::ErrorINVALID_FORMAT);
	ERROR_DEF(EndOfStreamException, ResultCode::ErrorEND_OF_STREAM);
	ERROR_DEF(EncodingException, ResultCode::ErrorENCODING);
	ERROR_DEF(Win32Exception, ResultCode::ErrorWIN32);
	ERROR_DEF(COMException, ResultCode::ErrorCOM);

#undef ERROR_DEF

	LastErrorCode = ResultCode::ErrorUnknown;
	return ResultCode::ErrorUnknown;
}

//------------------------------------------------------------------------------
void LFManager::ReleaseObject(LNHandle handle)
{
	if (!IsSystemInitialized)
		return;		// すべて解放済み

	int index = LWIG_TO_INDEX(handle);
	ObjectEntry& e = m_objectEntryList[index];

	if (e.Object != NULL)
	{
		e.RefCount--;
		if (e.RefCount <= 0)
		{
			LN_SAFE_RELEASE(e.Object);

			// Index 返却
			m_objectIndexStack.Push(index);
		}
	}
}

//------------------------------------------------------------------------------
void LFManager::AddRefObject(LNHandle handle)
{
	int index = LWIG_TO_INDEX(handle);
	ObjectEntry& e = m_objectEntryList[index];
	if (e.Object != NULL) {
		e.RefCount++;
	}
}

//------------------------------------------------------------------------------
ObjectEntry* LFManager::GetObjectEntry(LNHandle handle)
{
	return &m_objectEntryList[LWIG_TO_INDEX(handle)];
}

//------------------------------------------------------------------------------
int LFManager::GetHandleCount()
{
	return m_objectEntryList.GetCount() - m_objectIndexStack.GetCount();
}
