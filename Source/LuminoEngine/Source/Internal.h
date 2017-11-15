
#pragma once
#define LN_INTERNAL_ACCESS				public
#define LN_PROTECTED_INTERNAL_ACCESS	public

#if defined(_WIN32)
#define NOMINMAX
#include <Windows.h>
#endif
#include <LuminoCore.h>
#include "../Include/Lumino/Graphics/Common.h"	// TODO: Internal.h には置きたくない


namespace ln {
class PhysicsWorld;
class SceneGraphManager;
class SceneGraph2D;
class SceneGraph3D;
class World2D;
class World3D;
class EngineManager;

namespace detail {
class ArchiveManager;
class AnimationManager;
class GraphicsManager;
class ModelManager;
class UIManager;

class EngineDomain
{
public:
	static PhysicsWorld* getPhysicsWorld3D();

	static EngineManager* getEngineManager();
	static ArchiveManager* getArchiveManager();
	static AnimationManager* getAnimationManager();
	static GraphicsManager* getGraphicsManager();
	static ModelManager* getModelManager();
	static UIManager* getUIManager();

	static SceneGraphManager* getSceneGraphManager();
	static SceneGraph2D* getDefaultSceneGraph2D();
	static SceneGraph3D* getDefaultSceneGraph3D();
	static World2D* getDefaultWorld2D();
	static World3D* getDefaultWorld3D();
};

} // namespace detail
} // namespace ln
