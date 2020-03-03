
#include "Internal.hpp"
#include <LuminoEngine/Scene/MeshTilemap/MeshTilemapModel.hpp>

namespace ln {

//==============================================================================
// MeshTilemapModel

LN_OBJECT_IMPLEMENT(MeshTilemapModel, Object) {}

MeshTilemapModel::MeshTilemapModel()
{
}

void MeshTilemapModel::init()
{
    Object::init();
}

void MeshTilemapModel::draw(RenderingContext* context, const Matrix& transform)
{
}

} // namespace ln

