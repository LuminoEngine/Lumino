
#include "Internal.h"
#include <Lumino/Version.h>

LN_NAMESPACE_BEGIN
namespace Version
{

//------------------------------------------------------------------------------
int GetMajor()
{
	return LUMINO_VERSION_MAJOR;
}

//------------------------------------------------------------------------------
int GetMinor()
{
	return LUMINO_VERSION_MINOR;
}

//------------------------------------------------------------------------------
int GetRevision()
{
	return LUMINO_MERSION_REVISION;
}

//------------------------------------------------------------------------------
const TCHAR* GetString()
{
	return LUMINO_VERSION_STRING;
}

//------------------------------------------------------------------------------
bool IsAtLeast(int major, int minor, int revision)
{
	if (LUMINO_VERSION_MAJOR < major) return false;
	if (LUMINO_VERSION_MAJOR > major) return true;
	if (LUMINO_VERSION_MINOR < minor) return false;
	if (LUMINO_VERSION_MINOR > minor) return true;
	if (LUMINO_MERSION_REVISION < revision) return false;
	return true;
}

} // namespace Version
LN_NAMESPACE_END
