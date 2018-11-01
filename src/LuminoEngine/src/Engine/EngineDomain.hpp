
#pragma once

namespace ln
{
//class Application;
//class PhysicsWorld;
//class SceneGraphManager;
//class SceneGraph2D;
//class SceneGraph3D;
//class World2D;
//class World3D;

namespace detail
{
class EngineManager;
class PlatformManager;
class InputManager;
class AudioManager;
class AnimationManager;
class ShaderManager;
class GraphicsManager;
class FontManager;
class MeshManager;
class RenderingManager;
class AssetManager;
class VisualManager;
class SceneManager;
class UIManager;

class EngineDomain
{
public:
	static void release();
	static EngineManager* engineManager();
	static PlatformManager* platformManager();
	static InputManager* inputManager();
	static AudioManager* audioManager();
	static ShaderManager* shaderManager();
	static GraphicsManager* graphicsManager();
	static FontManager* fontManager();
	static MeshManager* meshManager();
	static RenderingManager* renderingManager();
    static AssetManager* assetManager();
    static VisualManager* visualManager();
    static SceneManager* sceneManager();
	static UIManager* uiManager();

	//static Application* application;

	//static PhysicsWorld* getPhysicsWorld3D();



	//static AnimationManager* getAnimationManager();
	//static GraphicsManager* getGraphicsManager();
	//static UIManager* getUIManager();

	//static SceneGraphManager* getSceneGraphManager();
	//static World2D* defaultWorld2D();
	//static World3D* defaultWorld3D();

	//static void release();

};

} // namespace detail
} // namespace ln
