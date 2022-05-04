
#include "Internal.hpp"
#include <LuminoGraphics/Rendering/RenderingContext.hpp>
#include <LuminoEngine/Scene/MeshTilemap/MeshTilemapLayer.hpp>
#include <LuminoEngine/Scene/MeshTilemap/MeshTilemapModel.hpp>
#include <LuminoEngine/Scene/MeshTilemap/MeshTileset.hpp>

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

