
#include "Internal.h"
#include <Lumino/Version.h>

LN_NAMESPACE_BEGIN
namespace Version
{

//------------------------------------------------------------------------------
int Engine::GetMajor()
{
	return LUMINO_ENGINE_VERSION_MAJOR;
}

//------------------------------------------------------------------------------
int Engine::GetMinor()
{
	return LUMINO_ENGINE_VERSION_MINOR;
}

//------------------------------------------------------------------------------
int Engine::GetRevision()
{
	return LUMINO_ENGINE_VERSION_REVISION;
}

//------------------------------------------------------------------------------
int Engine::GetBuild()
{
	return LUMINO_ENGINE_VERSION_BUILD;
}

//------------------------------------------------------------------------------
const TCHAR* Engine::GetString()
{
	return LUMINO_ENGINE_VERSION_STRING;
}

//------------------------------------------------------------------------------
bool Engine::IsAtLeast(int major, int minor, int revision)
{
	if (LUMINO_ENGINE_VERSION_MAJOR < major) return false;
	if (LUMINO_ENGINE_VERSION_MAJOR > major) return true;
	if (LUMINO_ENGINE_VERSION_MINOR < minor) return false;
	if (LUMINO_ENGINE_VERSION_MINOR > minor) return true;
	if (LUMINO_ENGINE_VERSION_REVISION < revision) return false;
	return true;
}

} // namespace Version
LN_NAMESPACE_END
