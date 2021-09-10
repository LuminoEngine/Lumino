
#pragma once
#include <LuminoEngine/Base/Delegate.hpp>
#include "../Graphics/ColorStructs.hpp"

namespace ln {
class MeshModel;
class MeshComponent;
class ParticleEmitterComponent2;

class WorldObject;
class World;
class WorldRenderView;
class Level;
class Camera;
class Raycaster;
class RaycastResult;
class Material;
class RenderingContext;
class OffscreenWorldRenderView;
class ReflectorComponent;
class Collision;

class CameraOrbitControlComponent;


/** CollisionEventHandler */
LN_DELEGATE()
using CollisionEventHandler = Delegate<void(Collision*)>;

/** レベル遷移時の画面エフェクトの種類 */
LN_ENUM()
enum class LevelTransitionEffectMode
{
	/** エフェクト無し */
	None,

	/** フェードイン・フェードアウト */
	FadeInOut,

	/** クロスフェード */
	CrossFade,
};

class IWorldRenderingElement
{
protected:
	//virtual void onPrepareRender(RenderingContext* context) = 0;
	virtual void render(RenderingContext* context) = 0;

	friend class World;
};

namespace detail {
class IWorldObjectVisitor;
} // namespace detail
} // namespace ln
