
#include "Internal.hpp"
#include <LuminoEngine/Scene/MeshTilemap/MeshTileset.hpp>

namespace ln {

//==============================================================================
// MeshAutoTileset

MeshAutoTileset::MeshAutoTileset()
{
}

Result<> MeshAutoTileset::init() {
	return Object::init();
}

//==============================================================================
// MeshTileset

LN_OBJECT_IMPLEMENT(MeshTileset, Object) {}

MeshTileset::MeshTileset()
{
}

Result<> MeshTileset::init() {
	return Object::init();
}

} // namespace ln
