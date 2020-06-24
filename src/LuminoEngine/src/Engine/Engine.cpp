
#include "Internal.hpp"
#include "EngineManager.hpp"
//#include "EngineDomain.hpp"
#include <LuminoEngine/Engine/Engine.hpp>
#include <LuminoEngine/Engine/EngineSettings.hpp>
#include <LuminoEngine/Graphics/GraphicsContext.hpp>
#include <LuminoEngine/UI/UIContext.hpp>
#include <LuminoEngine/UI/UIRenderView.hpp>
#include <LuminoEngine/UI/UIFrameWindow.hpp>
#include <LuminoEngine/UI/UIViewport.hpp>
#include <LuminoEngine/UI/UIFocusNavigator.hpp>
#include <LuminoEngine/Physics/PhysicsWorld.hpp>
#include <LuminoEngine/Physics/PhysicsWorld2D.hpp>
#include <LuminoEngine/Scene/WorldRenderView.hpp>
#include <LuminoEngine/Scene/World.hpp>
#include <LuminoEngine/Scene/Camera.hpp>
#include <LuminoEngine/Scene/Light.hpp>
#include "../Graphics/GraphicsManager.hpp"
#include "../Scene/SceneManager.hpp"

namespace ln {

//==============================================================================
// Engine

/** メインウィンドウのクライアント領域の幅と高さを設定します。(default: 640x480) */
void EngineSettings::setMainWindowSize(int width, int height)
{
	detail::EngineManager::s_settings.mainWindowSize.set(width, height);
}

void EngineSettings::setMainWorldViewSize(int width, int height)
{
	detail::EngineManager::s_settings.mainWorldViewSize.set(width, height);
}

void EngineSettings::setMainWindowTitle(const String& title)
{
	detail::EngineManager::s_settings.mainWindowTitle = title;
}

void EngineSettings::addAssetDirectory(const ln::String& path)
{
	detail::EngineManager::s_settings.assetDirectories.add(path);
}

void EngineSettings::addAssetArchive(const ln::String& fileFullPath, const ln::String& password)
{
	detail::EngineManager::s_settings.assetArchives.add({ fileFullPath, password });
}

void EngineSettings::setAssetStorageAccessPriority(AssetStorageAccessPriority value)
{
	detail::EngineManager::s_settings.assetStorageAccessPriority = value;
}

void EngineSettings::setFrameRate(int value)
{
    detail::EngineManager::s_settings.frameRate = value;
}

void EngineSettings::setGraphicsAPI(GraphicsAPI value)
{
	detail::EngineManager::s_settings.graphicsAPI = value;
}

void EngineSettings::setGraphicsDebugEnabled(bool enabled)
{
    detail::EngineManager::s_settings.graphicsDebugEnabled = enabled;
}

void EngineSettings::setDefaultUITheme(const String& value)
{
    detail::EngineManager::s_settings.defaultUITheme = value;
}

void EngineSettings::setDebugToolEnabled(bool enabled)
{
	detail::EngineManager::s_settings.debugToolEnabled = enabled;
}

void EngineSettings::setEngineLogEnabled(bool enabled)
{
	detail::EngineManager::s_settings.engineLogEnabled = enabled;
}

void EngineSettings::setEngineLogFilePath(const ln::String& filePath)
{
	detail::EngineManager::s_settings.engineLogFilePath = filePath;
}

void EngineSettings::setUserMainWindow(intptr_t value)
{
    detail::EngineManager::s_settings.userMainWindow = value;
}

void EngineSettings::setStandaloneFpsControl(bool enabled)
{
    detail::EngineManager::s_settings.standaloneFpsControl = enabled;
}

void EngineSettings::setEngineFeatures(Flags<EngineFeature> features)
{
    detail::EngineManager::s_settings.features = features;
}

void EngineSettings::setDefaultObjectsCreation(bool value)
{
    detail::EngineManager::s_settings.defaultObjectsCreation = value;
}

void EngineSettings::setUseGLFWWindowSystem(bool value)
{
    detail::EngineManager::s_settings.useGLFWWindowSystem = value;
}

void EngineSettings::setGraphicsContextManagement(bool value)
{
    detail::EngineManager::s_settings.graphicsContextManagement = value;
}

void EngineSettings::setExternalMainLoop(bool value)
{
    detail::EngineManager::s_settings.externalMainLoop = value;
}

void EngineSettings::setExternalRenderingManagement(bool value)
{
    detail::EngineManager::s_settings.externalRenderingManagement = value;
}

//==============================================================================
// Engine

static void beginFrame()
{
	detail::EngineDomain::engineManager()->updateFrame();
	detail::EngineDomain::engineManager()->renderFrame();
}

static void endFrame()
{
	detail::EngineDomain::engineManager()->presentFrame();
}

void Engine::initialize()
{
	detail::EngineDomain::engineContext()->initializeEngineManager();
    detail::EngineManager* manager = detail::EngineDomain::engineManager();
    manager->initializeAllManagers();
    if (manager->settings().externalMainLoop) {
        beginFrame();
    }
}

void Engine::finalize()
{
    detail::EngineManager* manager = detail::EngineDomain::engineManager();
	if (manager) {
		if (manager->settings().externalMainLoop) {
			endFrame();
		}
		detail::EngineDomain::engineContext()->disposeEngineManager();
		detail::EngineDomain::engineContext()->disposeRuntimeManager();
	}
}

bool Engine::update()
{

    detail::EngineManager* manager = detail::EngineDomain::engineManager();
    if (manager->settings().externalMainLoop) {
        endFrame();
        beginFrame();
    }
    else {
        beginFrame();
        endFrame();
    }

	return !manager->isExitRequested();
}

void Engine::quit()
{
    detail::EngineDomain::engineManager()->quit();
}

void Engine::resetFrameDelay()
{
    detail::EngineDomain::engineManager()->resetFrameDelay();
}

double Engine::time()
{
    return detail::EngineDomain::engineManager()->fpsController().getTotalGameTime();
}

void Engine::setTimeScale(float value)
{
    detail::EngineDomain::engineManager()->setTimeScale(value);
}

const Path& Engine::persistentDataPath()
{
	return detail::EngineDomain::engineManager()->persistentDataPath();
}

//void Engine::setShowDebugFpsEnabled(bool enabled)
//{
//    return detail::EngineDomain::engineManager()->setShowDebugFpsEnabled(enabled);
//}

GraphicsContext* Engine::graphicsContext()
{
	return detail::EngineDomain::engineManager()->mainWindow()->m_renderingGraphicsContext;
	//return detail::EngineDomain::graphicsManager()->mainWindowGraphicsContext();
}

UIContext* Engine::mainUIContext()
{
    return detail::EngineDomain::engineManager()->mainUIContext();
}

UIFrameWindow* Engine::mainWindow()
{
	return detail::EngineDomain::engineManager()->mainWindow();
}

UIViewport* Engine::mainViewport()
{
    return detail::EngineDomain::engineManager()->mainViewport();
}

UIControl* Engine::mainUIView()
{
	return detail::EngineDomain::engineManager()->mainUIView();
}

Size Engine::mainViewSize()
{
    return mainViewport()->actualViewboxSize();
}

World* Engine::world()
{
    return detail::EngineDomain::engineManager()->mainWorld();
}

Camera* Engine::camera()
{
    return detail::EngineDomain::engineManager()->mainCamera();
}

DirectionalLight* Engine::light()
{
	return detail::EngineDomain::engineManager()->mainWorld()->mainDirectionalLight();
}

AmbientLight* Engine::ambientLight()
{
	return detail::EngineDomain::engineManager()->mainWorld()->mainAmbientLight();
}

//AmbientLight* Engine::mainAmbientLight()
//{
//    return detail::EngineDomain::engineManager()->mainAmbientLight();
//}
//
//DirectionalLight* Engine::mainDirectionalLight()
//{
//    return detail::EngineDomain::engineManager()->mainDirectionalLight();
//}

WorldRenderView* Engine::renderView()
{
    return detail::EngineDomain::engineManager()->mainRenderView();
}

UIFocusNavigator* Engine::navigator()
{
    return static_cast<UIDomainProvidor*>(Engine::mainUIView())->focusNavigator();
}

PhysicsWorld* Engine::mainPhysicsWorld()
{
    return detail::EngineDomain::engineManager()->mainPhysicsWorld();
}

PhysicsWorld2D* Engine::mainPhysicsWorld2D()
{
    return detail::EngineDomain::engineManager()->mainPhysicsWorld2D();
}



World* Engine::activeWorld()
{
    return detail::EngineDomain::engineManager()->sceneManager()->activeWorld();
}

void Engine::setActiveWorld(World* world)
{
    detail::EngineDomain::engineManager()->sceneManager()->setActiveWorld(world);
}

ln::Path Engine::findProjectLocalRoot()
{
    // TODO: proj.ext定数化
    return detail::EngineDomain::engineManager()->findParentDirectoryContainingSpecifiedFile(u"*.lnproj");
}

// TODO: time
//Engine::elapsedSeconds()
//Engine::totalRealTime()
//Engine::frameCount()
//
//World::elapsedSeconds()
//World::totalTime()
//Engine::frameCount()
//World::timeScale


} // namespace ln

