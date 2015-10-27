
#include "../../Internal.h"
#include "MMEShaderErrorInfo.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_SCENE_BEGIN
	
//=============================================================================
// MMEShaderErrorInfo
//=============================================================================
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void MMEShaderErrorInfo::AddError(MMEShaderError errorCode)
{
	MMEShaderErrorObject e;
	e.ErrorCode = errorCode;
	m_errorList.Add(e);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void MMEShaderErrorInfo::AddError(MMEShaderError errorCode, const String& message)
{
	MMEShaderErrorObject e;
	e.ErrorCode = errorCode;
	e.Message = message;
	m_errorList.Add(e);
}

LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END
