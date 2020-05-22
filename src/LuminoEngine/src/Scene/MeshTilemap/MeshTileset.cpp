
#include "Internal.hpp"
#include <LuminoEngine/Scene/MeshTilemap/MeshTileset.hpp>

namespace ln {

//==============================================================================
// MeshAutoTileset

MeshAutoTileset::MeshAutoTileset()
{
}

bool MeshAutoTileset::init()
{
	return Object::init();
}

//==============================================================================
// MeshTileset

LN_OBJECT_IMPLEMENT(MeshTileset, Object) {}

MeshTileset::MeshTileset()
{
}

bool MeshTileset::init()
{
	return Object::init();
}

} // namespace ln
