
#include "Internal.hpp"
#include <LuminoFramework/Physics/PhysicsWorld2D.hpp>
#include <LuminoFramework/Tilemap/TilemapLayer.hpp>
#include <LuminoFramework/Tilemap/TilemapModel.hpp>

namespace ln {
namespace detail {

class TilemapPhysicsObject
{
public:
	static Ref<EdgeListCollisionShape2D> createTilemapCollisionShape(const TilemapModel* tilemap);
};

} // namespace detail
} // namespace ln

