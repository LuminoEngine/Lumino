
#include "../../include/Lumino/Math/Version.h"

namespace Lumino
{
namespace Version
{

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
int Math::GetMajor()
{
	return LUMINO_MATH_VERSION_MAJOR;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
int Math::GetMinor()
{
	return LUMINO_MATH_VERSION_MINOR;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
int Math::GetPatch()
{
	return LUMINO_MATH_VERSION_PATCH;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
const char* Math::GetVersionString()
{
	return LUMINO_MATH_VERSION_STRING;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool Math::IsAtLeast(int major, int minor, int patch)
{
	if (LUMINO_MATH_VERSION_MAJOR < major) return false;
	if (LUMINO_MATH_VERSION_MAJOR > major) return true;
	if (LUMINO_MATH_VERSION_MINOR < minor) return false;
	if (LUMINO_MATH_VERSION_MINOR > minor) return true;
	if (LUMINO_MATH_VERSION_PATCH < patch) return false;
	return true;
}

} // namespace Version
} // namespace Lumino
