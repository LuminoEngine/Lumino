
#include "Internal.hpp"
#include <LuminoEngine/Physics/PhysicsWorld2D.hpp>
#include <LuminoEngine/Tilemap/TilemapLayer.hpp>
#include <LuminoEngine/Tilemap/TilemapModel.hpp>

namespace ln {
namespace detail {

class TilemapPhysicsObject
{
public:
	static Ref<EdgeListCollisionShape2D> createTilemapCollisionShape(const TilemapModel* tilemap);
};

} // namespace detail
} // namespace ln

