
#pragma once

namespace ln
{
class Application;
class PhysicsWorld;
class SceneGraphManager;
class SceneGraph2D;
class SceneGraph3D;
class World2D;
class World3D;
class EngineManager;

namespace detail
{
class ArchiveManager;
class AnimationManager;
class GraphicsManager;
class ModelManager;
class UIManager;

class EngineDomain
{
public:
	//static Application* application;

	static PhysicsWorld* getPhysicsWorld3D();

	static EngineManager* getEngineManager();


	static ArchiveManager* getArchiveManager();
	static AnimationManager* getAnimationManager();
	static GraphicsManager* getGraphicsManager();
	static ModelManager* getModelManager();
	static UIManager* getUIManager();

	static SceneGraphManager* getSceneGraphManager();
	static World2D* defaultWorld2D();
	static World3D* defaultWorld3D();

	static void release();

private:
	static EngineManager* m_engineManager;
};

} // namespace detail
} // namespace ln
