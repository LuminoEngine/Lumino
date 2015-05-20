
#include "../../Internal.h"
#include "MMEShaderErrorInfo.h"

namespace Lumino
{
namespace Scene
{
	
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

} // namespace Scene
} // namespace Lumino
