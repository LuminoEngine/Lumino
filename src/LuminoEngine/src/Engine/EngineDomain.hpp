
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
class AnimationManager;
class InputManager;
class AudioManager;
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
    static AnimationManager* animationManager();
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
};

} // namespace detail
} // namespace ln
