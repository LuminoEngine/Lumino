
#include "Internal.h"
#include "../../LuminoEngine/Source/EngineDomain.h"
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
void LFManager::preInitialize()
{
	if (Engine == nullptr)
	{
		Engine = detail::EngineDomain::getEngineManager();
		Engine->initialize(ConfigData);
		IsSystemInitialized = true;
	}
}

//------------------------------------------------------------------------------
void LFManager::postInitialize()
{
	// 文字コード変換器
	TCharToUTF8Converter = LN_NEW EncodingConverter();
	TCharToUTF8Converter->getSourceEncoding(Encoding::getTCharEncoding());
	TCharToUTF8Converter->setDestinationEncoding(Encoding::getUTF8Encoding());

	// オブジェクト管理配列
	for (int i = 511; i >= 0; --i)
	{
		ObjectEntry e;
		e.Object = NULL;
		e.Index = i;
		m_objectEntryList.add(e);

		if (i > 0) {  // 0 は NULL 扱い。欠番にする。
			m_objectIndexStack.push(i);
		}
	}
}

//------------------------------------------------------------------------------
void LFManager::terminate()
{
	// 残っているオブジェクトを全て削除
	for (ObjectEntry& e : m_objectEntryList) {
		LN_SAFE_RELEASE(e.Object);
	}

	if (Engine != NULL)
	{
		LN_SAFE_RELEASE(Engine);
	}

	LN_SAFE_DELETE(LastException);
	LN_SAFE_DELETE(TCharToUTF8Converter);

	IsSystemInitialized = false;
}

//------------------------------------------------------------------------------
LNHandle LFManager::CheckRegisterObject(Object* obj)
{
	if (obj == nullptr) return NULL;

	// 登録済みならハンドル (管理配列上のインデックス) を返す
	if (obj->getUserData() != NULL)
	{
		ObjectRegisterData* data = (ObjectRegisterData*)obj->getUserData();
		//obj->AddRef();
		return data->Index;
	}

	// 管理配列がすべて埋まっている場合
	if (m_objectIndexStack.isEmpty())
	{
		// 末尾に追加する
		ObjectEntry e;
		e.Object = obj;
		obj->addRef();
		//e.Interface = obj;
		e.Index = m_objectEntryList.getCount();
		e.RefCount = 1;
		m_objectEntryList.add(e);

		// ユーザーデータ登録
		ObjectRegisterData* data = LN_NEW ObjectRegisterData();
		data->Index = e.Index;
		obj->setUserData(data);

		//obj->AddRef();
		return e.Index;
	}
	else
	{
		// 空き場所を取得
		int newPos = m_objectIndexStack.getTop();
		m_objectIndexStack.pop();

		// 格納
		ObjectEntry& e = m_objectEntryList[newPos];

		e.Object = obj;
		obj->addRef();
		e.Index = newPos;
		e.RefCount = 1;

		// ユーザーデータ登録
		ObjectRegisterData* data = LN_NEW ObjectRegisterData();
		data->Index = e.Index;
		obj->setUserData(data);

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

	ERROR_DEF(NotImplementedException, ResultCode::ErrorNOT_IMPLEMENTED);
	ERROR_DEF(IOException, ResultCode::ErrorIO);
	ERROR_DEF(FileNotFoundException, ResultCode::ErrorFILE_NOT_FOUND);
	ERROR_DEF(InvalidFormatException, ResultCode::ErrorINVALID_FORMAT);
	ERROR_DEF(EncodingException, ResultCode::ErrorENCODING);

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
			m_objectIndexStack.push(index);
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
	return m_objectEntryList.getCount() - m_objectIndexStack.getCount();
}
