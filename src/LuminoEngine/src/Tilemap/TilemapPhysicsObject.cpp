
#include "Internal.hpp"
#include "TilemapPhysicsObject.hpp"

namespace ln {
namespace detail {

//==============================================================================
// TilemapPhysicsObject
//
//TilemapPhysicsObject::TilemapPhysicsObject()
//{
//}
//
//void TilemapPhysicsObject::init()
//{
//    VisualObject::init();
//}



Ref<RigidBody2D> TilemapPhysicsObject::createTilemapBodyFromLayer(const TilemapLayer* layer)
{
	int width = layer->getWidth();
	int height = layer->getWidth();
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			// 周囲のタイルによって、完全に侵入を妨げられているかを確認する.
			// ※このタイルから外に出られるかではなく、周囲から侵入できるかどうか. なお、外周からは侵入可能とする.
			bool buried = false;
			{
				if (x <= 0 || y <= 0 || width - 1 <= x || height - 1 <= y) {
					// outer is not buried.
				}
				else {

				}
			}
		}
	}


	return nullptr;
}

} // namespace detail
} // namespace ln

