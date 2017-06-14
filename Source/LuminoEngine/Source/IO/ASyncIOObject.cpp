
#include <Lumino/Base/Exception.h>
#include <Lumino/IO/ASyncIOObject.h>
#include <Lumino/IO/FileManager.h>

LN_NAMESPACE_BEGIN

//==============================================================================
// ASyncIOObject
//==============================================================================

//------------------------------------------------------------------------------
ASyncIOObject::ASyncIOObject()
	: m_ayncIOState(ASyncIOState_Idle)
	, m_ayncIOException(NULL)
{
}

//------------------------------------------------------------------------------
ASyncIOObject::~ASyncIOObject()
{
	LN_SAFE_DELETE(m_ayncIOException);
}

//------------------------------------------------------------------------------
ASyncIOState ASyncIOObject::getASyncIOState() const
{
	return m_ayncIOState;
}

//------------------------------------------------------------------------------
Exception* ASyncIOObject::getASyncIOException() const
{
	return m_ayncIOException;
}

//------------------------------------------------------------------------------
void ASyncIOObject::invokeIOProc(bool isASync, FileManager* fileManager)
{
	if (isASync) {
		fileManager->requestASyncTask(this);
	}
	else
	{
		m_ayncIOState = ASyncIOState_Processing;
		try
		{
			onASyncIOProc();
		}
		catch (...)
		{
			m_ayncIOState = ASyncIOState_Failed;
			throw;	// メンバには保持せず、呼び出し側に投げる
		}
		m_ayncIOState = ASyncIOState_Completed;
	}
}

LN_NAMESPACE_END
