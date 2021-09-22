
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
class EngineContext;

class World;

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
class EffectManager;
class PhysicsManager;
class VisualManager;
class SceneManager;
class UIManager;
class RuntimeManager;

class EngineDomain
{
public:
	static void release();

	static EngineContext* engineContext();
	static EngineManager* engineManager();
    static AnimationManager* animationManager();
	static InputManager* inputManager();
	static AudioManager* audioManager();
	static ShaderManager* shaderManager();
	static GraphicsManager* graphicsManager();
	static FontManager* fontManager();
	static MeshManager* meshManager();
	static RenderingManager* renderingManager();
    static EffectManager* effectManager();
    static PhysicsManager* physicsManager();
    static VisualManager* visualManager();
    static SceneManager* sceneManager();
	static UIManager* uiManager();

    static World* mainWorld();
	
	template<class T>
	static void registerType() {
		T::_lnref_registerTypeInfo();
	}
};

} // namespace detail
} // namespace ln
