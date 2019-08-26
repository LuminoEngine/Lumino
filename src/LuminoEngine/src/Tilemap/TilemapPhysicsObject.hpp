
#include "Internal.hpp"
#include <LuminoEngine/Physics/PhysicsWorld2D.hpp>
#include <LuminoEngine/Tilemap/TilemapLayer.hpp>

namespace ln {
namespace detail {

class TilemapPhysicsObject
{
public:
	static Ref<RigidBody2D> createTilemapBodyFromLayer(const TilemapLayer* layer);
};

} // namespace detail
} // namespace ln

