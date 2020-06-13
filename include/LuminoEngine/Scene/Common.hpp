
#pragma once
#include "../Graphics/ColorStructs.hpp"
#include "../Graphics/GeometryStructs.hpp"

namespace ln {
class World;
class Level;
class Camera;
class Raycaster;
class RaycastResult;
class Material;
class RenderingContext;
class ReflectorComponent;

enum class LevelTransitionEffectMode
{
	None,

	/**  */
	FadeInOut,

	/**  */
	CrossFade,
};

class IWorldRenderingElement
{
protected:
	//virtual void onPrepareRender(RenderingContext* context) = 0;
	virtual void render(RenderingContext* context) = 0;

	friend class World;
};

} // namespace ln
