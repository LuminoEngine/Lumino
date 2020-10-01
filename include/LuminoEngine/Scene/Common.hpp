﻿
#pragma once
#include "../Graphics/ColorStructs.hpp"
#include "../Graphics/GeometryStructs.hpp"

namespace ln {
class StaticMeshModel;
class StaticMeshComponent;

class WorldObject;
class World;
class Level;
class Camera;
class Raycaster;
class RaycastResult;
class Material;
class RenderingContext;
class OffscreenWorldRenderView;
class ReflectorComponent;
class Collision;


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
