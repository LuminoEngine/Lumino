
#include "Internal.hpp"
#include <LuminoEngine/Graphics/Texture.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Tilemap/TilemapComponent.hpp>

#include <LuminoEngine/Scene/WorldObject.hpp>   // for WorldObjectTransform
#include <LuminoEngine/Rendering/RenderView.hpp>    // for RenderViewPoint

namespace ln {

//==============================================================================
// TilemapComponent

TilemapComponent::TilemapComponent()
{
}

TilemapComponent::~TilemapComponent()
{
}

void TilemapComponent::initialize()
{
    VisualComponent::initialize();

    m_material = Material::create();
    m_material->setMainTexture(newObject<Texture2D>(u"D:/tmp/110220c_as019.png"));
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
    //Plane plane(Vector3(0, 0, 0), Vector3(0, 0, -1));
    Vector3 corners[8];
    Vector3 pt;
    frustum.getCornerPoints(corners);

    // TileMap の平面とカメラの視錐台から描画するべき範囲を求める
    float l, t, r, b;
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
            l = pt.x;
            t = pt.y;
            r = pt.x;
            b = pt.y;
        }
        else
        {
            l = std::min(l, pt.x);
            r = std::max(r, pt.x);
            t = std::max(t, pt.y);
            b = std::min(b, pt.y);
        }
    }


    context->drawSprite(Matrix::Identity, Size(5, 5), Vector2::Zero, Rect(0, 0, 1, 1), Color::White, SpriteBaseDirection::ZMinus, BillboardType::None, m_material);
}

} // namespace ln

