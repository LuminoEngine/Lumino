
#include "Internal.h"
#include <Lumino/Version.h>

LN_NAMESPACE_BEGIN

//------------------------------------------------------------------------------
int Version::getMajor()
{
	return LUMINO_VERSION_MAJOR;
}

//------------------------------------------------------------------------------
int Version::getMinor()
{
	return LUMINO_VERSION_MINOR;
}

//------------------------------------------------------------------------------
int Version::getRevision()
{
	return LUMINO_VERSION_REVISION;
}

//------------------------------------------------------------------------------
int Version::getBuild()
{
	return LUMINO_VERSION_BUILD;
}

//------------------------------------------------------------------------------
const Char* Version::getString()
{
	return LUMINO_VERSION_STRING;
}

//------------------------------------------------------------------------------
bool Version::isAtLeast(int major, int minor, int revision)
{
	if (LUMINO_VERSION_MAJOR < major) return false;
	if (LUMINO_VERSION_MAJOR > major) return true;
	if (LUMINO_VERSION_MINOR < minor) return false;
	if (LUMINO_VERSION_MINOR > minor) return true;
	if (LUMINO_VERSION_REVISION < revision) return false;
	return true;
}

LN_NAMESPACE_END
