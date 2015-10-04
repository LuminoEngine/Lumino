
#include "../../Internal.h"
#include "GraphicsDriverInterface.h"

namespace Lumino
{
LN_NAMESPACE_GRAPHICS_BEGIN
namespace Driver
{

//=============================================================================
// IShader
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
IShaderVariable* IShader::GetVariableByName(const TCHAR* name) const
{
	int count = GetVariableCount();
	for (int i = 0; i < count; ++i)
	{
		IShaderVariable* v = GetVariable(i);
		if (v->GetName() == name) {
			return v;
		}
	}
	return NULL;
}

} // namespace Driver
LN_NAMESPACE_GRAPHICS_END
} // namespace Lumino
