
#pragma once

#define LN_INTERNAL_ACCESS				public
#define LN_CONSTRUCT_ACCESS				public
#define LN_PROTECTED_INTERNAL_ACCESS	public

#define NOMINMAX
#include <LuminoCore.h>
#include "../include/Lumino/Graphics/Common.h"	// TODO: Internal.h には置きたくない


namespace ln {
class PhysicsWorld;
class SceneGraphManager;
class SceneGraph2D;
class SceneGraph3D;
class World2D;
class World3D;

namespace detail {
class GraphicsManager;
class UIManager;

class EngineDomain
{
public:
	static PhysicsWorld* GetPhysicsWorld3D();

	static GraphicsManager* GetGraphicsManager();

	static UIManager* GetUIManager();

	static SceneGraphManager* GetSceneGraphManager();
	static SceneGraph2D* GetDefaultSceneGraph2D();
	static SceneGraph3D* GetDefaultSceneGraph3D();
	static World2D* GetDefaultWorld2D();
	static World3D* GetDefaultWorld3D();
};

} // namespace detail
} // namespace ln
