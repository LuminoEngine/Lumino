
#include "Internal.hpp"
#include <LuminoEngine/Scene/World.hpp>
#include <LuminoEngine/Scene/Level.hpp>
#include <LuminoEngine/Tilemap/TilemapModel.hpp>
#include <LuminoEngine/Tilemap/TilemapComponent.hpp>
#include "TilemapPhysicsObject.hpp"

#include <LuminoEngine/Scene/WorldObject.hpp>   // for WorldObjectTransform
#include <LuminoGraphics/Rendering/RenderView.hpp>    // for RenderViewPoint

// Test
#include <LuminoEngine/Tilemap/Tileset.hpp>
#include <LuminoEngine/Tilemap/TilemapLayer.hpp>



namespace ln {

//==============================================================================
// TilemapComponent

LN_OBJECT_IMPLEMENT(TilemapComponent, VisualComponent) {}

TilemapComponent::TilemapComponent()
{
}

TilemapComponent::~TilemapComponent()
{
}

void TilemapComponent::init()
{
    VisualComponent::init();
}

TilemapModel* TilemapComponent::tilemapModel() const
{
    return m_tilemapModel;
}

void TilemapComponent::setTilemapModel(TilemapModel* tilemapModel)
{
    m_tilemapModel = tilemapModel;



}

bool TilemapComponent::intersectTile(const Ray& rayOnWorld, PointI* tilePoint)
{
	if (!m_tilemapModel) return false;

	Matrix worldInverse = Matrix::makeInverse(worldObject()->worldMatrix());
	Ray localRay(Vector3::transformCoord(rayOnWorld.origin, worldInverse), Vector3::transformCoord(rayOnWorld.direction, worldInverse), rayOnWorld.distance);

	TilemapLayer* layer = m_tilemapModel->layer(0);

	// TODO: ひとまず、 Z- を正面とする
	Plane plane(Vector3::Zero, -Vector3::UnitZ);

	Vector3 pt;
	if (plane.intersects(localRay, &pt)) {
        if (pt.x < 0.0 || pt.y < 0.0) return false;   // 小数切り捨てで、-0.99..~0.0 が 0 として扱われる問題の回避

		// TODO: スケールを考慮したい
		int x = static_cast<int>(pt.x);
		int y = static_cast<int>(pt.y);
		if (m_tilemapModel->isValidTilePosition(x, y)) {
			if (tilePoint) {
				tilePoint->x = x;
				tilePoint->y = (layer->getHeight() - y) - 1;
			}
			return true;
		}
	}
	return false;
}

void TilemapComponent::onStart()
{
	m_rigidBody = makeObject<RigidBody2D>();
	m_rigidBody->addCollisionShape(detail::TilemapPhysicsObject::createTilemapCollisionShape(m_tilemapModel));
	

    // TODO: addPhysicsObject をどうにかする
    //worldObject()->scene()->world()->physicsWorld2D()->addPhysicsObject(m_rigidBody);
    // TODO: m_rigidBody->removeFromPhysicsWorld(); をどうにかする
    LN_NOTIMPLEMENTED();
}

void TilemapComponent::onRender(RenderingContext* context)
{
    Matrix worldInverse = Matrix::makeInverse(worldObject()->worldMatrix());
    Matrix viewLocal = context->viewPoint()->worldMatrix * worldInverse;

#ifdef LN_COORD_RH
    Matrix viewMatrix = Matrix::makeLookAtRH(viewLocal.position(), viewLocal.position() + viewLocal.front(), viewLocal.up());
#else
    Matrix viewMatrix = Matrix::makeLookAtLH(viewLocal.position(), viewLocal.position() + viewLocal.front(), viewLocal.up());
#endif
    ViewFrustum frustum(viewMatrix * context->viewPoint()->projMatrix);

    // TODO: ひとまず、 Z- を正面とする
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

void TilemapComponent::onRenderGizmo(RenderingContext* context)
{
    if (m_tilemapModel) {
        Vector2 tileSize = m_tilemapModel->tileSize();
        int w = m_tilemapModel->width();
        int h = m_tilemapModel->height();
        float l = 0;
        float r = tileSize.x * (w + 1);
        float t = tileSize.y * (h + 1);
        float b = 0;

        for (int y = 0; y <= h; y++) {
            float p = tileSize.y * y;
            context->drawLine(Vector3(l, p, 0), Color::Orange, Vector3(r, p, 0), Color::Orange);
        }

        for (int x = 0; x <= w; x++) {
            float p = tileSize.x * x;
            context->drawLine(Vector3(p, t, 0), Color::Orange, Vector3(p, b, 0), Color::Orange);
        }
    }
}

//void TilemapComponent::serialize(Archive& ar)
//{
//	VisualComponent::serialize(ar);
//	ar & makeNVP(u"Model", m_tilemapModel);
//}

} // namespace ln

