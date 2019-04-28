
#include "Internal.hpp"
#include <LuminoEngine/Tilemap/TilemapModel.hpp>
#include <LuminoEngine/Tilemap/TilemapComponent.hpp>

#include <LuminoEngine/Scene/WorldObject.hpp>   // for WorldObjectTransform
#include <LuminoEngine/Rendering/RenderView.hpp>    // for RenderViewPoint

// Test
#include <LuminoEngine/Tilemap/Tileset.hpp>
#include <LuminoEngine/Tilemap/TilemapLayer.hpp>



namespace ln {

//==============================================================================
// TilemapComponent

TilemapComponent::TilemapComponent()
{
}

TilemapComponent::~TilemapComponent()
{
}

void TilemapComponent::init()
{
    VisualComponent::init();

    auto tileset = newObject<Tileset>();
    auto layer = newObject<TilemapLayer>();
    layer->resize(5, 5);
    layer->setTileSize(Size(1, 1));
    layer->setOrientation(TilemapOrientation::UpFlow);
    layer->setTileId(0, 0, 1);
    layer->setTileId(1, 1, 1);
    layer->setTileId(0, 4, 1);
    layer->setTileId(1, 4, 1);
    layer->setTileId(2, 4, 1);
    layer->setTileId(3, 4, 1);
    layer->setTileId(4, 4, 1);
    m_tilemapModel = newObject<TilemapModel>();
    m_tilemapModel->addTileset(tileset);
    m_tilemapModel->addLayer(layer);
}

void TilemapComponent::setTilemapModel(TilemapModel* tilemapModel)
{
    m_tilemapModel = tilemapModel;
}

void TilemapComponent::onRender(RenderingContext* context)
{
    Matrix worldInverse = Matrix::makeInverse(worldObject()->worldMatrix());
    Matrix viewLocal = context->viewPoint()->worldMatrix * worldInverse;

    Matrix viewMatrix = Matrix::makeLookAtLH(viewLocal.position(), viewLocal.position() + viewLocal.front(), viewLocal.up());
    ViewFrustum frustum(viewMatrix * context->viewPoint()->projMatrix);

    // ひとまず、 Z- を正面とする
    Plane plane(transrom()->position(), -transrom()->getFront());

    // TODO: 原点と正面方向
    Vector3 corners[8];
    Vector3 pt;
    frustum.getCornerPoints(corners);

    // TileMap の平面とカメラの視錐台から描画するべき範囲を求める
    detail::TilemapBounds bounds;
    for (int i = 0; i < 4; ++i)
    {
        if (plane.intersects(corners[i], corners[i + 4], &pt))
        {
            // pt をそのまま使う
        }
        else
        {
            // XY 平面上での最遠点を求める
            Vector2 pt2 = Vector2::normalize(corners[i + 4].xy() - corners[i].xy()) * context->viewPoint()->farClip;
            pt = Vector3(pt2, 0);
        }

        if (i == 0)
        {
            bounds.l = pt.x;
            bounds.t = pt.y;
            bounds.r = pt.x;
            bounds.b = pt.y;
        }
        else
        {
            bounds.l = std::min(bounds.l, pt.x);
            bounds.r = std::max(bounds.r, pt.x);
            bounds.t = std::max(bounds.t, pt.y);
            bounds.b = std::min(bounds.b, pt.y);
        }
    }

    
    m_tilemapModel->render(context, worldObject()->worldMatrix(), bounds);
}

} // namespace ln

