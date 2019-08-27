
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

// dir: NumPad dir
static bool checkPassage(uint32_t fileFlags, int dir)
{
	uint32_t df = 1 << ((dir / 2) - 1);
	return (fileFlags & df) == 0;
}

Ref<EdgeListCollisionShape2D> TilemapPhysicsObject::createTilemapCollisionShape(const TilemapModel* tilemap)
{
	auto shape = makeObject<EdgeListCollisionShape2D>();
	auto tileSize = tilemap->tileSize();

	uint32_t outerFlags = 0x00;	// Tilemap 範囲外の PassageFlags. 0x00 は侵入可。0xFF は侵入不可能。
	int width = tilemap->width();
	int height = tilemap->height();
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			uint32_t curFlags = tilemap->tilePassageFlags(x, y);
			uint32_t leftFlags = (x < 0) ? outerFlags : tilemap->tilePassageFlags(x, y);
			uint32_t rightFlags = (width - 1 <= x) ? outerFlags : tilemap->tilePassageFlags(x, y);
			uint32_t upperFlags = (y < 0) ? outerFlags : tilemap->tilePassageFlags(x, y);
			uint32_t lowerFlags = (height - 1 <= y) ? outerFlags : tilemap->tilePassageFlags(x, y);
			bool leftTo = checkPassage(leftFlags, 6);	// 左からここへ入れるか？
			bool rightTo = checkPassage(rightFlags, 4);
			bool upperTo = checkPassage(upperFlags, 2);
			bool lowerTo = checkPassage(lowerFlags, 8);

			if (leftTo && rightTo && upperTo && lowerTo) {
				// 周囲のタイルによって、完全に侵入を妨げられている.
				// ※このタイルから外に出られるかではなく、周囲から侵入できるかどうか. 
			}
			else {
				Vector2 pt(x * tileSize.width, y * tileSize.height);

				if (!leftTo || !checkPassage(curFlags, 4)) {
					// 左タイルからここへ入れない or ここから左へ出られない
					shape->addLine(pt, Vector2(pt.x, pt.y + tileSize.height));
				}

				if (!upperTo || !checkPassage(curFlags, 8)) {
					// 上タイルからここへ入れない or ここから上へ出られない
					shape->addLine(pt, Vector2(pt.x + tileSize.width, pt.y));
				}

				if (y == width - 1) {
					if (!rightTo || !checkPassage(curFlags, 6)) {
						// 右タイルからここへ入れない or ここから右へ出られない
						shape->addLine(Vector2(pt.x + tileSize.width, pt.y), Vector2(pt.x + tileSize.width, pt.y + tileSize.height));
					}
				}

				if (x == height - 1) {
					if (!rightTo || !checkPassage(curFlags, 2)) {
						// 下タイルからここへ入れない or ここから下へ出られない
						shape->addLine(Vector2(pt.x, pt.y + tileSize.height), Vector2(pt.x + tileSize.width, pt.y + tileSize.height));
					}
				}
			}
		}
	}

	return shape;
}

} // namespace detail
} // namespace ln

