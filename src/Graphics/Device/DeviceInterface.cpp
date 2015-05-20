
#include "../../Internal.h"
#include "DeviceInterface.h"

namespace Lumino
{
namespace Graphics
{
namespace Device
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

} // namespace Device
} // namespace Graphics
} // namespace Lumino
