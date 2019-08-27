
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

static Vector2 toWorld(const Vector2& pt, const Vector2& offset)
{
	return Vector2(pt.x + offset.x, -pt.y + offset.y);
}

static Vector2 toWorld(float x, float y, const Vector2& offset)
{
	return Vector2(x + offset.x, -y + offset.y);
}

Ref<EdgeListCollisionShape2D> TilemapPhysicsObject::createTilemapCollisionShape(const TilemapModel* tilemap)
{
	auto shape = makeObject<EdgeListCollisionShape2D>();
	auto tileSize = tilemap->tileSize();

	uint32_t outerFlags = 0x00;	// Tilemap 範囲外の PassageFlags. 0x00 は侵入可。0xFF は侵入不可能。
	int width = tilemap->width();
	int height = tilemap->height();
	auto offset = Vector2(0, height);
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			uint32_t curFlags = tilemap->tilePassageFlags(x, y);
			uint32_t leftFlags = (x <= 0) ? outerFlags : tilemap->tilePassageFlags(x - 1, y);
			uint32_t rightFlags = (width - 1 <= x) ? outerFlags : tilemap->tilePassageFlags(x + 1, y);
			uint32_t upperFlags = (y <= 0) ? outerFlags : tilemap->tilePassageFlags(x, y - 1);
			uint32_t lowerFlags = (height - 1 <= y) ? outerFlags : tilemap->tilePassageFlags(x, y + 1);
			bool leftTo = checkPassage(leftFlags, 6);	// 左からここへ入れるか？
			bool rightTo = checkPassage(rightFlags, 4);
			bool upperTo = checkPassage(upperFlags, 2);
			bool lowerTo = checkPassage(lowerFlags, 8);

			if (!leftTo && !rightTo && !upperTo && !lowerTo) {
				// 周囲のタイルによって、完全に侵入を妨げられている.
				// ※このタイルから外に出られるかではなく、周囲から侵入できるかどうか. 
			}
			else {
				Vector2 pt(x * tileSize.width, y * tileSize.height);

				// left
				{
					if ((curFlags & 0xF) == 0xF && (leftFlags & 0xF) == 0xF) {
						// Fully closed. (そもそもこのエッジに接触するところに侵入できない)
					}
					else if (!leftTo || !checkPassage(curFlags, 4)) {
						// 左タイルからここへ入れない or ここから左へ出られない
						shape->addLine(toWorld(pt, offset), toWorld(pt.x, pt.y + tileSize.height, offset));
					}
				}

				// upper
				{
					if ((curFlags & 0xF) == 0xF && (upperFlags & 0xF) == 0xF) {
						// Fully closed.
					}
					else if (!upperTo || !checkPassage(curFlags, 8)) {
						// 上タイルからここへ入れない or ここから上へ出られない
						shape->addLine(toWorld(pt, offset), toWorld(pt.x + tileSize.width, pt.y, offset));
					}
				}

				// right
				if (x == width - 1) {
					if ((curFlags & 0xF) == 0xF && (rightFlags & 0xF) == 0xF) {
						// Fully closed.
					}
					else if (!rightTo || !checkPassage(curFlags, 6)) {
						// 右タイルからここへ入れない or ここから右へ出られない
						shape->addLine(toWorld(pt.x + tileSize.width, pt.y, offset), toWorld(pt.x + tileSize.width, pt.y + tileSize.height, offset));
					}
				}

				// lower
				if (y == height - 1) {
					if ((curFlags & 0xF) == 0xF && (lowerFlags & 0xF) == 0xF) {
						// Fully closed.
					}
					else if (!rightTo || !checkPassage(curFlags, 2)) {
						// 下タイルからここへ入れない or ここから下へ出られない
						shape->addLine(toWorld(pt.x, pt.y + tileSize.height, offset), toWorld(pt.x + tileSize.width, pt.y + tileSize.height, offset));
					}
				}
#if 0

				if (!leftTo || !checkPassage(curFlags, 4)) {
					// 左タイルからここへ入れない or ここから左へ出られない
					shape->addLine(toWorld(pt, offset), toWorld(pt.x, pt.y + tileSize.height, offset));
				}

				if (!upperTo || !checkPassage(curFlags, 8)) {
					// 上タイルからここへ入れない or ここから上へ出られない
					shape->addLine(toWorld(pt, offset), toWorld(pt.x + tileSize.width, pt.y, offset));
				}

				if (x == width - 1) {
					if (!rightTo || !checkPassage(curFlags, 6)) {
						// 右タイルからここへ入れない or ここから右へ出られない
						shape->addLine(toWorld(pt.x + tileSize.width, pt.y, offset), toWorld(pt.x + tileSize.width, pt.y + tileSize.height, offset));
					}
				}

				if (y == height - 1) {
					if (!rightTo || !checkPassage(curFlags, 2)) {
						// 下タイルからここへ入れない or ここから下へ出られない
						shape->addLine(toWorld(pt.x, pt.y + tileSize.height, offset), toWorld(pt.x + tileSize.width, pt.y + tileSize.height, offset));
					}
				}
#endif
			}
		}
	}

	return shape;
}

} // namespace detail
} // namespace ln

