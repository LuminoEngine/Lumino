
#pragma once
#include "../Graphics/ColorStructs.hpp"
#include "../Graphics/GeometryStructs.hpp"

namespace ln {
class Level;
class Camera;
class Raycaster;
class RaycastResult;

enum class LevelTransitionMode
{
	/**  */
	FadeInOut,

	/**  */
	CrossFade,
};

} // namespace ln
