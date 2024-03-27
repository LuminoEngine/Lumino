
#include "Internal.hpp"
#include <LuminoEngine/Rendering/RenderingContext.hpp>
#include <LuminoFramework/Scene/MeshTilemap/MeshTilemapLayer.hpp>
#include <LuminoFramework/Scene/MeshTilemap/MeshTilemapModel.hpp>
#include <LuminoFramework/Scene/MeshTilemap/MeshTileset.hpp>

namespace ln {

//==============================================================================
// MeshTilemapLayer

LN_OBJECT_IMPLEMENT(MeshTilemapLayer, Object) {}

MeshTilemapLayer::MeshTilemapLayer()
{
}

void MeshTilemapLayer::init()
{
	Object::init();
}

void MeshTilemapLayer::draw(RenderingContext* context, MeshTileset* tileset)
{
}

} // namespace ln

