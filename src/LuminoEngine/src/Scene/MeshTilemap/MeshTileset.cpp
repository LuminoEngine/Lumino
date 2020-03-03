
#include "Internal.hpp"
#include <LuminoEngine/Graphics/Texture.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Rendering/RenderingContext.hpp>
#include <LuminoEngine/Scene/MeshTilemap/MeshTileset.hpp>

namespace ln {

//==============================================================================
// AutoMeshTileset

AutoMeshTileset::AutoMeshTileset()
{
}

void AutoMeshTileset::init()
{
	Object::init();
}

//==============================================================================
// MeshTileset

LN_OBJECT_IMPLEMENT(MeshTileset, Object) {}

MeshTileset::MeshTileset()
{
}

void MeshTileset::init()
{
    Object::init();
}

} // namespace ln

